#include "stdafx.h"
#include "Lab8.h"

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
	LoadString(hInstance, IDC_LAB8, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB8));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB8));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 700, 500, NULL, NULL, hInstance, NULL);

   InitControls(hWnd);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

#define BUTTON_DRAW_ID 1
#define BUTTON_CLEAR_ID 2

void InitControls(HWND hWnd) {
	HWND buttonAdd = CreateWindow(L"BUTTON", NULL, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON,
		600, 150, 50, 50, hWnd, (HMENU)BUTTON_DRAW_ID, NULL, NULL);
	HWND buttonClear = CreateWindow(L"BUTTON", NULL, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON,
		600, 250, 50, 50, hWnd, (HMENU)BUTTON_CLEAR_ID, NULL, NULL);
}

bool pictureVisible = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, i;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case BUTTON_DRAW_ID:
			pictureVisible = true;
			InvalidateRect(hWnd, NULL, true);
			break;
		case BUTTON_CLEAR_ID:
			pictureVisible = false;
			InvalidateRect(hWnd, NULL, true);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_DRAWITEM:
	{
		DRAWITEMSTRUCT *draw = (DRAWITEMSTRUCT*)lParam;
		HBRUSH brush;
		HPEN pen;

		switch (draw->CtlID)
		{
			case BUTTON_DRAW_ID:
				{
					HDC hdc = draw->hDC;

					if (draw->itemAction == ODA_SELECT) {
						brush = CreateSolidBrush(RGB(144, 238, 0));
					}
					else {
						brush = CreateSolidBrush(RGB(144, 238, 144));
					}

					SelectObject(hdc, brush);
					Ellipse(hdc, 0, 0, 50, 50);
				}
				break;
			case BUTTON_CLEAR_ID:
				{
					HDC hdc = draw->hDC;

					if (draw->itemAction == ODA_SELECT) {
						brush = CreateSolidBrush(RGB(205, 92, 92));
					}
					else {
						brush = CreateSolidBrush(RGB(240, 128, 144));
					}

					SelectObject(hdc, brush);
					Ellipse(hdc, 0, 0, 50, 50);
				}
				break;
		default:
			break;
		}

	}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		HBRUSH brush;
		HPEN pen;
		if (pictureVisible)
		{
			pen = CreatePen(BS_SOLID, 2, RGB(0, 0, 0));
			SelectObject(hdc, pen);
			Rectangle(hdc, 10, 10, 450, 450);

			brush = CreateSolidBrush(RGB(50, 150, 50));
			SelectObject(hdc, brush);
			pen = CreatePen(BS_SOLID, 2, RGB(50, 150, 50));
			SelectObject(hdc, pen);
			Ellipse(hdc, 50, 300, 400, 150);
			Ellipse(hdc, 90, 200, 190, 100);
			Ellipse(hdc, 260, 200, 360, 100);

			brush = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hdc, brush);
			pen = CreatePen(BS_SOLID, 2, RGB(255, 255, 255));
			SelectObject(hdc, pen);
			Ellipse(hdc, 100, 190, 180, 110);
			Ellipse(hdc, 270, 190, 350, 110);

			brush = CreateSolidBrush(RGB(0, 0, 0));
			SelectObject(hdc, brush);
			pen = CreatePen(BS_SOLID, 2, RGB(0, 0, 0));
			SelectObject(hdc, pen);
			Ellipse(hdc, 110, 150, 150, 190);
			Ellipse(hdc, 280, 150, 320, 190);
			
			Arc(hdc, 90, 300, 350, 200, 350, 250, 70, 250);

			brush = CreateSolidBrush(RGB(221, 0, 111));
			SelectObject(hdc, brush);
			pen = CreatePen(BS_SOLID, 2, RGB(0, 0, 0));
			SelectObject(hdc, pen);

			MoveToEx(hdc, 100, 120, NULL);
			LineTo(hdc, 90, 110);
			MoveToEx(hdc, 120, 105, NULL);
			LineTo(hdc, 115, 90);

			MoveToEx(hdc, 285, 105, NULL);
			LineTo(hdc, 275, 95);
			MoveToEx(hdc, 320, 100, NULL);
			LineTo(hdc, 315, 85);
		}
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