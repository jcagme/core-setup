// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "comhost.h"
#include "redirected_error_writer.h"
#include "hostfxr.h"
#include "fxr_resolver.h"
#include "pal.h"
#include "trace.h"
#include "error_codes.h"
#include "utils.h"
#include <type_traits>

using comhost::clsid_map_entry;
using comhost::clsid_map;

#if defined(_WIN32)

// COM entry points are defined without the __declspec(dllexport) attribute.
// The issue here is that the compiler will throw an error regarding linkage
// redefinion. The solution here is to the use a .def file on Windows.
#define COM_API extern "C"

#else

#define COM_API SHARED_API

#endif // _WIN32


//
// See ComActivator class in System.Private.CoreLib
//
struct com_activation_context
{
    GUID class_id;
    GUID interface_id;
    const pal::char_t *assembly_path;
    const pal::char_t *assembly_name;
    const pal::char_t *type_name;
    void **class_factory_dest;
};

using com_activation_fn = int(*)(com_activation_context*);

namespace
{
    int get_com_activation_delegate(pal::string_t *app_path, com_activation_fn *delegate)
    {
        return load_fxr_and_get_delegate(
            hostfxr_delegate_type::com_activation,
            [](const pal::string_t& host_path, pal::string_t* app_path_out)
            {
                pal::string_t app_path_local{ host_path };

                // Strip the comhost suffix to get the 'app'
                size_t idx = app_path_local.rfind(_X(".comhost.dll"));
                assert(idx != pal::string_t::npos);
                app_path_local.replace(app_path_local.begin() + idx, app_path_local.end(), _X(".dll"));

                *app_path_out = std::move(app_path_local);

                return StatusCode::Success;
            },
            delegate,
            app_path
        );
    }
    
    void report_com_error_info(const GUID& guid, pal::string_t errs)
    {
        ICreateErrorInfo *cei;
        if (!errs.empty() && SUCCEEDED(::CreateErrorInfo(&cei)))
        {
            if (SUCCEEDED(cei->SetGUID(guid)))
            {
                if (SUCCEEDED(cei->SetDescription((LPOLESTR)errs.c_str())))
                {
                    IErrorInfo *ei;
                    if (SUCCEEDED(cei->QueryInterface(__uuidof(ei), (void**)&ei)))
                    {
                        ::SetErrorInfo(0, ei);
                        ei->Release();
                    }
                }
            }

            cei->Release();
        }
    }
}

COM_API HRESULT STDMETHODCALLTYPE DllGetClassObject(
    _In_ REFCLSID rclsid,
    _In_ REFIID riid,
    _Outptr_ LPVOID FAR* ppv)
{
    // Check if the CLSID map contains a mapping
    clsid_map map;
    RETURN_HRESULT_IF_EXCEPT(map = comhost::get_clsid_map());

    clsid_map::const_iterator iter = map.find(rclsid);
    if (iter == std::end(map))
        return CLASS_E_CLASSNOTAVAILABLE;

    HRESULT hr;
    pal::string_t app_path;
    com_activation_fn act;
    {
        trace::setup();
        reset_redirected_error_writer();
        
        error_writer_scope_t writer_scope(redirected_error_writer);

        int ec = get_com_activation_delegate(&app_path, &act);
        if (ec != StatusCode::Success)
        {
            report_com_error_info(rclsid, std::move(get_redirected_error_string()));
            return __HRESULT_FROM_WIN32(ec);
        }
    }

    // Query the CLR for the type

    IUnknown *classFactory = nullptr;
    com_activation_context cxt
    {
        rclsid,
        riid,
        app_path.c_str(),
        iter->second.assembly.c_str(),
        iter->second.type.c_str(),
        (void**)&classFactory
    };
    RETURN_IF_FAILED(act(&cxt));
    assert(classFactory != nullptr);

    hr = classFactory->QueryInterface(riid, ppv);
    classFactory->Release();
    return hr;
}

COM_API HRESULT STDMETHODCALLTYPE DllCanUnloadNow(void)
{
    return S_FALSE;
}

#if defined(_WIN32)

namespace
{
    const WCHAR EntryKeyFmt[] = _X("SOFTWARE\\Classes\\CLSID\\%s");

    struct OleStr : public std::unique_ptr<std::remove_pointer<LPOLESTR>::type, decltype(&::CoTaskMemFree)>
    {
        OleStr(_In_z_ LPOLESTR raw)
            : std::unique_ptr<std::remove_pointer<LPOLESTR>::type, decltype(&::CoTaskMemFree)>(raw, ::CoTaskMemFree)
        { }
    };

    struct RegKey : public std::unique_ptr<std::remove_pointer<HKEY>::type, decltype(&::RegCloseKey)>
    {
        RegKey(_In_ HKEY raw)
            : std::unique_ptr<std::remove_pointer<HKEY>::type, decltype(&::RegCloseKey)>(raw, ::RegCloseKey)
        { }
    };

