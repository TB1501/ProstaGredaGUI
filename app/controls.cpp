#include "controls.h"
#include "res.h"

void AddMenus(HWND hWnd) {
    HMENU hMenubar = CreateMenu();
    HMENU hExitMenu = CreateMenu();
    HMENU hHelpMenu = CreateMenu();

    AppendMenuW(hExitMenu, MF_STRING, IDC_FILE_EXIT, L"Exit");
    AppendMenuW(hHelpMenu, MF_STRING, IDC_HELP_ABOUT, L"About");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hExitMenu, L"File");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hHelpMenu, L"Help");

    SetMenu(hWnd, hMenubar);
}

void AddControls(HWND hWnd) {
	//Beam paramenters
	CreateWindowW(L"static", L"Beam: ", WS_VISIBLE | WS_CHILD, 25, 25, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_ETCHEDHORZ, 15, 50, 350, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Length = ", WS_VISIBLE | WS_CHILD, 25, 75, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 70, 75, 25, hWnd, (HMENU)IDC_EDIT_BEAM, NULL, NULL);
	CreateWindowW(L"static", L"m", WS_VISIBLE | WS_CHILD, 210, 75, 40, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"button", L"Add", WS_VISIBLE | WS_CHILD, 260, 70, 80, 25, hWnd, (HMENU)ID_ADD_BEAM, NULL, NULL);

	//Loads
	CreateWindowW(L"static", L"Loads: ", WS_VISIBLE | WS_CHILD, 25, 130, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_ETCHEDHORZ, 15, 155, 350, 25, hWnd, NULL, NULL, NULL);

	//Forces
	CreateWindowW(L"static", L"Vertical force: ", WS_VISIBLE | WS_CHILD, 25, 180, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Magnitude = ", WS_VISIBLE | WS_CHILD, 25, 205, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 200, 75, 25, hWnd, (HMENU)IDC_EDIT_FORCE_Y, NULL, NULL);
	CreateWindowW(L"static", L"kN", WS_VISIBLE | WS_CHILD, 210, 205, 40, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Position = ", WS_VISIBLE | WS_CHILD, 25, 235, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 230, 75, 25, hWnd, (HMENU)IDC_EDIT_FORCE_Y_POSITION, NULL, NULL);
	CreateWindowW(L"static", L"m", WS_VISIBLE | WS_CHILD, 210, 235, 40, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"button", L"Add", WS_VISIBLE | WS_CHILD, 260, 215, 80, 25, hWnd, (HMENU)ID_ADD_FORCE_Y, NULL, NULL);

	CreateWindowW(L"static", L"Horizontal force: ", WS_VISIBLE | WS_CHILD, 25, 265, 125, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Magnitude = ", WS_VISIBLE | WS_CHILD, 25, 290, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 285, 75, 25, hWnd, (HMENU)IDC_EDIT_FORCE_X, NULL, NULL);
	CreateWindowW(L"static", L"kN", WS_VISIBLE | WS_CHILD, 210, 290, 40, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Position = ", WS_VISIBLE | WS_CHILD, 25, 320, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 315, 75, 25, hWnd, (HMENU)IDC_EDIT_FORCE_X_POSITION, NULL, NULL);
	CreateWindowW(L"static", L"m", WS_VISIBLE | WS_CHILD, 210, 320, 40, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"button", L"Add", WS_VISIBLE | WS_CHILD, 260, 300, 80, 25, hWnd, (HMENU)ID_ADD_FORCE_X, NULL, NULL);

	//Moment
	CreateWindowW(L"static", L"Moment: ", WS_VISIBLE | WS_CHILD, 25, 350, 125, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Magnitude = ", WS_VISIBLE | WS_CHILD, 25, 375, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 370, 75, 25, hWnd, (HMENU)IDC_EDIT_MOMENT, NULL, NULL);
	CreateWindowW(L"static", L"kNm", WS_VISIBLE | WS_CHILD, 210, 375, 40, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Position = ", WS_VISIBLE | WS_CHILD, 25, 405, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 400, 75, 25, hWnd, (HMENU)IDC_EDIT_MOMENT_POSITION, NULL, NULL);
	CreateWindowW(L"static", L"m", WS_VISIBLE | WS_CHILD, 210, 405, 40, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"button", L"Add", WS_VISIBLE | WS_CHILD, 260, 385, 80, 25, hWnd, (HMENU)ID_ADD_MOMENT, NULL, NULL);

	//Uniform load
	CreateWindowW(L"static", L"Uniform Load: ", WS_VISIBLE | WS_CHILD, 25, 435, 125, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Magnitude = ", WS_VISIBLE | WS_CHILD, 25, 460, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 455, 75, 25, hWnd, (HMENU)IDC_EDIT_UNIFORM_LOAD, NULL, NULL);
	CreateWindowW(L"static", L"kNm", WS_VISIBLE | WS_CHILD, 210, 460, 40, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Position 1 = ", WS_VISIBLE | WS_CHILD, 25, 490, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 485, 75, 25, hWnd, (HMENU)IDC_EDIT_UNIFORM_LOAD_X1_POSITION, NULL, NULL);
	CreateWindowW(L"static", L"m", WS_VISIBLE | WS_CHILD, 210, 490, 40, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Position 2 = ", WS_VISIBLE | WS_CHILD, 25, 520, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 125, 515, 75, 25, hWnd, (HMENU)IDC_EDIT_UNIFORM_LOAD_X2_POSITION, NULL, NULL);
	CreateWindowW(L"static", L"m", WS_VISIBLE | WS_CHILD, 210, 520, 40, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"button", L"Add", WS_VISIBLE | WS_CHILD, 260, 485, 80, 25, hWnd, (HMENU)ID_ADD_UNIFORM_LOAD, NULL, NULL);

	//Calculate button and results
	CreateWindowW(L"button", L"Calculate", WS_VISIBLE | WS_CHILD, 25, 550, 80, 25, hWnd, (HMENU)ID_CALCULATE, NULL, NULL);

	CreateWindowW(L"static", L"Results: ", WS_VISIBLE | WS_CHILD, 25, 605, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", NULL, WS_VISIBLE | WS_CHILD | SS_ETCHEDHORZ, 15, 600, 350, 25, hWnd, NULL, NULL, NULL);

	CreateWindowW(L"static", L"Support A: Fy = ", WS_VISIBLE | WS_CHILD, 25, 625, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD, 150, 625, 50, 25, hWnd, (HMENU)IDC_RESULT_A_FX, NULL, NULL);
	CreateWindowW(L"static", L"kN", WS_VISIBLE | WS_CHILD, 210, 625, 35, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Support A: Fx = ", WS_VISIBLE | WS_CHILD, 25, 650, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD, 150, 650, 50, 25, hWnd, (HMENU)IDC_RESULT_A_FY, NULL, NULL);
	CreateWindowW(L"static", L"kN", WS_VISIBLE | WS_CHILD, 210, 650, 35, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Support B: Fy = ", WS_VISIBLE | WS_CHILD, 25, 675, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD, 150, 675, 50, 25, hWnd, (HMENU)IDC_RESULT_B_FY, NULL, NULL);
	CreateWindowW(L"static", L"kN", WS_VISIBLE | WS_CHILD, 210, 675, 35, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Mmax = ", WS_VISIBLE | WS_CHILD, 25, 700, 100, 25, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD, 150, 700, 50, 25, hWnd, (HMENU)IDC_RESULT_MAX_MOMENT, NULL, NULL);
	CreateWindowW(L"static", L"kNm", WS_VISIBLE | WS_CHILD, 210, 700, 35, 25, hWnd, NULL, NULL, NULL);

}
