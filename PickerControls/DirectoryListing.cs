using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace PickerControls
{
    public sealed class DirectoryListing : Control
    {
        public DirectoryListing()
        {
            DefaultStyleKey = typeof(DirectoryListing);
            DataContext = this;
        }

        public static DependencyProperty DirectoryProperty { get; } =
            DependencyProperty.Register("Directory", typeof(DirectoryEntry), typeof(DirectoryListing),
            new PropertyMetadata(null, OnDirectoryChangedHandler));

        public DirectoryEntry Directory
        {
            get { return GetValue(DirectoryProperty) as DirectoryEntry; }
            set { SetValue(DirectoryProperty, value); }
        }

        static void OnDirectoryChangedHandler(DependencyObject source, DependencyPropertyChangedEventArgs args)
        {
            if (args.OldValue == args.NewValue)
            {
                return;
            }
            (source as DirectoryListing).OnDirectoryChanged(args.NewValue as DirectoryEntry);
        }

        public static DependencyProperty SelectedFileProperty { get; } =
            DependencyProperty.Register("SelectedFile", typeof(FileEntry), typeof(DirectoryListing),
            new PropertyMetadata(null, OnSelectedFileChangedHandler));

        public FileEntry SelectedFile
        {
            get { return GetValue(SelectedFileProperty) as FileEntry; }
            set { SetValue(SelectedFileProperty, value); }
        }

        static void OnSelectedFileChangedHandler(DependencyObject source, DependencyPropertyChangedEventArgs args)
        {
            if (args.OldValue == args.NewValue)
            {
                return;
            }
            (source as DirectoryListing).OnSelectedFileChanged(args.NewValue as FileEntry);
        }

        DirectoryEntry directory = null;
        bool alreadyChangingDirectory = false;

        FileEntry file = null;
        bool alreadyChangingFile = false;

        void OnSelectedFileChanged(FileEntry fileEntry)
        {
            if (alreadyChangingFile)
            {
                return;
            }

            file = fileEntry;
            if (gridView != null)
            {
                gridView.SelectedItem = fileEntry;
            }
        }

        void OnDirectoryChanged(DirectoryEntry directoryEntry)
        {
            if (alreadyChangingDirectory)
            {
                return;
            }

            directory = directoryEntry;
            if (gridView != null)
            {
                AddDirectoryContents();
            }
        }

        GridView gridView;
        protected override void OnApplyTemplate()
        {
            if (gridView != null)
            {
                gridView.SelectionChanged -= GridView_SelectionChanged;
            }

            gridView = GetTemplateChild("directoryListing") as GridView;
            gridView.SelectionChanged += GridView_SelectionChanged;

            AddDirectoryContents();
        }

        void AddDirectoryContents()
        {
            if (gridView == null || directory == null)
            {
                return;
            }

            gridView.ItemsSource = directory.Files;
        }

        private void GridView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.AddedItems.Count <= 0)
            {
                return;
            }

            alreadyChangingFile = true;
            SelectedFile = e.AddedItems[0] as FileEntry;
            alreadyChangingFile = false;
        }
    }
}
