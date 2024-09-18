#include "controls.h"
#include "resource.h"
#include <windows.h>
#include <windowsx.h>

void AddMenus(HWND hWnd) {
	// Load the menu from the resource file
	HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
	WCHAR s[128];
	WCHAR label[128];

	if (hMenu) {
		// Set the menu to the window
		SetMenu(hWnd, hMenu);
	}
	else {
		::LoadString(0, IDS_MENU_ERROR_LABEL, s, sizeof s);
		::LoadString(0, IDS_ADD_BEAM_ERROR_SHORT_TEXT, label, sizeof label);
		::MessageBox(hWnd, s, label, MB_OK | MB_ICONERROR);
	}
}




void AddControls(HWND hWnd) {
	
	WCHAR s[128];

	//BEAM

	//Load and create the "Beam" static label
	::LoadString(0, IDS_BEAM_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 25, 100, 25, hWnd, NULL, NULL, NULL);

	// Load and create the "Length =" static label
	::LoadString(0, IDS_LENGTH_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 75, 100, 25, hWnd, NULL, NULL, NULL);

	// Create the "edit" control for beam length
	::CreateWindow(L"edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 70, 75, 25, hWnd, (HMENU)IDC_EDIT_BEAM, NULL, NULL);
	
	// Load and create the "m" static label
	::LoadString(0, IDS_METER_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 210, 75, 40, 25, hWnd, NULL, NULL, NULL);

	// Load and create the "Set" button
	::LoadString(0, IDS_SET_BUTTON, s, sizeof s);
	::CreateWindow(L"button", s, WS_VISIBLE | WS_CHILD, 260, 70, 80, 25, hWnd, (HMENU)ID_ADD_BEAM, NULL, NULL);
	


	//LOADS


	::LoadString(0, IDS_LOAD_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 130, 100, 25, hWnd, NULL, NULL, NULL);
	::CreateWindow(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_ETCHEDHORZ, 15, 155, 350, 25, hWnd, NULL, NULL, NULL);

	//Force
	// Load and create the "Length =" static label
	::LoadString(0, IDS_VERTICAL_FORCE_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 180, 100, 25, hWnd, NULL, NULL, NULL);
	::LoadString(0, IDS_MAGNITUDE_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 205, 100, 25, hWnd, NULL, NULL, NULL);
	::CreateWindow(L"edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 200, 75, 25, hWnd, (HMENU)IDC_EDIT_FORCE_Y, NULL, NULL);
	::LoadString(0, IDS_FORCE_UNIT_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 210, 205, 40, 25, hWnd, NULL, NULL, NULL);
	::LoadString(0, IDS_FORCE_POSITION, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 235, 100, 25, hWnd, NULL, NULL, NULL);
	::CreateWindow(L"edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 230, 75, 25, hWnd, (HMENU)IDC_EDIT_FORCE_Y_POSITION, NULL, NULL);
	::LoadString(0, IDS_METER_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 210, 235, 40, 25, hWnd, NULL, NULL, NULL);
	::LoadString(0, IDS_SET_BUTTON, s, sizeof s);
	::CreateWindow(L"button", s, WS_VISIBLE | WS_CHILD, 260, 215, 80, 25, hWnd, (HMENU)ID_ADD_FORCE_Y, NULL, NULL);

	::LoadString(0, IDS_HORIZONTAL_FORCE_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 265, 125, 25, hWnd, NULL, NULL, NULL);
	::LoadString(0, IDS_MAGNITUDE_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 290, 100, 25, hWnd, NULL, NULL, NULL);
	::CreateWindow(L"edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 285, 75, 25, hWnd, (HMENU)IDC_EDIT_FORCE_X, NULL, NULL);
	::LoadString(0, IDS_FORCE_UNIT_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 210, 290, 40, 25, hWnd, NULL, NULL, NULL);
	::LoadString(0, IDS_FORCE_POSITION, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 320, 100, 25, hWnd, NULL, NULL, NULL);
	::CreateWindow(L"edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 315, 75, 25, hWnd, (HMENU)IDC_EDIT_FORCE_X_POSITION, NULL, NULL);
	::LoadString(0, IDS_METER_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 210, 320, 40, 25, hWnd, NULL, NULL, NULL);
	::LoadString(0, IDS_SET_BUTTON, s, sizeof s);
	::CreateWindow(L"button", s, WS_VISIBLE | WS_CHILD, 260, 300, 80, 25, hWnd, (HMENU)ID_ADD_FORCE_X, NULL, NULL);



	//Moment
	::LoadString(0, IDS_MOMENT_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 350, 125, 25, hWnd, NULL, NULL, NULL);
	::LoadString(0, IDS_MAGNITUDE_LABEL, s, sizeof s); 
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 375, 100, 25, hWnd, NULL, NULL, NULL);
	::CreateWindow(L"edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 370, 75, 25, hWnd, (HMENU)IDC_EDIT_MOMENT, NULL, NULL);
	::LoadString(0, IDS_MOMENT_UNIT_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 210, 375, 40, 25, hWnd, NULL, NULL, NULL);
	::LoadString(0, IDS_FORCE_POSITION, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 405, 100, 25, hWnd, NULL, NULL, NULL);
	::CreateWindow(L"edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 400, 75, 25, hWnd, (HMENU)IDC_EDIT_MOMENT_POSITION, NULL, NULL);
	::LoadString(0, IDS_METER_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 210, 405, 40, 25, hWnd, NULL, NULL, NULL);
	::LoadString(0, IDS_SET_BUTTON, s, sizeof s);
	::CreateWindow(L"button", s, WS_VISIBLE | WS_CHILD, 260, 385, 80, 25, hWnd, (HMENU)ID_ADD_MOMENT, NULL, NULL);





	//Uniform load
	::LoadString(0, IDS_UNIFORM_LOAD_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 435, 125, 25, hWnd, NULL, NULL, NULL);
	::LoadString(0, IDS_MAGNITUDE_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 460, 100, 25, hWnd, NULL, NULL, NULL);
	::CreateWindow(L"edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 455, 75, 25, hWnd, (HMENU)IDC_EDIT_UNIFORM_LOAD, NULL, NULL);
	::LoadString(0, IDS_UNIFORM_LOAD_UNIT_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 210, 460, 40, 25, hWnd, NULL, NULL, NULL);
	::LoadString(0, IDS_UNIFORM_LOAD_POSITION_X1, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 490, 100, 25, hWnd, NULL, NULL, NULL);
	::CreateWindow(L"edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 485, 75, 25, hWnd, (HMENU)IDC_EDIT_UNIFORM_LOAD_X1_POSITION, NULL, NULL);
	::LoadString(0, IDS_METER_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 210, 490, 40, 25, hWnd, NULL, NULL, NULL);
	::LoadString(0, IDS_UNIFORM_LOAD_POSITION_X2, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 520, 100, 25, hWnd, NULL, NULL, NULL);
	::CreateWindow(L"edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 515, 75, 25, hWnd, (HMENU)IDC_EDIT_UNIFORM_LOAD_X2_POSITION, NULL, NULL);
	::LoadString(0, IDS_METER_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 210, 520, 40, 25, hWnd, NULL, NULL, NULL);
	::LoadString(0, IDS_SET_BUTTON, s, sizeof s);
	::CreateWindow(L"button", s, WS_VISIBLE | WS_CHILD, 260, 485, 80, 25, hWnd, (HMENU)ID_ADD_UNIFORM_LOAD, NULL, NULL);



	//Calculate button and results
	::LoadString(0, IDS_CALCULATE_BUTTON, s, sizeof s);
	::CreateWindow(L"button", s, WS_VISIBLE | WS_CHILD, 25, 550, 80, 25, hWnd, (HMENU)ID_CALCULATE, NULL, NULL);
	
	::LoadString(0, IDS_RESULTS_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 605, 100, 25, hWnd, NULL, NULL, NULL);
	::CreateWindow(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_ETCHEDHORZ, 15, 600, 350, 25, hWnd, NULL, NULL, NULL);

	::LoadString(0, IDS_RESULT_VERTICAL_SUPPORT_A, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 625, 100, 25, hWnd, NULL, NULL, NULL);
	::CreateWindow(L"static", L"", WS_VISIBLE | WS_CHILD, 150, 625, 50, 25, hWnd, (HMENU)IDC_RESULT_A_FX, NULL, NULL);
	::LoadString(0, IDS_FORCE_UNIT_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 210, 625, 35, 25, hWnd, NULL, NULL, NULL);
	::LoadString(0, IDS_RESULT_HORIZONTAL_SUPPORT_A, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 650, 100, 25, hWnd, NULL, NULL, NULL);
	::CreateWindow(L"static", L"", WS_VISIBLE | WS_CHILD, 150, 650, 50, 25, hWnd, (HMENU)IDC_RESULT_A_FY, NULL, NULL);
	::LoadString(0, IDS_FORCE_UNIT_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 210, 650, 35, 25, hWnd, NULL, NULL, NULL);
	::LoadString(0, IDS_RESULT_VERTICAL_SUPPORT_B, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 675, 100, 25, hWnd, NULL, NULL, NULL);
	::CreateWindow(L"static", L"", WS_VISIBLE | WS_CHILD, 150, 675, 50, 25, hWnd, (HMENU)IDC_RESULT_B_FY, NULL, NULL);
	::LoadString(0, IDS_FORCE_UNIT_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 210, 675, 35, 25, hWnd, NULL, NULL, NULL);
	::LoadString(0, IDS_MAX_MOMENT_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 25, 700, 100, 25, hWnd, NULL, NULL, NULL);
	::CreateWindow(L"static", L"", WS_VISIBLE | WS_CHILD, 150, 700, 50, 25, hWnd, (HMENU)IDC_RESULT_MAX_MOMENT, NULL, NULL);
	::LoadString(0, IDS_MOMENT_UNIT_LABEL, s, sizeof s);
	::CreateWindow(L"static", s, WS_VISIBLE | WS_CHILD, 210, 700, 35, 25, hWnd, NULL, NULL, NULL);

}
