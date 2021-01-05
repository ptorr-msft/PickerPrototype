#include "pch.h"
#include "FilePickedEventArgs.h"

namespace winrt::Control::implementation
{
    FilePickedEventArgs::FilePickedEventArgs()
    {
    }

    FilePickedEventArgs::FilePickedEventArgs(winrt::hstring path) : m_Path(path)
    {
    }

    winrt::hstring FilePickedEventArgs::Path()
    {
        return m_Path;
    }

    void FilePickedEventArgs::Path(winrt::hstring path)
    {
        m_Path = path;
    }
}