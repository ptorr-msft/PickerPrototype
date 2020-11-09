using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;

namespace PickerControls
{
    static class Win32Interop
    {
        internal enum FINDEX_INFO_LEVELS
        {
            FindExInfoStandard = 0,
            FindExInfoBasic = 1
        }

        internal enum FINDEX_SEARCH_OPS
        {
            FindExSearchNameMatch = 0,
            FindExSearchLimitToDirectories = 1,
            FindExSearchLimitToDevices = 2
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        internal struct WIN32_FIND_DATA
        {
            public FileAttributes dwFileAttributes;
            public FILETIME ftCreationTime;
            public FILETIME ftLastAccessTime;
            public FILETIME ftLastWriteTime;
            public uint nFileSizeHigh;
            public uint nFileSizeLow;
            public uint dwReserved0;
            public uint dwReserved1;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
            public string cFileName;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 14)]
            public string cAlternateFileName;
        }

        [DllImport("api-ms-win-core-file-fromapp-l1-1-0", SetLastError = true, CharSet = CharSet.Unicode)]
        internal static extern IntPtr FindFirstFileExFromAppW(
                string lpFileName,
                FINDEX_INFO_LEVELS fInfoLevelId,
                out WIN32_FIND_DATA lpFindFileData,
                FINDEX_SEARCH_OPS fSearchOp,
                IntPtr lpSearchFilter,
                int dwAdditionalFlags);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        internal static extern bool FindNextFile(
            IntPtr hFindFile,
            out WIN32_FIND_DATA lpFindFileData);

        [DllImport("kernel32.dll")]
        internal static extern bool FindClose(
            IntPtr hFindFile);

        internal const int FIND_FIRST_EX_LARGE_FETCH = 2;
        internal const long INVALID_HANDLE_VALUE = -1;

        [DllImport("kernel32.dll")]
        internal static extern UInt32 GetLogicalDrives();
    }
}
