#pragma once

#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Windows.UI.Xaml.Markup.h"
#include "winrt/Windows.UI.Xaml.Interop.h"
#include "winrt/Windows.UI.Xaml.Controls.Primitives.h"
#include "FilePickerControl.g.h"

namespace winrt::Control::implementation
{
    struct FilePickerControl : public FilePickerControlT<FilePickerControl>
    {
    public:
        FilePickerControl();
        winrt::Windows::UI::Xaml::UIElement ExtraControls();
        void ExtraControls(winrt::Windows::UI::Xaml::UIElement value);
        winrt::hstring SuggestedStartLocation();
        void SuggestedStartLocation(winrt::hstring startLocation);
        void OnApplyTemplate();
        static winrt::Windows::UI::Xaml::DependencyProperty ExtraControlsProperty();

    private:
        void InitializeRootDirectories();

        static winrt::Windows::UI::Xaml::DependencyProperty s_ExtraControls;

        DirectoryTree m_Tree;
        DirectoryListing m_Listing;
        winrt::Windows::UI::Xaml::Controls::Button m_OpenButton;
        winrt::Windows::UI::Xaml::Controls::Button m_CancelButton;
        winrt::hstring m_SuggestedStartLocation;

        winrt::event_token m_OpenToken;
        winrt::event_token m_CancelToken;
        std::vector<DirectoryEntry> m_RootDirectories;
    };
}

namespace winrt::Control::factory_implementation
{
    struct FilePickerControl : public FilePickerControlT<FilePickerControl, implementation::FilePickerControl>
    {
    };
}
