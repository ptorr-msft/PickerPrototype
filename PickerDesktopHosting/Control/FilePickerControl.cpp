#include "pch.h"
#include "FilePickerControl.h"
#if __has_include("FilePickerControl.g.cpp")
#include "FilePickerControl.g.cpp"
#endif

using namespace winrt;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Controls;

namespace winrt::Control::implementation
{
    DependencyProperty FilePickerControl::s_ExtraControls = DependencyProperty::Register(
        winrt::hstring(L"ExtraControls"),
        winrt::xaml_typename<UIElement>(),
        winrt::xaml_typename<winrt::Control::FilePickerControl>(),
        nullptr);

    FilePickerControl::FilePickerControl()
    {
        InitializeComponent();
    }

    DependencyProperty FilePickerControl::ExtraControlsProperty()
    {
        return s_ExtraControls;
    }

    void FilePickerControl::ExtraControls(UIElement value)
    {
        SetValue(s_ExtraControls, value);
    }

    void FilePickerControl::OnApplyTemplate()
    {
        if (m_OpenButton != nullptr)
        {
            m_OpenButton.Click(m_OpenToken);
        }

        if (m_CancelButton != nullptr)
        {
            m_CancelButton.Click(m_CancelToken);
        }

        m_Tree = GetTemplateChild(winrt::hstring(L"directoryTree")).as<DirectoryTree>();
        InitializeRootDirectories();

        m_Listing = GetTemplateChild(winrt::hstring(L"directoryListing")).as<DirectoryListing>();
        m_OpenButton = GetTemplateChild(winrt::hstring(L"openButton")).as<Button>();
        m_CancelButton = GetTemplateChild(winrt::hstring(L"cancelButton")).as<Button>();

        m_OpenToken = m_OpenButton.Click(RoutedEventHandler([&](auto sender, RoutedEventArgs e)
            {
                if (m_Listing.SelectedFile() == nullptr)
                {
                    return;
                }
            })
        );

        m_CancelToken = m_CancelButton.Click(RoutedEventHandler([&](auto sender, RoutedEventArgs e) 
            {
            })
        );

    }

    /*
    winrt::event_token FilePickerControl::FilePicked(FilePickedHandler handler)
    {
        m_FilePickedHandler = handler;
        return winrt::event_token();
    }

    void FilePickerControl::FilePicked(winrt::event_token& token)
    {
        m_FilePickedHandler = nullptr;
        token.value = 0;
    }
    */

    UIElement FilePickerControl::ExtraControls()
    {
        return s_ExtraControls.as<UIElement>();
    }

    winrt::hstring FilePickerControl::SuggestedStartLocation()
    {
        return m_SuggestedStartLocation;
    }

    void FilePickerControl::SuggestedStartLocation(winrt::hstring startLocation)
    {
        m_SuggestedStartLocation = startLocation;
    }

    void FilePickerControl::InitializeRootDirectories()
    {
        for (auto entry : m_RootDirectories)
        {
            m_Tree.AddRootEntry(entry);
        }

        if (!m_SuggestedStartLocation.empty())
        {
            m_Tree.GoToDirectory(m_SuggestedStartLocation);
        }
    }
}
