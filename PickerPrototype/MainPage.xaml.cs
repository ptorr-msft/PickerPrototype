using PickerControls;

using System;

using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace PickerPrototype
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();

            picker.AddRootDirectory(DirectoryEntry.Downloads);
            var entry = new DirectoryEntry(@"C:\Users", "Users", null);
            picker.AddRootDirectory(entry);
            picker.AddRootDirectory(DirectoryEntry.ThisPc);

            picker.SuggestedStartLocation = DirectoryEntry.Downloads.Path;
        }

        private void RadioButton_Click(object sender, RoutedEventArgs e)
        {
            picker.SuggestedStartLocation = @"C:\Windows\System32\";
        }

        private void RadioButton_Click_1(object sender, RoutedEventArgs e)
        {
            picker.SuggestedStartLocation = @"C:\Users\ptorr\";
        }

        private void RadioButton_Click_2(object sender, RoutedEventArgs e)
        {
            picker.SuggestedStartLocation = @"c:\Program Files\Microsoft Office\";
        }

        private async void picker_FilePicked(object sender, PickerControls.FilePickedEventArgs e)
        {
            await (new MessageDialog(e.Path, "You picked...")).ShowAsync();
        }
    }
}
