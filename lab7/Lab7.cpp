#include "stdafx.h"
#include "Lab7.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
void				InitControls(HWND);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
HWND hWnd;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
 	
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LAB7, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB7));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB7));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LAB7);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 755, 420, NULL, NULL, hInstance, NULL);

   InitControls(hWnd);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

#define EDIT_ID 1
#define BUTTON_ADD_ID 2
#define BUTTON_CLEAR_ID 3
#define BUTTON_TORIGHT_ID 4
#define BUTTON_DELETE_ID 5
#define LISTBOX1_ID 6
#define LISTBOX2_ID 7

HWND hEdit, hButtonAdd, hButtonClear, hButtonToRight, hButtonDelete, hListBox1, hListBox2;

void InitControls(HWND hWnd) {
	hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"string", WS_TABSTOP | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		40, 40, 200, 20, hWnd, (HMENU)EDIT_ID, hInst, NULL);

	hButtonAdd = CreateWindow(L"BUTTON", L"Add line", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		260, 35, 80, 30, hWnd, (HMENU)BUTTON_ADD_ID, hInst, NULL);

	hButtonClear = CreateWindow(L"BUTTON", L"Clear all", WS_TABSTOP | WS_CHILD | WS_VISIBLE,
		120, 300, 80, 30, hWnd, (HMENU)BUTTON_CLEAR_ID, hInst, NULL);

	hButtonToRight = CreateWindow(L"BUTTON", L"To right", WS_TABSTOP | WS_CHILD | WS_VISIBLE,
		320, 300, 80, 30, hWnd, (HMENU)BUTTON_TORIGHT_ID, hInst, NULL);

	hButtonDelete = CreateWindow(L"BUTTON", L"Delete line", WS_TABSTOP | WS_CHILD | WS_VISIBLE,
		520, 300, 80, 30, hWnd, (HMENU)BUTTON_DELETE_ID, hInst, NULL);

	hListBox1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"LISTBOX", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | ES_AUTOHSCROLL | LBS_MULTIPLESEL,
		40, 80, 300, 200, hWnd, (HMENU)LISTBOX1_ID, hInst, NULL);

	hListBox2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"LISTBOX", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | LBS_MULTIPLESEL,
		400, 80, 300, 200, hWnd, (HMENU)LISTBOX2_ID, hInst, NULL);

	SendDlgItemMessage(hWnd, EDIT_ID, (UINT)LB_SETHORIZONTALEXTENT, (WPARAM)1024, (LPARAM)0);
}

bool ContainsString(HWND hListBox, TCHAR* s) {
	int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);
	for (int i = 0; i < count; i++) {
		TCHAR buffer[256];
		buffer[0] = '/0';
		SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)buffer);
		if (!_tcscmp(s, buffer))
			return true;
	}

	return false;
}

void UpdateListBoxScroll(HWND hListBox, int itemId = -1) {
	int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);
	if (count == 0) SendDlgItemMessage(hWnd, hListBox == hListBox1 ? LISTBOX1_ID : LISTBOX2_ID, (UINT)LB_SETHORIZONTALEXTENT, (WPARAM)0, (LPARAM)0);
	TCHAR buffer[256];
	buffer[0] = '/0';

	for (int i = 0; i < count; i++) {
		if (i == itemId) continue;
		SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)buffer);
		int SizeOfHBListBox = _tcslen(buffer) * 8;
		 
		// scrollable height
		SendDlgItemMessage(hWnd, hListBox == hListBox1 ? LISTBOX1_ID : LISTBOX2_ID, (UINT)LB_SETHORIZONTALEXTENT, (WPARAM)SizeOfHBListBox, (LPARAM)0);
	}
}

void AddStringInListBoxIfNeed(HWND hListBox, TCHAR* s) {
	if (!ContainsString(hListBox, s)) {
		SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)s);
		UpdateListBoxScroll(hListBox);
	}
}

bool DeleteSelectedItem(HWND hListBox) {
	int selCount = SendMessage(hListBox, LB_GETSELCOUNT, 0, 0);
	int countBuffer[100];
	SendMessage(hListBox, LB_GETSELITEMS, 100, (LPARAM)countBuffer);

	for (int i = selCount - 1; i >= 0; --i) {
		int itemId = countBuffer[i];
		SendMessage(hListBox, LB_DELETESTRING, itemId, 0);
	}

	UpdateListBoxScroll(hListBox);
	return true;
}

void ToRight() {
	int selCount = SendMessage(hListBox1, LB_GETSELCOUNT, 0, 0);
	int countBuffer[100];
	SendMessage(hListBox1, LB_GETSELITEMS, 100, (LPARAM)countBuffer);

	if (selCount == 0) {
		MessageBox(hWnd, L"Select item in first ListBox", L"Error", MB_OK);
	}
	else {
		for (int i = selCount - 1; i >= 0; --i) {
			int itemId = countBuffer[i];
			TCHAR buffer[256];
			SendMessage(hListBox1, LB_GETTEXT, itemId, (LPARAM)buffer);
			AddStringInListBoxIfNeed(hListBox2, buffer);
		}

		DeleteSelectedItem(hListBox1);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmEvent)
		{
			case BN_CLICKED:
				switch (wmId)
				{
					case BUTTON_ADD_ID:
						int len;
						len = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
						if (len == 0)
							MessageBox(hWnd, L"Fill the text field", L"Error", MB_OK);
						else {
							TCHAR *buffer = new TCHAR[len];
							SendMessage(hEdit, WM_GETTEXT, len + 1, (LPARAM)buffer);

							AddStringInListBoxIfNeed(hListBox1, buffer);
							SetWindowText(GetDlgItem(hWnd, EDIT_ID), L"");
						}
						break;
					case BUTTON_CLEAR_ID:
						SendMessage(hListBox1, LB_RESETCONTENT, 0, 0);
						SendMessage(hListBox2, LB_RESETCONTENT, 0, 0);
						UpdateListBoxScroll(hListBox1);
						UpdateListBoxScroll(hListBox2);
						break;
					case BUTTON_TORIGHT_ID:
						ToRight();

						break;
					case BUTTON_DELETE_ID:
						DeleteSelectedItem(hListBox1);
						DeleteSelectedItem(hListBox2);
						break;
					default:
						break;
				}
			default:
				switch (wmId)
				{
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
