#pragma once
#include <DirectoryListing.g.h>
namespace winrt::Control::implementation
{
    struct DirectoryListing : public DirectoryListingT<DirectoryListing>
    {
    public:
        DirectoryListing();
        FileEntry SelectedFile();
        DirectoryEntry Directory();
        void Directory(DirectoryEntry directory);
        void SelectedFile(FileEntry file);

        static winrt::Windows::UI::Xaml::DependencyProperty SelectedFileProperty();
        static winrt::Windows::UI::Xaml::DependencyProperty DirectoryProperty();

        static void OnSelectedFileChangedHandler(winrt::Windows::UI::Xaml::DependencyObject source, winrt::Windows::UI::Xaml::DependencyPropertyChangedEventArgs args);
        static void OnDirectoryChangedHandler(winrt::Windows::UI::Xaml::DependencyObject source, winrt::Windows::UI::Xaml::DependencyPropertyChangedEventArgs args);

    private:
        void OnSelectedFileChanged(FileEntry fileEntry);
        void OnDirectoryChanged(DirectoryEntry directory);

        FileEntry m_SelectedFile;
        DirectoryEntry m_Directory;

        static winrt::Windows::UI::Xaml::DependencyProperty s_SelectedFile;
        static winrt::Windows::UI::Xaml::DependencyProperty s_SelectedDirectory;
    };
}

namespace winrt::Control::factory_implementation
{
    struct DirectoryListing : public DirectoryListingT<DirectoryListing, implementation::DirectoryListing>
    {
    };
}

