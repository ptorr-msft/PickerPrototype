using System;
using System.Collections.Generic;
using System.IO;

namespace PickerControls
{
    public sealed class DirectoryEntry
    {
        public string Path { get; }
        public string Label { get; }
        public DirectoryEntry Parent { get; }

        List<DirectoryEntry> directories;
        public IReadOnlyList<DirectoryEntry> Directories => GetDirectories();

        private IReadOnlyList<DirectoryEntry> GetDirectories()
        {
            if (directories == null)
            {
                GetChildren();
            }

            return directories;
        }

        List<FileEntry> files;
        public IReadOnlyList<FileEntry> Files => GetFiles();
        Action<DirectoryEntry> populateChildrenCallback;

        private IReadOnlyList<FileEntry> GetFiles()
        {
            if (files == null)
            {
                GetChildren();
            }

            return files;
        }

        private void GetChildren()
        {
            directories = new List<DirectoryEntry>();
            files = new List<FileEntry>();

            if (populateChildrenCallback != null)
            {
                populateChildrenCallback(this);
                return;
            }

            string path = Path + "*";
            Win32Interop.WIN32_FIND_DATA data;
            IntPtr hFind = Win32Interop.FindFirstFileExFromAppW(path,
                Win32Interop.FINDEX_INFO_LEVELS.FindExInfoBasic,
                out data,
                Win32Interop.FINDEX_SEARCH_OPS.FindExSearchNameMatch,
                IntPtr.Zero,
                Win32Interop.FIND_FIRST_EX_LARGE_FETCH);

            if (hFind.ToInt64() == Win32Interop.INVALID_HANDLE_VALUE)
            {
                return;
            }

            do
            {
                AddItem(data);
            } while (Win32Interop.FindNextFile(hFind, out data));

            Win32Interop.FindClose(hFind);

            void AddItem(Win32Interop.WIN32_FIND_DATA itemData)
            {
                if (itemData.cFileName == "." || itemData.cFileName == "..")
                {
                    return;
                }

                string fullPath = System.IO.Path.Combine(Path, itemData.cFileName);

                if (itemData.dwFileAttributes.HasFlag(FileAttributes.Directory))
                {
                    directories.Add(new DirectoryEntry(fullPath,
                        itemData.cFileName,
                        this));
                }
                else
                {
                    files.Add(new FileEntry(fullPath,
                        itemData.cFileName,
                        itemData.nFileSizeLow + (itemData.nFileSizeHigh << 16),
                        this));
                }
            }
        }

        public DirectoryEntry(string path, string label, DirectoryEntry parent)
        {
            if (!path.EndsWith(@"\"))
            {
                Path = path + @"\";
            }
            else
            {
                Path = path;
            }

            Label = label;
            Parent = parent;
        }

        private DirectoryEntry(string label, Action<DirectoryEntry> populateChildrenCallback)
        {
            Path = "";
            Label = label;
            this.populateChildrenCallback = populateChildrenCallback;
        }

        public override string ToString()
        {
            if (string.IsNullOrEmpty(Path))
            {
                return Label;
            }

            return Path;
        }

        public bool IsAncestorOf(string path)
        {
            return path.StartsWith(Path, StringComparison.OrdinalIgnoreCase);
        }

        static DirectoryEntry thisPcEntry = null;
        public static DirectoryEntry ThisPc
        {
            get
            {
                if (thisPcEntry == null)
                {
                    thisPcEntry = new DirectoryEntry("This PC", entry =>
                    {
                        var drives = Environment.GetLogicalDrives();
                        foreach (var drive in drives)
                        {
                            entry.directories.Add(new DirectoryEntry(drive, drive, entry));
                        }
                        //char drive = 'a';
                        //for (int i = 0; i < 32; ++i)
                        //{
                        //    if ((drives & 1) == 1)
                        //    {
                        //        var path = $@"{drive}:\";
                        //        var newItem = new DirectoryEntry(path, path, entry);
                        //        entry.directories.Add(newItem);
                        //    }
                        //    ++drive;
                        //    drives >>= 1;
                        //}
                    });
                }

                return thisPcEntry;
            }
        }

        static DirectoryEntry downloadsEntry = null;
        public static DirectoryEntry Downloads
        {
            get
            {
                if (downloadsEntry == null)
                {
                    downloadsEntry = new DirectoryEntry(Environment.ExpandEnvironmentVariables(@"%userprofile%\downloads\"),
                        "Downloads", null);
                }

                return downloadsEntry;
            }
        }
    }
}
