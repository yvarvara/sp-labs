#include "stdafx.h"
#include "Lab12.h"
#include "math.h"

#define MAX_LOADSTRING 100
#define ID_BUTTON_1 3000
#define ID_BUTTON_2 3001

HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            
HWND hBtn1;
HWND hBtn2;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LAB12, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB12));

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB12));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB12);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

HWND hWnd;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; 

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	return TRUE;
}
LPWSTR CharToLPWSTR(LPCSTR char_string)
{
	LPWSTR res;
	DWORD res_len = MultiByteToWideChar(1251, 0, char_string, -1, NULL, 0);
	res = (LPWSTR)GlobalAlloc(GPTR, (res_len + 1) * sizeof(WCHAR));
	MultiByteToWideChar(1251, 0, char_string, -1, res, res_len);
	return res;
}

char str[5]="AAAA";
HANDLE hEvent;
HANDLE hThread[4];
CRITICAL_SECTION cs;

DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
	for (;;)
	{
		if ((int)lpParam == 1)
		{
			WaitForSingleObject(hEvent, 1);
			SetEvent(hEvent);
			EnterCriticalSection(&cs);         
			strcpy(str, "AAAA");
			Sleep(1000);
			LeaveCriticalSection(&cs);          
			ResetEvent(hEvent);
		}
		else if((int)lpParam == 2)
		{		
			WaitForSingleObject(hEvent, 1);

			SetEvent(hEvent);			       
			EnterCriticalSection(&cs);        
			strcpy(str, "BBBB");
			Sleep(1000);
			LeaveCriticalSection(&cs);          
			ResetEvent(hEvent);
		}
		else if ((int)lpParam == 3)
		{
			WaitForSingleObject(hEvent, 1);
			SetEvent(hEvent);
			EnterCriticalSection(&cs);         
			strcpy(str, "CCCC");
			Sleep(1000);
			LeaveCriticalSection(&cs);          
			ResetEvent(hEvent);
		}
		else if ((int)lpParam == 4)
		{
			WaitForSingleObject(hEvent, 1);
			SetEvent(hEvent);
			EnterCriticalSection(&cs);         
			strcpy(str, "DDDD");
			Sleep(1000);
			LeaveCriticalSection(&cs);          
			ResetEvent(hEvent);
		}
		InvalidateRect(hWnd, NULL, FALSE);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	DWORD dwThreadId[4];
	int param[4] = { 1,2,3,4 };
	switch (message)
	{
	case WM_CREATE:
		hEvent = CreateEventW(NULL, TRUE, FALSE, L"DRAW");
		InitializeCriticalSection(&cs);     
		hThread[0] = CreateThread(NULL, 0, ThreadFunc, (LPVOID)param[0], 0, &dwThreadId[0]);
		hThread[1] = CreateThread(NULL, 0, ThreadFunc, (LPVOID)param[1], 0, &dwThreadId[1]);
		hThread[2] = CreateThread(NULL, 0, ThreadFunc, (LPVOID)param[2], 0, &dwThreadId[2]);
		hThread[3] = CreateThread(NULL, 0, ThreadFunc, (LPVOID)param[3], 0, &dwThreadId[3]);
	
		if (hThread[0] == NULL) MessageBoxW(NULL, L"Cannot create first thread", L"Fail", MB_OK);
		if (hThread[1] == NULL) MessageBoxW(NULL, L"Cannot create second thread", L"Fail", MB_OK);
		if (hThread[2] == NULL) MessageBoxW(NULL, L"Cannot create third thread", L"Fail", MB_OK);
		if (hThread[3] == NULL) MessageBoxW(NULL, L"Cannot create fourth thread", L"Fail", MB_OK);

		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DeleteCriticalSection(&cs);        
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		RECT Rect;
		HFONT hf;
		GetClientRect(hWnd, &Rect);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HBRUSH hBrush;                                 
		hBrush = CreateSolidBrush(RGB(255, 255, 255)); 
		FillRect(hdc, &Rect, hBrush);

		long lfHeight;

		hf = CreateFontW(30, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");
		SelectObject(hdc, hf);
		SetTextColor(hdc, RGB(204, 51, 153));
		TextOutW(hdc, 200, 200, CharToLPWSTR(str), strlen(str));
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		CloseHandle(hThread[0]);
		CloseHandle(hThread[1]);
		CloseHandle(hThread[2]);
		CloseHandle(hThread[3]);
		DeleteCriticalSection(&cs);       
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