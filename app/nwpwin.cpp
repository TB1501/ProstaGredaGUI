#include <string>
#include <windows.h>
#include <array>

// Function to load string from resource and return it as std::wstring
std::wstring load_string(UINT id) {
    std::array<WCHAR, 128> buffer;
    ::LoadString(0, id, buffer.data(), buffer.size() / sizeof(WCHAR));
    return buffer.data();
}

// Function to get a float value from a child window (like an edit control)
float get_child_float(HWND hWndParent, int controlId) {
    std::array<WCHAR, 50> buffer;
    GetWindowText(GetDlgItem(hWndParent, controlId), buffer.data(), buffer.size() / sizeof(WCHAR));
    return std::stof(buffer.data());
}
