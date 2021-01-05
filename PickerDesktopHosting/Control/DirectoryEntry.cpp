#include "pch.h"
#include "DirectoryEntry.h"
#if __has_include("DirectoryEntry.g.cpp")
#include "DirectoryEntry.g.cpp"
#endif

#ifdef WINAPI_FAMILY
#undef WINAPI_FAMILY
#endif
#define WINAPI_FAMILY (WINAPI_FAMILY_DESKTOP_APP|WINAPI_PARTITION_DESKTOP)
#ifdef NOSHLWAPI
#undef NOSHLWAPI
#endif
#ifdef NO_SHLWAPI_PATH
#undef NO_SHLWAPI_PATH
#endif
#include <Shlwapi.h>

using namespace winrt;
using namespace winrt::Windows::Foundation::Collections;

namespace winrt::Control::implementation
{
    DirectoryEntry DirectoryEntry::s_Downloads = DirectoryEntry();

    DirectoryEntry::DirectoryEntry()
    {
    }

    DirectoryEntry::DirectoryEntry(winrt::Control::DirectoryEntry parent, winrt::hstring path, winrt::hstring label) : m_Label(label)
    {
        m_Parent = parent;

        if (std::wstring(path).back() != '\\')
        {
            std::wstring temp = (std::wstring)path;
            temp.push_back('\\');
            m_Path = temp;
        }
        else
        {
            m_Path = path;
        }
    }

    DirectoryEntry::DirectoryEntry(const winrt::Control::DirectoryEntry& other)
    {
        this->m_Path = other.Path();
        this->m_Parent = other.ParentDirectory();
        this->m_Label = other.Label();
    }

    winrt::hstring DirectoryEntry::Path()
    {
        return m_Path;
    }

    winrt::hstring DirectoryEntry::Label()
    {
        return m_Label;
    }

    winrt::Control::DirectoryEntry DirectoryEntry::ParentDirectory()
    {
        return m_Parent.as<winrt::Control::DirectoryEntry>();
    }

    winrt::Windows::Foundation::Collections::IVectorView<winrt::Control::DirectoryEntry> DirectoryEntry::Directories()
    {
        return m_Directories.GetView();
    }

    winrt::Windows::Foundation::Collections::IVectorView<winrt::Control::FileEntry> DirectoryEntry::Files()
    {
        return m_Files.GetView();
    }

    boolean DirectoryEntry::IsAncestorOf(winrt::hstring path)
    {
        return std::wstring(m_Path)._Starts_with((std::wstring)path);
    }

    winrt::Control::DirectoryEntry DirectoryEntry::Downloads()
    {
        if (s_Downloads.m_Path.empty())
        {
            LPWSTR expandedString = nullptr;
            int bufferSize = 0;
            bufferSize = ExpandEnvironmentStringsW(L"%userprofile%\\downloads", expandedString, 0);

            if (bufferSize > 0)
            {
                expandedString = (LPWSTR)malloc(bufferSize * sizeof(wchar_t));
                ExpandEnvironmentStringsW(L"%userprofile%\\downloads", expandedString, bufferSize);
                s_Downloads.m_Parent = nullptr;
                s_Downloads.m_Path = winrt::to_hstring(expandedString);
                s_Downloads.m_Label = winrt::to_hstring(L"Downloads");

                delete[] expandedString;
            }
        }

        return s_Downloads;
    }

    void DirectoryEntry::GetChildren()
    {
        std::wstring path = ((std::wstring)m_Path).append(L"*");
        WIN32_FIND_DATAW findData{};

        wil::unique_handle findHandle(FindFirstFileW(path.c_str(), &findData));
        wil::verify_bool(findHandle.get() != INVALID_HANDLE_VALUE);

        do
        {
            AddItem(findData);
        } while (FindNextFileW(findHandle.get(), &findData) == TRUE);
    }

    void DirectoryEntry::AddItem(WIN32_FIND_DATAW findData)
    {
        if ((wcscmp(findData.cFileName, L".") != 0) && (wcscmp(findData.cFileName, L"..") != 0))
        {
            std::wstring combinedPath;
            PathCombineW(combinedPath.data(), m_Path.c_str(), findData.cFileName);

            if (PathIsDirectoryW(combinedPath.c_str()))
            {
                m_Directories.Append(DirectoryEntry(*this, winrt::to_hstring(combinedPath.c_str()), winrt::hstring(findData.cFileName)));
            }
            else
            {
                m_Files.Append(FileEntry(*this, combinedPath.c_str(), findData.cFileName, (int64_t)(findData.nFileSizeLow) + (findData.nFileSizeHigh << 16)));
            }
        }
    }
}
