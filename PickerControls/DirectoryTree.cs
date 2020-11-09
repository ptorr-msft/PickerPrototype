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
            //            set { SetValue(SelectedItemProperty, value); }
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

        protected override void OnApplyTemplate()
        {
            if (directoryTree != null)
            {
                directoryTree.Expanding -= DirectoryTree_Expanding;
                directoryTree.ItemInvoked -= DirectoryTree_ItemInvoked;
                directoryTree = null;
            }

            directoryTree = (TreeView)GetTemplateChild("directoryTree");
            directoryTree.Expanding += DirectoryTree_Expanding;
            directoryTree.ItemInvoked += DirectoryTree_ItemInvoked;

            InitializeTree();
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

            if (goToDirectory != null)
            {
                GoToDirectory(goToDirectory);
            }
        }

        public void AddRootEntry(DirectoryEntry entry)
        {
            (RootEntries as List<DirectoryEntry>).Add(entry);

            // Already initialized; need to add manually
            if (directoryTree != null)
            {
                var node = new TreeViewNode();
                node.Content = entry;
                node.HasUnrealizedChildren = true;
                directoryTree.RootNodes.Add(node);
            }
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

        string goToDirectory;
        public void GoToDirectory(string path)
        {
            if (directoryTree == null)
            {
                goToDirectory = path;
                return;
            }

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
                directoryTree.SelectedNodes.Clear();
                directoryTree.Dispatcher.RunAsync(Windows.UI.Core.CoreDispatcherPriority.Normal,
                    () => directoryTree.SelectedNodes.Add(lastNode));
                SetValue(SelectedItemProperty, lastNode.Content);
            }
        }
    }
}