    HRESULT RemoveClsid(_In_ REFCLSID clsid)
    {
        HRESULT hr;

        LPOLESTR clsidAsStrRaw;
        RETURN_IF_FAILED(::StringFromCLSID(clsid, &clsidAsStrRaw));

        OleStr clsidAsStr{ clsidAsStrRaw };

        WCHAR regKeyPath[1024];
        ::swprintf_s(regKeyPath, EntryKeyFmt, clsidAsStr.get());

        LSTATUS res;

        // Handle sub keys
        {
            HKEY toDeleteRaw;
            res = ::RegOpenKeyExW(HKEY_LOCAL_MACHINE, regKeyPath, 0, KEY_READ | KEY_WRITE, &toDeleteRaw);
            if (ERROR_FILE_NOT_FOUND == res)
            {
                return S_OK;
            }
            else if (ERROR_SUCCESS != res)
            {
                return __HRESULT_FROM_WIN32(res);
            }

            RegKey toDelete{ toDeleteRaw };
            res = ::RegDeleteTreeW(toDelete.get(), nullptr);
            if (ERROR_SUCCESS != res)
                return __HRESULT_FROM_WIN32(res);
        }

        res = ::RegDeleteKeyW(HKEY_LOCAL_MACHINE, regKeyPath);
        if (ERROR_SUCCESS != res)
            return __HRESULT_FROM_WIN32(res);

        return S_OK;
    }

    HRESULT RegisterClsid(_In_ REFCLSID clsid, _In_opt_z_ const WCHAR *threadingModel)
    {
        HRESULT hr;

        // Remove the CLSID in case it exists and has undesirable settings
        RETURN_IF_FAILED(RemoveClsid(clsid));

        LPOLESTR clsidAsStrRaw;
        RETURN_IF_FAILED(::StringFromCLSID(clsid, &clsidAsStrRaw));

        OleStr clsidAsStr{ clsidAsStrRaw };

        WCHAR regKeyClsidPath[1024];
        ::swprintf_s(regKeyClsidPath, EntryKeyFmt, clsidAsStr.get());

        HKEY regKeyRaw;
        DWORD disp;
        LSTATUS res = ::RegCreateKeyExW(
            HKEY_LOCAL_MACHINE,
            regKeyClsidPath,
            0,
            REG_NONE,
            REG_OPTION_NON_VOLATILE,
            (KEY_READ | KEY_WRITE),
            nullptr,
            &regKeyRaw,
            &disp);
        if (res != ERROR_SUCCESS)
            return __HRESULT_FROM_WIN32(res);

        RegKey regKey{ regKeyRaw };

        // Set the default value for all COM host servers
        const WCHAR defServerName[] = _X("CoreCLR COMHost Server");
        res = ::RegSetValueExW(
            regKey.get(),
            nullptr,
            0,
            REG_SZ,
            reinterpret_cast<const BYTE*>(defServerName),
            static_cast<DWORD>(sizeof(defServerName) * sizeof(defServerName[0])));
        if (res != ERROR_SUCCESS)
            return __HRESULT_FROM_WIN32(res);

        WCHAR regKeyServerPath[ARRAYSIZE(regKeyClsidPath) * 2];
        ::swprintf_s(regKeyServerPath, L"%s\\InProcServer32", regKeyClsidPath);

        HKEY regServerKeyRaw;
        res = ::RegCreateKeyExW(
            HKEY_LOCAL_MACHINE,
            regKeyServerPath,
            0,
            REG_NONE,
            REG_OPTION_NON_VOLATILE,
            (KEY_READ | KEY_WRITE),
            nullptr,
            &regServerKeyRaw,
            &disp);
        if (res != ERROR_SUCCESS)
            return __HRESULT_FROM_WIN32(res);

        regKey.reset(regServerKeyRaw);

        HMODULE mod;
        if (FALSE == ::GetModuleHandleExW(
            (GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT),
            reinterpret_cast<LPCWSTR>(&RegisterClsid),
            &mod))
        {
            return HRESULT_FROM_WIN32(::GetLastError());
        }

        pal::string_t modPath;
        if (!pal::get_own_module_path(&modPath))
            return E_UNEXPECTED;

        // The default value for the key is the path to the DLL
        res = ::RegSetValueExW(
            regKey.get(),
            nullptr,
            0,
            REG_SZ,
            reinterpret_cast<const BYTE*>(modPath.data()),
            static_cast<DWORD>(modPath.size() + 1) * sizeof(modPath[0]));
        if (res != ERROR_SUCCESS)
            return __HRESULT_FROM_WIN32(res);

        // Set the threading model if provided
        if (threadingModel != nullptr)
        {
            res = ::RegSetValueExW(
                regKey.get(),
                _X("ThreadingModel"),
                0,
                REG_SZ,
                reinterpret_cast<const BYTE*>(threadingModel),
                static_cast<DWORD>(::wcslen(threadingModel) + 1) * sizeof(threadingModel[0]));
            if (res != ERROR_SUCCESS)
                return __HRESULT_FROM_WIN32(res);
        }

        return S_OK;
    }
}

COM_API HRESULT STDMETHODCALLTYPE DllRegisterServer(void)
{
    // Step 0: Initialize logging
    trace::setup();

    // Step 1: Get CLSID mapping
    clsid_map map;
    RETURN_HRESULT_IF_EXCEPT(map = comhost::get_clsid_map());

    trace::info(_X("Registering %d CLSIDs"), (int)map.size());

    // Step 2: Register each CLSID
    HRESULT hr;
    for (clsid_map::const_reference p : map)
        RETURN_IF_FAILED(RegisterClsid(p.first, _X("Both")));

    return S_OK;
}

COM_API HRESULT STDMETHODCALLTYPE DllUnregisterServer(void)
{
    // Step 0: Initialize logging
    trace::setup();

    // Step 1: Get CLSID mapping
    clsid_map map;
    RETURN_HRESULT_IF_EXCEPT(map = comhost::get_clsid_map());

    // Step 2: Unregister each CLSID
    HRESULT hr;
    for (clsid_map::const_reference p : map)
        RETURN_IF_FAILED(RemoveClsid(p.first));

    return S_OK;
}

#endif // _WIN32
