using System;
using System.Collections.Generic;

using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace PickerControls
{
    public sealed class DirectoryTree : Control
    {
        public DirectoryTree()
        {
            DefaultStyleKey = typeof(DirectoryTree);
        }

        public static DependencyProperty SelectedItemProperty { get; } =
            DependencyProperty.Register("SelectedItem", typeof(DirectoryEntry), typeof(DirectoryTree),
                new PropertyMetadata(null, OnSelectedItemChangedHandler));

        public DirectoryEntry SelectedItem
        {
            get { return GetValue(SelectedItemProperty) as DirectoryEntry; }
        }

        static void OnSelectedItemChangedHandler(DependencyObject source, DependencyPropertyChangedEventArgs args)
        {
            if (args.OldValue == args.NewValue)
            {
                return;
            }
            (source as DirectoryTree).OnSelectedItemChanged(args.NewValue as DirectoryEntry);
        }

        void OnSelectedItemChanged(DirectoryEntry newEntry)
        {
            // TODO?
        }

        TreeView directoryTree;
        bool treeLoaded = false;
        Queue<Action> treeLoadedActions = new Queue<Action>();

        protected override void OnApplyTemplate()
        {
            if (directoryTree != null)
            {
                directoryTree.Expanding -= DirectoryTree_Expanding;
                directoryTree.ItemInvoked -= DirectoryTree_ItemInvoked;
                directoryTree.Loaded -= DirectoryTree_Loaded;
                directoryTree = null;
            }

            treeLoaded = false;

            directoryTree = (TreeView)GetTemplateChild("directoryTree");
            directoryTree.Expanding += DirectoryTree_Expanding;
            directoryTree.ItemInvoked += DirectoryTree_ItemInvoked;
            directoryTree.Loaded += DirectoryTree_Loaded;

            RunWhenLoaded(InitializeTree);
        }

        void RunWhenLoaded(Action a)
        {
            if (treeLoaded)
            {
                a();
            }
            else
            {
                treeLoadedActions.Enqueue(a);
            }
        }

        private void DirectoryTree_Loaded(object sender, RoutedEventArgs e)
        {
            treeLoaded = true;
            directoryTree.Loaded -= DirectoryTree_Loaded;
            while (treeLoadedActions.Count > 0)
            {
                treeLoadedActions.Dequeue()();
            }
        }

        public IReadOnlyList<DirectoryEntry> RootEntries { get; } = new List<DirectoryEntry>();

        private void InitializeTree()
        {
            foreach (var entry in RootEntries)
            {
                var node = new TreeViewNode();
                node.Content = entry;
                node.HasUnrealizedChildren = true;
                directoryTree.RootNodes.Add(node);
            }
        }

        public void AddRootEntry(DirectoryEntry entry)
        {
            (RootEntries as List<DirectoryEntry>).Add(entry);

            RunWhenLoaded(() =>
            {
                var node = new TreeViewNode();
                node.Content = entry;
                node.HasUnrealizedChildren = true;
                directoryTree.RootNodes.Add(node);
            });
        }

        private void DirectoryTree_ItemInvoked(TreeView sender, TreeViewItemInvokedEventArgs args)
        {
            SetValue(SelectedItemProperty, (args.InvokedItem as TreeViewNode).Content);
            args.Handled = true;
        }

        private void DirectoryTree_Expanding(TreeView sender, TreeViewExpandingEventArgs args)
        {
            PopulateChildren(args.Node);
        }

        private void PopulateChildren(TreeViewNode node)
        {
            if (node.Children.Count > 0)
            {
                return;
            }

            var children = (node.Content as DirectoryEntry).Directories;
            node.HasUnrealizedChildren = false;

            if (children.Count == 0)
            {
                return;
            }

            foreach (var child in children)
            {
                var childNode = new TreeViewNode();
                childNode.Content = child;
                childNode.HasUnrealizedChildren = true;
                node.Children.Add(childNode);
            }
        }

        public void GoToDirectory(string path)
        {
            RunWhenLoaded(() =>
            {
                TreeViewNode node = null;
                TreeViewNode lastNode = null;

                foreach (var rootNode in directoryTree.RootNodes)
                {
                    var rootEntry = rootNode.Content as DirectoryEntry;
                    if (rootEntry.IsAncestorOf(path))
                    {
                        node = rootNode;
                        break;
                    }
                }

                while (node != null)
                {
                    lastNode = node;
                    PopulateChildren(node);
                    node.IsExpanded = true;
                    bool found = false;
                    foreach (var child in node.Children)
                    {
                        var childEntry = child.Content as DirectoryEntry;
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

                if (lastNode != null)
                {
                    var item = directoryTree.ContainerFromNode(lastNode) as TreeViewItem;
                    
                    // Usually this doesn't work... XAML bug / missing feature?
                    if (item != null)
                    {
                        item.IsSelected = true;
                        item.StartBringIntoView();
                    }
                }
            });
        }
    }
}
