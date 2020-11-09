
using System;
using System.Collections.Generic;

using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

// The Templated Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234235

namespace PickerControls
{
    public sealed class FilePickedEventArgs
    {
        public string Path { get; }
        public FilePickedEventArgs(string path)
        {
            Path = path;
        }
    }

    public sealed class FilePicker : Control
    {
        public FilePicker()
        {
            this.DefaultStyleKey = typeof(FilePicker);
        }

        public static DependencyProperty ExtraControlsProperty { get; } = DependencyProperty.Register("ExtraControls",
            typeof(UIElement), typeof(FilePicker), null);

        public UIElement ExtraControls
        {
            get => GetValue(ExtraControlsProperty) as UIElement;
            set => SetValue(ExtraControlsProperty, value);
        }

        DirectoryTree tree;
        DirectoryListing listing;
        Button openButton;
        Button cancelButton;

        protected override void OnApplyTemplate()
        {
            if (openButton!= null)
            {
                openButton.Click -= OpenButton_Click;
            }
            if (cancelButton != null)
            {
                cancelButton.Click -= CancelButton_Click;
            }

            tree = GetTemplateChild("directoryTree") as DirectoryTree;
            InitializeRootDirectories();

            listing = GetTemplateChild("directoryListing") as DirectoryListing;
            openButton = GetTemplateChild("openButton") as Button;
            cancelButton = GetTemplateChild("cancelButton") as Button;

            openButton.Click += OpenButton_Click;
            cancelButton.Click += CancelButton_Click;
        }

        private void InitializeRootDirectories()
        {
            foreach (var entry in rootDirectories)
            {
                tree.AddRootEntry(entry);
            }

            if (suggestedStartLocation != null)
            {
                tree.GoToDirectory(suggestedStartLocation);
            }
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            
        }

        private void OpenButton_Click(object sender, RoutedEventArgs e)
        {
            if (listing.SelectedFile == null)
            {
                return;
            }

            var args = new FilePickedEventArgs(listing.SelectedFile.Path);
            FilePicked?.Invoke(this, args);
        }

        List<DirectoryEntry> rootDirectories = new List<DirectoryEntry>();

        public void AddRootDirectory(DirectoryEntry entry)
        {
            rootDirectories.Add(entry);
            if (tree != null)
            {
                tree.AddRootEntry(entry);
            }
        }

        string suggestedStartLocation;
        public string SuggestedStartLocation 
        { 
            get
            {
                return suggestedStartLocation;
            }
            
            set
            {
                suggestedStartLocation = value;
                if (tree != null)
                {
                    tree.GoToDirectory(value);
                }
            }
        }

        public event EventHandler<FilePickedEventArgs> FilePicked;
    }
}
