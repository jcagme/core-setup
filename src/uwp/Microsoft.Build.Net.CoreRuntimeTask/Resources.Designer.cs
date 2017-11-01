﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.42000
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace Microsoft.Build.Net.CoreRuntimeTask {
    using System;
    
    
    /// <summary>
    ///   A strongly-typed resource class, for looking up localized strings, etc.
    /// </summary>
    // This class was auto-generated by the StronglyTypedResourceBuilder
    // class via a tool like ResGen or Visual Studio.
    // To add or remove a member, edit your .ResX file then rerun ResGen
    // with the /str option, or rebuild your VS project.
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("System.Resources.Tools.StronglyTypedResourceBuilder", "4.0.0.0")]
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
    [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
    internal class Resources {
        
        private static global::System.Resources.ResourceManager resourceMan;
        
        private static global::System.Globalization.CultureInfo resourceCulture;
        
        [global::System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1811:AvoidUncalledPrivateCode")]
        internal Resources() {
        }
        
        /// <summary>
        ///   Returns the cached ResourceManager instance used by this class.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        internal static global::System.Resources.ResourceManager ResourceManager {
            get {
                if (object.ReferenceEquals(resourceMan, null)) {
                    global::System.Resources.ResourceManager temp = new global::System.Resources.ResourceManager("Microsoft.Build.Net.CoreRuntimeTask.Resources", typeof(Resources).Assembly);
                    resourceMan = temp;
                }
                return resourceMan;
            }
        }
        
        /// <summary>
        ///   Overrides the current thread's CurrentUICulture property for all
        ///   resource lookups using this strongly typed resource class.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        internal static global::System.Globalization.CultureInfo Culture {
            get {
                return resourceCulture;
            }
            set {
                resourceCulture = value;
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to CopyWin32Resources failed with exit code {0}.
        /// </summary>
        internal static string Error_CopyWin32ResourcesFailed {
            get {
                return ResourceManager.GetString("Error_CopyWin32ResourcesFailed", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Applications with custom entry point executables are not supported. Check Executable attribute of the Application element in the package manifest.
        /// </summary>
        internal static string Error_CustomEntryPointNotSupported {
            get {
                return ResourceManager.GetString("Error_CustomEntryPointNotSupported", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Failed to load resource config file &quot;{0}&quot;.
        /// </summary>
        internal static string Error_FailedToLoadResourceConfig {
            get {
                return ResourceManager.GetString("Error_FailedToLoadResourceConfig", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to WireUpCoreRuntime encountered an error: {0}.
        /// </summary>
        internal static string Error_InternalWireUpCoreRuntimeError {
            get {
                return ResourceManager.GetString("Error_InternalWireUpCoreRuntimeError", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Invalid resource config file &apos;{0}&apos;.
        /// </summary>
        internal static string Error_InvalidResourceConfig {
            get {
                return ResourceManager.GetString("Error_InvalidResourceConfig", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to IO exception while copying file {0}.  {1}.
        /// </summary>
        internal static string Error_IOExceptionCopyFile {
            get {
                return ResourceManager.GetString("Error_IOExceptionCopyFile", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to IO exception while creating output folder. {0}.
        /// </summary>
        internal static string Error_IOExceptionOutputFolder {
            get {
                return ResourceManager.GetString("Error_IOExceptionOutputFolder", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to PathTooLongException while copying file {0}.
        /// </summary>
        internal static string Error_PathTooLongExceptionCopyFile {
            get {
                return ResourceManager.GetString("Error_PathTooLongExceptionCopyFile", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to PathTooLongException while creating output folder.
        /// </summary>
        internal static string Error_PathTooLongExceptionOutputFolder {
            get {
                return ResourceManager.GetString("Error_PathTooLongExceptionOutputFolder", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Framework resource extraction failed. {0}.
        /// </summary>
        internal static string Error_ResourceExtractionFailed {
            get {
                return ResourceManager.GetString("Error_ResourceExtractionFailed", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Encountered an error while checking if the resource cache of &quot;{0}&quot; is up to date: {1}.
        /// </summary>
        internal static string Error_UnspecifiedCheckUpToDate {
            get {
                return ResourceManager.GetString("Error_UnspecifiedCheckUpToDate", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Exception while creating entry point shim for hybrid apps: {0}.
        /// </summary>
        internal static string Error_UnspecifiedCreatingUWPShimForHybrid {
            get {
                return ResourceManager.GetString("Error_UnspecifiedCreatingUWPShimForHybrid", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Encountered an error while saving the resource state for &quot;{0}&quot;: {1}.
        /// </summary>
        internal static string Error_UnspecifiedSaveState {
            get {
                return ResourceManager.GetString("Error_UnspecifiedSaveState", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Cached ResW of assembly &quot;{0}&quot; doesn&apos;t exist: &quot;{1}&quot;.
        /// </summary>
        internal static string Message_CachedReswNotExists {
            get {
                return ResourceManager.GetString("Message_CachedReswNotExists", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Cached ResW &quot;{0}&quot; is not up to date.
        /// </summary>
        internal static string Message_CachedReswNotUpToDate {
            get {
                return ResourceManager.GetString("Message_CachedReswNotUpToDate", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Cached ResW of assembly &quot;{0}&quot; is not up to date, assembly itself is newer.
        /// </summary>
        internal static string Message_CachedReswNotUpToDateAssemblyNewer {
            get {
                return ResourceManager.GetString("Message_CachedReswNotUpToDateAssemblyNewer", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Processing file: &quot;{0}&quot;.
        /// </summary>
        internal static string Message_ProcessingFile {
            get {
                return ResourceManager.GetString("Message_ProcessingFile", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to An error occured while reading the state file: {0}.
        /// </summary>
        internal static string Message_UnspecifiedReadStateFile {
            get {
                return ResourceManager.GetString("Message_UnspecifiedReadStateFile", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to An error occured while writing the state file: {0}.
        /// </summary>
        internal static string Message_UnspecifiedSaveStateFile {
            get {
                return ResourceManager.GetString("Message_UnspecifiedSaveStateFile", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Encountered an error while reading the  state file &quot;{0}&quot;. The state file may be corrupt. Disregarding the state file..
        /// </summary>
        internal static string Message_UnspecifiedStateFileCorrupted {
            get {
                return ResourceManager.GetString("Message_UnspecifiedStateFileCorrupted", resourceCulture);
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to Using cached ResW &quot;{0}&quot; for &quot;{1}&quot;.
        /// </summary>
        internal static string Message_UsingCachedResw {
            get {
                return ResourceManager.GetString("Message_UsingCachedResw", resourceCulture);
            }
        }
    }
}