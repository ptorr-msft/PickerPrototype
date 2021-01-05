#pragma once
#include <DirectoryTree.g.h>
#include <queue>

namespace winrt::Control::implementation
{
    struct DirectoryTree : DirectoryTreeT<DirectoryTree>
    {
    public:
        DirectoryTree();
        winrt::Control::DirectoryEntry SelectedItem();
        winrt::Windows::Foundation::Collections::IVectorView<winrt::Control::DirectoryEntry> RootEntries();
        void AddRootEntry(winrt::Control::DirectoryEntry directoryEntry);
        void GoToDirectory(winrt::hstring path);

        static winrt::Windows::UI::Xaml::DependencyProperty SelectedItemProperty();

        bool ApplyTemplate();


    private:
        static winrt::Windows::UI::Xaml::DependencyProperty s_SelectedItem;

        void RunWhenLoaded(std::function<void()> func);
        void OnSelectedItemChanged(DirectoryEntry newEntry);
        void PopulateChildren(winrt::Windows::UI::Xaml::Controls::TreeViewNode node);
        static void OnSelectedItemChangedHandler(winrt::Windows::UI::Xaml::DependencyObject source, winrt::Windows::UI::Xaml::DependencyPropertyChangedEventArgs args);

        winrt::Windows::UI::Xaml::Controls::TreeView m_DirectoryTree;
        winrt::event_token m_ExpandingToken;
        winrt::event_token m_ItemInvokedToken;
        winrt::event_token m_LoadedToken;
        bool m_TreeLoaded{ false };
        std::queue<std::function<void()>> m_TreeLoadedActions;
        std::vector<winrt::Control::DirectoryEntry> m_RootEntries;
    };
}

namespace winrt::Control::factory_implementation
{
    struct DirectoryTree : DirectoryTreeT<DirectoryTree, implementation::DirectoryTree>
    {
    };
}

