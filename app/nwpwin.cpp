#include <string>
#include <windows.h>

// Function to load string from resource and return it as std::wstring
std::wstring loadString(HINSTANCE hInstance, UINT id) {
    WCHAR buffer[128];
    ::LoadString(hInstance, id, buffer, sizeof(buffer) / sizeof(WCHAR));
    return std::wstring(buffer);
}

// Function to get a float value from a child window (like an edit control)
float get_child_float(HWND hWndParent, int controlId) {
    WCHAR buffer[50];
    GetWindowText(GetDlgItem(hWndParent, controlId), buffer, sizeof(buffer) / sizeof(WCHAR));
    return _wtof(buffer);
}
