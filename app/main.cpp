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

//Crete beam
Beam beam;

//Create force
Force force;

//Create uniform load
UniformLoad uniformLoad;

//Create moment
Moment moment;

StaticEquilibrium eq;

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

	//Cretae main window
	HWND hMainWnd=CreateWindow(MAIN_WINDOW_CLASS, L"Simply supported beam", WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL, 100, 100, 1000, 950, 0, 0, 0, 0);
	
	//Create child window
	hChildWnd = CreateWindow(DESIGN_WINDOW_CLASS, NULL, WS_VISIBLE | WS_CHILD, 450, 25, 500, 400, hMainWnd, NULL, hInst, NULL);

	//Create child window 2
	hChildWnd2 = CreateWindow(MOMENTS_GRAPH_WINDOW_CLASS, NULL, WS_VISIBLE | WS_CHILD, 450, 450, 500, 400, hMainWnd, NULL, hInst, NULL);


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

	switch (msg)
	{
		case WM_CREATE:
			AddMenus(hWnd);
			AddControls(hWnd);
			break;

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
			switch (wp)
			{
				case ID_FILE_EXIT:
					DestroyWindow(hWnd);
					break;
				case ID_HELP_ABOUT:
					MessageBoxW(hWnd, L"This is a program that calculates the reactions on supports and internal forces for simply supported beams ", L"About", MB_OK | MB_ICONINFORMATION);
					break;
				case ID_ADD_BEAM:
				{
						wchar_t buffer[10];
						GetWindowTextW(GetDlgItem(hWnd, IDC_EDIT_BEAM), buffer, 10);
						beam.L = _wtof(buffer);
						if (beam.L <= 0)
						{
							MessageBoxW(hWnd, L"Beam length cannot be zero or negative!", L"Error", MB_OK | MB_ICONERROR);
						}


						InvalidateRect(hChildWnd, NULL, TRUE);

				}
					break;
				case ID_ADD_FORCE_Y:
				{
						wchar_t buffer[10];
						GetWindowTextW(GetDlgItem(hWnd, IDC_EDIT_FORCE_Y), buffer, 10);
						force.Fy = _wtof(buffer);
						GetWindowTextW(GetDlgItem(hWnd, IDC_EDIT_FORCE_Y_POSITION), buffer, 10);
						force.Fy_x = _wtof(buffer);
						if (force.Fy_x > beam.L || force.Fy_x < 0)
						{
							MessageBoxW(hWnd, L"Position of force or moment cannot be outside the beam length!", L"Error", MB_OK | MB_ICONERROR);
						}

						// Trigger repaint of the child window to draw the force
						InvalidateRect(hChildWnd, NULL, TRUE);


				}
					break;
				case ID_ADD_FORCE_X:
				{
						wchar_t buffer[10];
						GetWindowTextW(GetDlgItem(hWnd, IDC_EDIT_FORCE_X), buffer, 10);
						force.Fx = _wtof(buffer);
						GetWindowTextW(GetDlgItem(hWnd, IDC_EDIT_FORCE_X_POSITION), buffer, 10);
						force.Fx_x = _wtof(buffer);
						if (force.Fx_x > beam.L || force.Fx_x < 0)
						{
							MessageBoxW(hWnd, L"Position of force cannot be outside the beam length or negative!", L"Error", MB_OK | MB_ICONERROR);
						}

						// Trigger repaint of the child window to draw the force
						InvalidateRect(hChildWnd, NULL, TRUE);


				}
					break;
				case ID_ADD_MOMENT:
				{
						wchar_t buffer[10];
						GetWindowTextW(GetDlgItem(hWnd, IDC_EDIT_MOMENT), buffer, 10);
						moment.M = _wtof(buffer);
						GetWindowTextW(GetDlgItem(hWnd, IDC_EDIT_MOMENT_POSITION), buffer, 10);
						moment.x = _wtof(buffer);
						if (moment.x > beam.L || moment.x < 0)
						{
							MessageBoxW(hWnd, L"Position of moment cannot be outside the beam length or negative!", L"Error", MB_OK | MB_ICONERROR);
						}

						// Trigger repaint of the child window to draw the moment
						InvalidateRect(hChildWnd, NULL, TRUE);

				}
					break;
				case ID_ADD_UNIFORM_LOAD:
				{
					wchar_t buffer[50];  

					GetWindowTextW(GetDlgItem(hWnd, IDC_EDIT_UNIFORM_LOAD), buffer, 50);
					uniformLoad.q = _wtof(buffer);

					GetWindowTextW(GetDlgItem(hWnd, IDC_EDIT_UNIFORM_LOAD_X1_POSITION), buffer, 50);
					uniformLoad.x1 = _wtof(buffer);

					GetWindowTextW(GetDlgItem(hWnd, IDC_EDIT_UNIFORM_LOAD_X2_POSITION), buffer, 50);
					uniformLoad.x2 = _wtof(buffer);

					if (uniformLoad.x2 < uniformLoad.x1)
					{
						MessageBoxW(hWnd, L"Second position of uniform load cannot be smaller than the first position!", L"Error", MB_OK | MB_ICONERROR);
					}
					if (uniformLoad.x1 > beam.L)
					{
						MessageBoxW(hWnd, L"First position of uniform load cannot be outside the beam length!", L"Error", MB_OK | MB_ICONERROR);
					}
					if (uniformLoad.x1 < 0)
					{
						MessageBoxW(hWnd, L"First position of uniform load cannot be negative!", L"Error", MB_OK | MB_ICONERROR);
					}
					if (uniformLoad.x2 > beam.L)
					{
						MessageBoxW(hWnd, L"Second position of uniform load cannot be outside the beam length!", L"Error", MB_OK | MB_ICONERROR);
					}
					if (uniformLoad.x2 < 0)
					{
						MessageBoxW(hWnd, L"Second position of uniform load cannot be negative!", L"Error", MB_OK | MB_ICONERROR);
					}

					// Trigger repaint of the child window to draw uniform load
					InvalidateRect(hChildWnd, NULL, TRUE);

				}

					break;
				case ID_CALCULATE:
				{
					std::string FAx = std::format("{:.2f}",eq.reactionOnSupportAx(force));
					std::wstring FAx_w(FAx.begin(), FAx.end());
					std::string FAy = std::format("{:.2f}",eq.reactionOnSupportAy(beam, force, moment, uniformLoad));
					std::wstring FAy_w(FAy.begin(), FAy.end());
					std::string FBy = std::format("{:.2f}",eq.reactionOnSupportBy(beam, force, moment, uniformLoad));
					std::wstring FBy_w(FBy.begin(), FBy.end());
					eq.calcInternalMoment(beam, moment, force, uniformLoad);
					const auto& internalMoments= eq.getInternalMoments();
					double maxMoment = eq.getMaxMomentValue(internalMoments);
					std::string maxMoment_s= std::format("{:.2f}", maxMoment);
					std::wstring maxMoment_w(maxMoment_s.begin(), maxMoment_s.end());
					SetWindowText(GetDlgItem(hWnd, IDC_RESULT_A_FX), FAx_w.c_str());
					SetWindowText(GetDlgItem(hWnd, IDC_RESULT_A_FY), FAy_w.c_str());
					SetWindowText(GetDlgItem(hWnd, IDC_RESULT_B_FY), FBy_w.c_str());
					SetWindowText(GetDlgItem(hWnd, IDC_RESULT_MAX_MOMENT), maxMoment_w.c_str());


					// Trigger repaint of the child window to draw the moment
					InvalidateRect(hChildWnd2, NULL, TRUE);
				}
				break;
			}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProcW(hWnd, msg, wp, lp);
	}

	return 0;
}

//Handle child window messages
LRESULT CALLBACK Design(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	
	switch (msg)
	{
		case WM_PAINT:
		{
			
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			RECT clientRect;
			GetClientRect(hWnd, &clientRect);
			DrawBeamAndLoads(hdc, clientRect, beam, force, uniformLoad, moment);
			EndPaint(hWnd, &ps);
		}

		break;
	
		default:
			return DefWindowProcW(hWnd, msg, wp, lp);
		}
		return 0;
	}

//Handle child 2 window messages
LRESULT CALLBACK GraphForInternalMoments(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			RECT clientRect;
			GetClientRect(hWnd, &clientRect);
			DrawInternalMoments(hdc, clientRect, beam, eq);

			EndPaint(hWnd, &ps);

			
		}
		break;
		default:
		{
			return DefWindowProcW(hWnd, msg, wp, lp);
		}
		
	}
	return 0;


}

