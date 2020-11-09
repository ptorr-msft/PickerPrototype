using System;

namespace PickerControls
{
    public sealed class FileEntry
    {
        public string Path { get; }
        public string Label { get; }
        public long Size { get; }
        public DirectoryEntry Parent { get; }
        public FileEntry(string path, string label, long size, DirectoryEntry parent)
        {
            if (path.EndsWith(@"\"))
            {
                throw new ArgumentException("Must be a filename", nameof(path));
            }

            Path = path;
            Label = label;
            Size = size;
            Parent = parent;
        }
    }
}
