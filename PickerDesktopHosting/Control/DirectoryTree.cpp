#include "pch.h"
#include "DirectoryTree.h"
#if __has_include("DirectoryTree.g.cpp")
#include "DirectoryTree.g.cpp"
#endif

using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml::Controls;

namespace winrt::Control::implementation
{
    DependencyProperty DirectoryTree::s_SelectedItem = DependencyProperty::Register(
                                                            winrt::hstring(L"SelectedItem"),
                                                            winrt::xaml_typename<DirectoryEntry>(),
                                                            winrt::xaml_typename<winrt::Control::DirectoryTree>(),
                                                            PropertyMetadata(nullptr, OnSelectedItemChangedHandler)
                                                       );

    DirectoryTree::DirectoryTree()
    {
    }

    winrt::Control::DirectoryEntry DirectoryTree::SelectedItem()
    {
        return winrt::Control::DirectoryEntry();
    }

    winrt::Windows::Foundation::Collections::IVectorView<winrt::Control::DirectoryEntry> DirectoryTree::RootEntries()
    {
        return winrt::single_threaded_vector(std::move(m_RootEntries)).GetView();
    }

    void DirectoryTree::OnSelectedItemChangedHandler(DependencyObject source, DependencyPropertyChangedEventArgs args)
    {
        if (args.OldValue() == args.NewValue())
        {
            return;
        }

        source.as<DirectoryTree>()->OnSelectedItemChanged(args.NewValue().as<DirectoryEntry>());
    }

    void DirectoryTree::OnSelectedItemChanged(DirectoryEntry newEntry)
    {
    }

    DependencyProperty DirectoryTree::SelectedItemProperty()
    {
        return s_SelectedItem;
    }

    bool DirectoryTree::ApplyTemplate()
    {
        if (m_DirectoryTree)
        {
            m_DirectoryTree.Expanding(m_ExpandingToken);
            m_DirectoryTree.ItemInvoked(m_ItemInvokedToken);
            m_DirectoryTree.as<winrt::Windows::UI::Xaml::Controls::Control>().Loaded(m_LoadedToken);
            m_DirectoryTree = nullptr;
        }

        m_TreeLoaded = false;
        m_DirectoryTree = GetTemplateChild(winrt::hstring(L"directoryTree")).as<TreeView>();
        m_ExpandingToken = m_DirectoryTree.Expanding(TypedEventHandler<TreeView, TreeViewExpandingEventArgs>([&](TreeView treeView, TreeViewExpandingEventArgs args)
            {
                PopulateChildren(args.Node());
            })
        );

        m_ItemInvokedToken = m_DirectoryTree.ItemInvoked(TypedEventHandler<TreeView, TreeViewItemInvokedEventArgs>([&](TreeView treeView, TreeViewItemInvokedEventArgs args)
            {
                SetValue(SelectedItemProperty(), args.InvokedItem().as<TreeViewNode>().Content());
                args.Handled(true);
            })
        );

        m_LoadedToken = m_DirectoryTree.as<winrt::Windows::UI::Xaml::Controls::Control>().Loaded(RoutedEventHandler([&](auto sender, RoutedEventArgs args) 
            {
                m_TreeLoaded = true;
                m_DirectoryTree.as<winrt::Windows::UI::Xaml::Controls::Control>().Loaded(m_LoadedToken);
                while (m_TreeLoadedActions.size() > 0)
                {
                    std::function<void()> loadedFunction = m_TreeLoadedActions.front();
                    m_TreeLoadedActions.pop();
                    loadedFunction();
                }
            })
        );

        RunWhenLoaded([&]() 
            {
                for (auto entry : m_RootEntries)
                {
                    auto node = TreeViewNode();
                    node.Content(entry.as<IInspectable>());
                    node.HasUnrealizedChildren(true);
                    m_DirectoryTree.RootNodes().Append(node);
                }
            });

        return true;
    }

    void DirectoryTree::RunWhenLoaded(std::function<void()> fn)
    {
        if (m_TreeLoaded)
        {
            fn();
        }
        else
        {
            m_TreeLoadedActions.push(fn);
        }
    }

    void DirectoryTree::AddRootEntry(DirectoryEntry entry)
    {
        m_RootEntries.push_back(entry);

        std::function<void()> loadedFn = [&]()
        {
            auto node = TreeViewNode();
            node.Content(entry.as<IInspectable>());
            node.HasUnrealizedChildren(true);
            m_DirectoryTree.RootNodes().Append(node);
        };

        RunWhenLoaded(loadedFn);
    }

    void DirectoryTree::GoToDirectory(winrt::hstring path)
    {
        RunWhenLoaded([&]()
            {
                TreeViewNode node;
                TreeViewNode lastNode;

                for (auto rootNode : m_DirectoryTree.RootNodes())
                {
                    auto rootEntry = rootNode.Content().as<DirectoryEntry>();
                    if (rootEntry.IsAncestorOf(path))
                    {
                        node = rootNode;
                        break;
                    }
                }

                while (node != nullptr)
                {
                    lastNode = node;
                    PopulateChildren(node);
                    node.IsExpanded(true);

                    bool found{ false };

                    for (auto child : node.Children())
                    {
                        auto childEntry = child.Content().as<DirectoryEntry>();
                        if (childEntry.IsAncestorOf(path))
                        {
                            node = child;
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        break;
                    }
                }

                if (lastNode != nullptr)
                {
                    auto item = m_DirectoryTree.ContainerFromNode(lastNode).as<TreeViewItem>();

                    if (item != nullptr)
                    {
                        item.IsSelected(true);
                        item.StartBringIntoView();
                    }
                }
            }
        );
    }

    void DirectoryTree::PopulateChildren(TreeViewNode node)
    {
        if (node.Children().Size() > 0)
        {
            return;
        }

        auto children = node.Content().as<DirectoryEntry>().Directories();
        node.HasUnrealizedChildren(false);

        if (children.Size() > 0)
        {
            for (auto child : children)
            {
                auto childNode = TreeViewNode();
                childNode.Content(child);
                childNode.HasUnrealizedChildren(true);
                node.Children().Append(childNode);
            }
        }
    }
}
