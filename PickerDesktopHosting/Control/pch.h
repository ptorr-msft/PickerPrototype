#pragma once
#include <windows.h>
#include <unknwn.h>
#include <restrictederrorinfo.h>
#include <hstring.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>
#include <winrt/Windows.UI.Xaml.Data.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Windows.UI.Xaml.Markup.h>
#include <winrt/Windows.UI.Xaml.Navigation.h>
#include <string>
#include <functional>
#include <vector>

#define WIL_ENABLE_EXCEPTIONS
#include <wil/resource.h>
#include <wil/result_macros.h>

#ifdef WINAPI_FAMILY_PARTITION
#undef WINAPI_FAMILY_PARTITION
#endif
#define WINAPI_FAMILY_PARTITION (WINAPI_PARTITION_APP | WINAPI_PARTITION_SYSTEM | WINAPI_PARTITION_GAMES | WINAPI_PARITION_DESKTOP)
#include <fileapi.h>

