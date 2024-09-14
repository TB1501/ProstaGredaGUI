#include "res.h"
#include "statika.h"
#include "controls.h"
#include "drawings.h"
#include <windows.h>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace statika;


LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK ChildWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK ChildWindowProcedure2(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

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

//Format double to two decimals
std::wstring FormatDoubleToTwoDecimals(double value) {
	std::wstringstream stream;
	stream << std::fixed << std::setprecision(2) << value;
	return stream.str();
}


//Main function
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	//Define main window
	WNDCLASSW wc{0};
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = L"Simply supported beam";
	wc.hInstance = hInst;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

	if(!RegisterClassW(&wc))
		return -1;
	
	//Define child window
	WNDCLASSW childClass = { 0 };
	childClass.lpfnWndProc = ChildWindowProcedure;
	childClass.hInstance = hInst;
	childClass.lpszClassName = L"ChildWindow";
	childClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	childClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	if (!RegisterClassW(&childClass))
		return -1;

	//Define child window 2
	WNDCLASSW childClass2 = { 0 };
	childClass2.lpfnWndProc = ChildWindowProcedure2;
	childClass2.hInstance = hInst;
	childClass2.lpszClassName = L"ChildWindow2";
	childClass2.hCursor = LoadCursor(nullptr, IDC_ARROW);
	childClass2.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	if (!RegisterClassW(&childClass2))
		return -1;

	//Cretae main window
	HWND hMainWnd=CreateWindowW(L"Simply supported beam", L"Simply supported beam", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1000, 950, 0, 0, 0, 0);
	
	//Create child window
	hChildWnd = CreateWindowW(L"ChildWindow", NULL, WS_VISIBLE | WS_CHILD, 450, 25, 500, 400, hMainWnd, NULL, hInst, NULL);

	//Create child window 2
	hChildWnd2 = CreateWindowW(L"ChildWindow2", NULL, WS_VISIBLE | WS_CHILD, 450, 450, 500, 400, hMainWnd, NULL, hInst, NULL);

	//Message loop
	MSG msg = {};
	
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	};
}

//Handdle main window messages
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
		AddMenus(hWnd);
		AddControls(hWnd);
		break;
	case WM_COMMAND:
		switch (wp)
		{
		case IDC_FILE_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDC_HELP_ABOUT:
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
			double FAx = eq.reactionOnSupportAx(force);
			double FAy = eq.reactionOnSupportAy(beam, force, moment, uniformLoad);
			double FBy = eq.reactionOnSupportBy(beam, force, moment, uniformLoad);
			eq.calcInternalMoment(beam, moment, force, uniformLoad);
			const auto& internalMoments = eq.getInternalMoments();
			SetWindowTextW(GetDlgItem(hWnd, IDC_RESULT_A_FX), FormatDoubleToTwoDecimals(FAx).c_str());
			SetWindowTextW(GetDlgItem(hWnd, IDC_RESULT_A_FY), FormatDoubleToTwoDecimals(FAy).c_str());
			SetWindowTextW(GetDlgItem(hWnd, IDC_RESULT_B_FY), FormatDoubleToTwoDecimals(FBy).c_str());
			SetWindowTextW(GetDlgItem(hWnd, IDC_RESULT_MAX_MOMENT), FormatDoubleToTwoDecimals(eq.getMaxMomentValue(internalMoments)).c_str());


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
LRESULT CALLBACK ChildWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
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
	
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProcW(hWnd, msg, wp, lp);
		}
		return 0;
	}

//Handle child 2 window messages
LRESULT CALLBACK ChildWindowProcedure2(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) 
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

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		default:
		{
			return DefWindowProcW(hWnd, msg, wp, lp);
		}
	}
	return 0;


}

