#include "resource.h"
#include "statika.h"
#include "controls.h"
#include "drawings.h"
#include <windows.h>
#include<windowsx.h>
#include <vector>
#include <format>

using namespace statika;


LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK Design(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK GraphForInternalMoments(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

HWND hChildWnd;
HWND hChildWnd2;

void AddMenus(HWND hWnd);

void AddControls(HWND hWnd);

struct Data {

	//Crete beam
	Beam beam;

	//Create force
	Force force;

	//Create uniform load
	UniformLoad uniformLoad;

	//Create moment
	Moment moment;

	StaticEquilibrium eq;

};


const wchar_t* const MAIN_WINDOW_CLASS = L"Simply supported beam";
const wchar_t* const DESIGN_WINDOW_CLASS = L"Design";
const wchar_t* const MOMENTS_GRAPH_WINDOW_CLASS = L"Internal moments graph";



//Main function
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	//Define main window
	const WNDCLASS wc{
	.lpfnWndProc = WindowProcedure,
	.hInstance = hInst,
	.hCursor = LoadCursor(nullptr, IDC_ARROW),
	.hbrBackground = (HBRUSH)COLOR_WINDOW,
	.lpszClassName = MAIN_WINDOW_CLASS,
	};

	if(!RegisterClass(&wc))
		return -1;
	
	//Define child window
	WNDCLASS childClass{
	.lpfnWndProc = Design,
	.hInstance = hInst,
	.hCursor = LoadCursor(nullptr, IDC_ARROW),
	.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH),
	.lpszClassName = DESIGN_WINDOW_CLASS,
	};

	if (!RegisterClass(&childClass))
		return -1;

	//Define child window 2
	WNDCLASS childClass2{
		.lpfnWndProc = GraphForInternalMoments,
		.hInstance = hInst,
		.hCursor = LoadCursor(nullptr, IDC_ARROW),
		.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH),
		.lpszClassName = MOMENTS_GRAPH_WINDOW_CLASS,
	};

	if (!RegisterClass(&childClass2))
		return -1;

	Data data;

	//Cretae main window
	HWND hMainWnd=CreateWindow(MAIN_WINDOW_CLASS, L"Simply supported beam", WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL, 100, 100, 1000, 950, 0, 0, hInst, &data);
	


	//Message loop
	MSG msg = {};
	
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	};

	return 0;
}

