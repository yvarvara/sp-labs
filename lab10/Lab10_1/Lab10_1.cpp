#include "stdafx.h"
#include "Lab10_1.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;								
TCHAR szTitle[MAX_LOADSTRING];					
TCHAR szWindowClass[MAX_LOADSTRING];			

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND Lab10_2_Wnd;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LAB10_1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB10_1));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB5A));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LAB10_1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

#define RBUTTON11_ID 1001
#define RBUTTON12_ID 1002
#define RBUTTON13_ID 1003

#define RBUTTON21_ID 1004
#define RBUTTON22_ID 1005
#define RBUTTON23_ID 1006
#define RBUTTON24_ID 1007

#define CHECKBOX_ID 1008

#define DRAW_MESSAGE "DrawMessage"
#define COLOR_MESSAGE "ColorMessage"
#define SHAPE_MESSAGE "ShapeMessage"

UINT WM_DRAW;
UINT WM_COLOR;
UINT WM_SHAPE;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; 

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 400, 300, NULL, NULL, hInstance, NULL);

   CreateWindow(L"button", L"Красный",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   10, 10, 80, 30, hWnd, (HMENU)RBUTTON11_ID, hInstance, NULL);
   CreateWindow(L"button", L"Зеленый",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   10, 35, 80, 30, hWnd, (HMENU)RBUTTON12_ID, hInstance, NULL);
   CreateWindow(L"button", L"Синий",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   10, 60, 80, 30, hWnd, (HMENU)RBUTTON13_ID, hInstance, NULL);

   CreateWindow(L"button", L"Ромб",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   200, 10, 80, 30, hWnd, (HMENU)RBUTTON21_ID, hInstance, NULL);
   CreateWindow(L"button", L"Квадрат",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   200, 35, 80, 30, hWnd, (HMENU)RBUTTON22_ID, hInstance, NULL);
   CreateWindow(L"button", L"Круг",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   200, 60, 80, 30, hWnd, (HMENU)RBUTTON23_ID, hInstance, NULL);
   CreateWindow(L"button", L"Звезда",
	   WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
	   200, 85, 80, 30, hWnd, (HMENU)RBUTTON24_ID, hInstance, NULL);

   CheckRadioButton(hWnd, RBUTTON11_ID, RBUTTON13_ID, RBUTTON11_ID);
   CheckRadioButton(hWnd, RBUTTON21_ID, RBUTTON24_ID, RBUTTON21_ID);

   CreateWindow(L"button", L"Нарисовать",
	   WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
	   100, 150, 100, 30, hWnd, (HMENU)CHECKBOX_ID, hInstance, NULL);

   WM_DRAW = RegisterWindowMessage((LPCWSTR)DRAW_MESSAGE);
   WM_COLOR = RegisterWindowMessage((LPCWSTR)COLOR_MESSAGE);
   WM_SHAPE = RegisterWindowMessage((LPCWSTR)SHAPE_MESSAGE);

   Lab10_2_Wnd = FindWindow(NULL, L"Lab10_2");

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
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

		if (wmId >= RBUTTON11_ID && wmId <= RBUTTON13_ID) {
			SendMessage(Lab10_2_Wnd, WM_COLOR, 0, wmId - RBUTTON11_ID);

			CheckRadioButton(hWnd, RBUTTON11_ID, RBUTTON13_ID, wmId);
		}
		else if (wmId >= RBUTTON21_ID && wmId <= RBUTTON24_ID) {
			SendMessage(Lab10_2_Wnd, WM_SHAPE, 0, wmId - RBUTTON21_ID);

			CheckRadioButton(hWnd, RBUTTON21_ID, RBUTTON24_ID, wmId);
		}
		else switch (wmId)
		{
		case CHECKBOX_ID:
			{
				SendMessage(Lab10_2_Wnd, WM_DRAW, 0, 0);

				HWND hWndCheck = GetDlgItem(hWnd, CHECKBOX_ID);
				LRESULT state = SendMessage(hWndCheck, BM_GETCHECK, 0, 0);

				if (state == BST_CHECKED)
				{
					SendMessage(Lab10_2_Wnd, WM_DRAW, 0, 1);
				}
				if (state == BST_UNCHECKED)
				{
					SendMessage(Lab10_2_Wnd, WM_DRAW, 0, 0);
				}
			}
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
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
