#include "pch.h"
#include "FileEntry.h"
#if __has_include("FileEntry.g.cpp")
#include "FileEntry.g.cpp"
#endif

using namespace winrt;

namespace winrt::Control::implementation
{
    FileEntry::FileEntry()
    {
    }

    FileEntry::FileEntry(DirectoryEntry parent, hstring path, hstring label, int64_t size) : m_Parent(parent), m_Path(path), m_Label(label), m_Size(size)
    {
    }

    DirectoryEntry FileEntry::Parent()
    {
        return m_Parent;
    }

    winrt::hstring FileEntry::Path()
    {
        return m_Path;
    }

    winrt::hstring FileEntry::Label()
    {
        return m_Label;
    }

    int64_t FileEntry::Size()
    {
        return m_Size;
    }
}