//Handdle main window messages
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	static int xPos = 0, yPos = 0;  // Position of the scrollbar
	SCROLLINFO si = { 0 };
	si.cbSize = sizeof(si);
	
	Data* data;
	if (msg == WM_CREATE) {
		// Extract AppData pointer from CREATESTRUCT
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lp);
		data = reinterpret_cast<Data*>(pCreate->lpCreateParams);

		// Store the pointer in window's user data
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));

		// Create child windows using CreateWindow
		HWND hChildWnd = CreateWindow(DESIGN_WINDOW_CLASS, NULL, WS_VISIBLE | WS_CHILD,
			450, 25, 500, 400, hWnd, (HMENU)IDC_CHILD1,
			((LPCREATESTRUCT)lp)->hInstance, data);

		HWND hChildWnd2 = CreateWindow(MOMENTS_GRAPH_WINDOW_CLASS, NULL, WS_VISIBLE | WS_CHILD,
			450, 450, 500, 400, hWnd, (HMENU)IDC_CHILD2,
			((LPCREATESTRUCT)lp)->hInstance, data);

		// Handle other initialization (AddMenus, AddControls) here...
		AddMenus(hWnd);
		AddControls(hWnd);
	}
	else {
		// Retrieve AppData pointer from user data
		data = reinterpret_cast<Data*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	switch (msg)
	{

		case WM_SIZE: 
		{
			RECT clientRect;
			GetClientRect(hWnd, &clientRect);

			// Set vertical scroll bar range and page size
			si.fMask = SIF_RANGE | SIF_PAGE;
			si.nMin = 0;
			si.nMax = 900;  // Set max vertical range based on content size
			si.nPage = clientRect.bottom;  // Visible window height
			SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

			// Set horizontal scroll bar range and page size
			si.nMax = 1000;  // Set max horizontal range based on content size
			si.nPage = clientRect.right;  // Visible window width
			SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
		}
					break;

		case WM_VSCROLL:
			// Handle vertical scroll
			si.fMask = SIF_ALL;
			GetScrollInfo(hWnd, SB_VERT, &si);
			yPos = si.nPos;

			switch (LOWORD(wp)) 
			{
				case SB_LINEUP: si.nPos -= 10; break;
				case SB_LINEDOWN: si.nPos += 10; break;
				case SB_PAGEUP: si.nPos -= si.nPage; break;
				case SB_PAGEDOWN: si.nPos += si.nPage; break;
				case SB_THUMBTRACK: si.nPos = si.nTrackPos; break;
			}

			SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
			ScrollWindow(hWnd, 0, yPos - si.nPos, NULL, NULL);
			UpdateWindow(hWnd);
			break;

		case WM_HSCROLL:
			// Handle horizontal scroll
			si.fMask = SIF_ALL;
			GetScrollInfo(hWnd, SB_HORZ, &si);
			xPos = si.nPos;

			switch (LOWORD(wp)) 
			{
				case SB_LINELEFT: si.nPos -= 10; break;
				case SB_LINERIGHT: si.nPos += 10; break;
				case SB_PAGELEFT: si.nPos -= si.nPage; break;
				case SB_PAGERIGHT: si.nPos += si.nPage; break;
				case SB_THUMBTRACK: si.nPos = si.nTrackPos; break;
			}

			SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
			ScrollWindow(hWnd, xPos - si.nPos, 0, NULL, NULL);
			UpdateWindow(hWnd);
			break;
		case WM_COMMAND:
		{
			HWND hChild1 = GetDlgItem(hWnd, IDC_CHILD1);
			HWND hChild2 = GetDlgItem(hWnd, IDC_CHILD2);
			WCHAR s[128];
			switch (wp)
			{
			case ID_FILE_EXIT:
				DestroyWindow(hWnd);
				break;
			case ID_HELP_ABOUT:
				::LoadString(0, IDS_HELP_ABOUT_TEXT, s, sizeof s);
				::MessageBox(hWnd, s, L"About", MB_OK | MB_ICONINFORMATION);
				break;
			case ID_ADD_BEAM:
			{
				wchar_t buffer[10];
				GetWindowText(GetDlgItem(hWnd, IDC_EDIT_BEAM), buffer, 10);
				data->beam.L = _wtof(buffer);
				if (data->beam.L <= 0)
				{
					::LoadString(0, IDS_ADD_BEAM_TEXT, s, sizeof s);
					::MessageBox(hWnd, s, L"Error", MB_OK | MB_ICONERROR);
				}


				InvalidateRect(hChildWnd, NULL, TRUE);

			}
			break;
			case ID_ADD_FORCE_Y:
			{
				wchar_t buffer[10];
				GetWindowText(GetDlgItem(hWnd, IDC_EDIT_FORCE_Y), buffer, 10);
				data->force.Fy = _wtof(buffer);
				GetWindowText(GetDlgItem(hWnd, IDC_EDIT_FORCE_Y_POSITION), buffer, 10);
				data->force.Fy_x = _wtof(buffer);
				if (data->force.Fy_x > data->beam.L || data->force.Fy_x < 0)
				{
					::LoadString(0, IDS_ADD_FORCE_Y_TEXT, s, sizeof s);
					MessageBox(hWnd, s, L"Error", MB_OK | MB_ICONERROR);
				}

				// Trigger repaint of the child window to draw the force
				InvalidateRect(hChildWnd, NULL, TRUE);


			}
			break;
			case ID_ADD_FORCE_X:
			{
				wchar_t buffer[10];
				GetWindowText(GetDlgItem(hWnd, IDC_EDIT_FORCE_X), buffer, 10);
				data->force.Fx = _wtof(buffer);
				GetWindowText(GetDlgItem(hWnd, IDC_EDIT_FORCE_X_POSITION), buffer, 10);
				data->force.Fx_x = _wtof(buffer);
				if (data->force.Fx_x > data->beam.L || data->force.Fx_x < 0)
				{
					::LoadString(0, IDS_ADD_FORCE_X_TEXT, s, sizeof s);
					::MessageBox(hWnd, s, L"Error", MB_OK | MB_ICONERROR);
				}

				// Trigger repaint of the child window to draw the force
				InvalidateRect(hChildWnd, NULL, TRUE);


			}
			break;
			case ID_ADD_MOMENT:
			{
				wchar_t buffer[10];
				GetWindowText(GetDlgItem(hWnd, IDC_EDIT_MOMENT), buffer, 10);
				data->moment.M = _wtof(buffer);
				GetWindowText(GetDlgItem(hWnd, IDC_EDIT_MOMENT_POSITION), buffer, 10);
				data->moment.x = _wtof(buffer);
				if (data->moment.x > data->beam.L || data->moment.x < 0)
				{
					::LoadString(0, IDS_ADD_MOMENT_TEXT, s, sizeof s);
					::MessageBox(hWnd, s, L"Error", MB_OK | MB_ICONERROR);
				}

				// Trigger repaint of the child window to draw the moment
				InvalidateRect(hChildWnd, NULL, TRUE);

			}
			InvalidateRect(hChild1, NULL, TRUE);
			break;
			case ID_ADD_UNIFORM_LOAD:
			{
				wchar_t buffer[50];

				GetWindowText(GetDlgItem(hWnd, IDC_EDIT_UNIFORM_LOAD), buffer, 50);
				data->uniformLoad.q = _wtof(buffer);

				GetWindowText(GetDlgItem(hWnd, IDC_EDIT_UNIFORM_LOAD_X1_POSITION), buffer, 50);
				data->uniformLoad.x1 = _wtof(buffer);

				GetWindowText(GetDlgItem(hWnd, IDC_EDIT_UNIFORM_LOAD_X2_POSITION), buffer, 50);
				data->uniformLoad.x2 = _wtof(buffer);

				if (data->uniformLoad.x2 < data->uniformLoad.x1)
				{
					::LoadString(0, IDS_ADD_UNIFORM_LOAD_X1_X2_POSITION_TEXT, s, sizeof s);
					::MessageBox(hWnd, s, L"Error", MB_OK | MB_ICONERROR);
				}
				if (data->uniformLoad.x1 > data->beam.L)
				{
					::LoadString(0, IDS_ADD_UNIFORM_LOAD_X1_POSITION_TEXT, s, sizeof s);
					::MessageBox(hWnd, s, L"Error", MB_OK | MB_ICONERROR);
				}
				if (data->uniformLoad.x1 < 0)
				{
					::LoadString(0, IDS_ADD_UNIFORM_LOAD_X1_NEGATIVE_TEXT, s, sizeof s);
					::MessageBox(hWnd, s, L"Error", MB_OK | MB_ICONERROR);
				}
				if (data->uniformLoad.x2 > data->beam.L)
				{
					::LoadString(0, IDS_ADD_UNIFORM_LOAD_X2_POSTION_TEXT, s, sizeof s);
					::MessageBox(hWnd, s, L"Error", MB_OK | MB_ICONERROR);
				}
				if (data->uniformLoad.x2 < 0)
				{
					::LoadString(0, IDS_ADD_UNIFORM_LOAD_X2_NEGATIVE_TEXT, s, sizeof s);
					::MessageBox(hWnd, s, L"Error", MB_OK | MB_ICONERROR);
				}

				// Trigger repaint of the child window to draw uniform load
				InvalidateRect(hChildWnd, NULL, TRUE);

			}

			break;
			case ID_CALCULATE:
			{
				std::wstring FAx = std::format(L"{:.2f}", data->eq.reactionOnSupportAx(data->force));

				std::wstring FAy = std::format(L"{:.2f}", data->eq.reactionOnSupportAy(data->beam, data->force, data->moment, data->uniformLoad));

				std::wstring FBy = std::format(L"{:.2f}", data->eq.reactionOnSupportBy(data->beam, data->force, data->moment, data->uniformLoad));

				data->eq.calcInternalMoment(data->beam, data->moment, data->force, data->uniformLoad);
				const auto& internalMoments = data->eq.getInternalMoments();
				double maxMoment = data->eq.getMaxMomentValue(internalMoments);
				std::wstring maxMoment_s = std::format(L"{:.2f}", maxMoment);

				SetWindowText(GetDlgItem(hWnd, IDC_RESULT_A_FX), FAx.c_str());
				SetWindowText(GetDlgItem(hWnd, IDC_RESULT_A_FY), FAy.c_str());
				SetWindowText(GetDlgItem(hWnd, IDC_RESULT_B_FY), FBy.c_str());
				SetWindowText(GetDlgItem(hWnd, IDC_RESULT_MAX_MOMENT), maxMoment_s.c_str());


				// Trigger repaint of the child window to draw the moment
				InvalidateRect(hChildWnd2, NULL, TRUE);
			}
			break;
			}
		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, msg, wp, lp);
	}

	return 0;
}

//Handle child window messages
LRESULT CALLBACK Design(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	Data* data = reinterpret_cast<Data*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));  // Retrieve Data pointer

	switch (msg)
	{
	case WM_CREATE:
	{
		// Set the Data pointer to the window's user data
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lp);
		data = reinterpret_cast<Data*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);

		// Now you can access the beam, force, uniformLoad, moment from `data`
		DrawBeamAndLoads(hdc, clientRect, data->beam, data->force, data->uniformLoad, data->moment);

		EndPaint(hWnd, &ps);
	}
	break;

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}

//Handle child 2 window messages
LRESULT CALLBACK GraphForInternalMoments(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	Data* data = reinterpret_cast<Data*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));  // Retrieve Data pointer

	switch (msg)
	{
	case WM_CREATE:
	{
		// Set the Data pointer to the window's user data
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lp);
		data = reinterpret_cast<Data*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);

		// Now you can access the beam and eq from `data`
		DrawInternalMoments(hdc, clientRect, data->beam, data->eq);

		EndPaint(hWnd, &ps);
	}
	break;

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}

