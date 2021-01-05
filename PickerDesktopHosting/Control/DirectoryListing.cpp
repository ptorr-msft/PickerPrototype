#include "pch.h"
#include "DirectoryListing.h"
#if __has_include("DirectoryListing.g.cpp")
#include "DirectoryListing.g.cpp"
#endif

using namespace winrt::Windows::UI::Xaml;

namespace winrt::Control::implementation
{
    DependencyProperty DirectoryListing::s_SelectedFile = DependencyProperty::Register(
        winrt::hstring(L"SelectedFile"),
        winrt::xaml_typename<FileEntry>(),
        winrt::xaml_typename<winrt::Control::DirectoryListing>(),
        PropertyMetadata(nullptr, OnSelectedFileChangedHandler));

    DependencyProperty DirectoryListing::s_SelectedDirectory = DependencyProperty::Register(
        winrt::hstring(L"Directory"),
        winrt::xaml_typename<DirectoryEntry>(),
        winrt::xaml_typename<winrt::Control::DirectoryListing>(),
        PropertyMetadata(nullptr, OnDirectoryChangedHandler));

    DirectoryListing::DirectoryListing()
    {
    }

    FileEntry DirectoryListing::SelectedFile()
    {
        return m_SelectedFile;
    }

    DirectoryEntry DirectoryListing::Directory()
    {
        return m_Directory;
    }

    void DirectoryListing::Directory(DirectoryEntry directory)
    {
        m_Directory = directory;
    }

    void DirectoryListing::SelectedFile(FileEntry selectedFile)
    {
        m_SelectedFile = selectedFile;
    }

    winrt::Windows::UI::Xaml::DependencyProperty DirectoryListing::SelectedFileProperty()
    {
        return s_SelectedFile;
    }

    winrt::Windows::UI::Xaml::DependencyProperty DirectoryListing::DirectoryProperty()
    {
        return s_SelectedDirectory;
    }

    void DirectoryListing::OnSelectedFileChangedHandler(winrt::Windows::UI::Xaml::DependencyObject source, winrt::Windows::UI::Xaml::DependencyPropertyChangedEventArgs args)
    {
        if (args.OldValue() == args.NewValue())
        {
            return;
        }

        source.as<DirectoryListing>()->OnSelectedFileChanged(args.NewValue().as<FileEntry>());
    }

    void DirectoryListing::OnDirectoryChangedHandler(winrt::Windows::UI::Xaml::DependencyObject source, winrt::Windows::UI::Xaml::DependencyPropertyChangedEventArgs args)
    {
        if (args.OldValue() == args.NewValue())
        {
            return;
        }

        source.as<DirectoryListing>()->OnDirectoryChanged(args.NewValue().as<DirectoryEntry>());
    }

    void DirectoryListing::OnSelectedFileChanged(FileEntry file)
    {
        m_SelectedFile = file;
    }

    void DirectoryListing::OnDirectoryChanged(DirectoryEntry directory)
    {
        m_Directory = directory;
    }
}
