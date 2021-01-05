#pragma once
#include "FileEntry.g.h"

namespace winrt::Control::implementation
{
    struct FileEntry : public FileEntryT<FileEntry>
    {
    public:
        FileEntry();
        FileEntry(DirectoryEntry parent, winrt::hstring path, winrt::hstring label, int64_t size);
        DirectoryEntry Parent();
        winrt::hstring Path();
        winrt::hstring Label();
        int64_t Size();

    private:
        winrt::hstring m_Path;
        winrt::hstring m_Label;
        int64_t m_Size{ 0 };
        DirectoryEntry m_Parent;
    };
}

namespace winrt::Control::factory_implementation
{
    struct FileEntry : public FileEntryT<FileEntry, implementation::FileEntry>
    {
    };
}

