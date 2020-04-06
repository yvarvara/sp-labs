#include "stdafx.h"
#include "Lab6.h"
#include <stdlib.h>
#include <ctime>

constexpr auto MAX_LOADSTRING = 100;

// global variables
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// functions' declarations
ATOM				MyRegisterClass(HINSTANCE hInstance); 
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,		// application instance handler
                     _In_opt_ HINSTANCE hPrevInstance,	// prev application instance handler 
                     _In_ LPTSTR    lpCmdLine,			// command line pointer
                     _In_ int       nCmdShow)			// showWindow (min/max)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LAB6, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// application initialization
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	srand(time(NULL));

	MSG msg;
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB6));

	// message loop
	while (GetMessage(&msg, NULL, 0, 0))		// all messages from all windows
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);				// call message proccessing method (CALLBACK WndProc())
		}
	}

	return (int) msg.wParam;
}

//  register the window class.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;			// proccessing method's address
	wcex.cbClsExtra	= 0;
	wcex.cbWndExtra	= 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB6));
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_POINTER));
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_LAB6);
	wcex.lpszClassName = szWindowClass;
	wcex.hbrBackground = (HBRUSH)(HBRUSH)(CreateSolidBrush(RGB(255, 192, 203)));

	return RegisterClassEx(&wcex);
}


//  save instance handle and creates main window
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   hInst = hInstance; // save instance to global variable

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

TCHAR text[] = L"MAGIC WAND";

#define TIMER_ID 1
#define TIMER_DELAY 20

int x = 10, width, height;
bool animStarted = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		case ID_ANIMATION_START:
			if (!animStarted) {
				SetTimer(hWnd, TIMER_ID, TIMER_DELAY, NULL);
				animStarted = true;
			}
			break;

		case ID_ANIMATION_STOP:
			if (animStarted) {
				KillTimer(hWnd, TIMER_ID);
				animStarted = false;
			}
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);	
		}
		break;

	case WM_SIZE:
		width = LOWORD(lParam);
		height = HIWORD(lParam);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOutW(hdc, x, height / 2, (LPCWSTR)text, lstrlen(text));
		EndPaint(hWnd, &ps);
		break;

	case WM_TIMER:
		x = (x + 10) % width;
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box
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