#include <string>
#include <stdexcept>
#include <windows.h>

// Function to load string from resource and return it as std::wstring
std::wstring loadString(HINSTANCE hInstance, UINT id);

// Function to get a float value from a child window (like an edit control)
float get_child_float(HWND hWndParent, int controlId);