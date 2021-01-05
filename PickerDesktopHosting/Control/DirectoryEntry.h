#pragma once
#include <DirectoryEntry.g.h>
#include <winrt/base.h>
#include <functional>

namespace winrt::Control::implementation
{
    struct DirectoryEntry : DirectoryEntryT<DirectoryEntry>
    {
        DirectoryEntry();
        DirectoryEntry(winrt::Control::DirectoryEntry parent, winrt::hstring path, winrt::hstring label);
        DirectoryEntry(const winrt::Control::DirectoryEntry& other);
        winrt::hstring Path();
        winrt::hstring Label();
        winrt::Control::DirectoryEntry ParentDirectory();
        winrt::Windows::Foundation::Collections::IVectorView<winrt::Control::DirectoryEntry> Directories();
        winrt::Windows::Foundation::Collections::IVectorView<winrt::Control::FileEntry> Files();
        boolean IsAncestorOf(winrt::hstring path);

        static winrt::Control::DirectoryEntry Downloads();

    private:
        void GetChildren();
        void AddItem(WIN32_FIND_DATAW findData);

        winrt::hstring m_Path;
        winrt::hstring m_Label;
        winrt::Control::DirectoryEntry m_Parent;
        winrt::Windows::Foundation::Collections::IVector<winrt::Control::DirectoryEntry> m_Directories;
        winrt::Windows::Foundation::Collections::IVector<winrt::Control::FileEntry> m_Files;

        static DirectoryEntry s_Downloads;
    };
}

namespace winrt::Control::factory_implementation
{
    struct DirectoryEntry : DirectoryEntryT<DirectoryEntry, implementation::DirectoryEntry>
    {
    };
}
