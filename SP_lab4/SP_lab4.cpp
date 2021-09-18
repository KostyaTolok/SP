// Lab4.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SP_Lab4.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name



// Forward declarations of functions included in this code module:
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

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SPLAB4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLAB4));

	MSG msg;

	// Main message loop:
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


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_SPLAB4));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//


void DrawBitmap(HDC hDC, int x, int y, HBITMAP hBitmap)
{
	HBITMAP hbm, hOldbm;
	HDC hMemDC;
	BITMAP bm;
	POINT  ptSize, ptOrg;

	// Создаем контекст памяти, совместимый
	// с контекстом отображения
	hMemDC = CreateCompatibleDC(hDC);

	// Выбираем изображение bitmap в контекст памяти
	hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmap);

	// Если не было ошибок, продолжаем работу
	if (hOldbm)
	{
		// Для контекста памяти устанавливаем тот же
		// режим отображения, что используется в
		// контексте отображения
		SetMapMode(hMemDC, GetMapMode(hDC));

		// Определяем размеры изображения
		GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);

		ptSize.x = bm.bmWidth;   // ширина
		ptSize.y = bm.bmHeight;  // высота

		// Преобразуем координаты устройства в логические
		// для устройства вывода
		DPtoLP(hDC, &ptSize, 1);

		ptOrg.x = 0;
		ptOrg.y = 0;

		// Преобразуем координаты устройства в логические
		// для контекста памяти
		DPtoLP(hMemDC, &ptOrg, 1);

		// Рисуем изображение bitmap
		BitBlt(hDC, x, y, ptSize.x, ptSize.y,
			hMemDC, ptOrg.x, ptOrg.y, SRCCOPY);

		// Восстанавливаем контекст памяти
		SelectObject(hMemDC, hOldbm);
	}

	// Удаляем контекст памяти
	DeleteDC(hMemDC);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rc;
	HDC hdc;
	static HBITMAP bitmaps[4];

	static int x = 0;
	static int y = 0;
	static int cur_x = 0;
	static int cur_y = 0;
	static int index = 0;
	static HBITMAP cur_bitmap;

	switch (message)
	{
	case WM_CREATE:
		bitmaps[0] = (HBITMAP)LoadImage(NULL, L"res\\1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bitmaps[1] = (HBITMAP)LoadImage(NULL, L"res\\2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bitmaps[2] = (HBITMAP)LoadImage(NULL, L"res\\3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		bitmaps[3] = (HBITMAP)LoadImage(NULL, L"res\\4.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		break;
	case WM_COMMAND:
		break;
	case WM_TIMER:
	{
		GetClientRect(hWnd, &rc);

		if (x == cur_x && y == cur_y) {
			KillTimer(hWnd, 1);
			InvalidateRect(hWnd, &rc, TRUE);
			break;
		}

		if (x-2 < cur_x) {
			cur_x-=3;
			index++;
			index %= 4;
		}
		if (x+2 > cur_x) {
			cur_x+=3;
			index++;
			index %= 4;
		}

		if (y-2 < cur_y) {
			cur_y-=3;
			index++;
			index %= 4;
		}
		if (y+2 > cur_y) {
			cur_y+=3;
			index++;
			index %= 4;

		}

		InvalidateRect(hWnd, &rc, TRUE);
	}
	break;
	case WM_LBUTTONUP:
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		SetTimer(hWnd, 1, 100, NULL);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		DrawBitmap(hdc, cur_x, cur_y, bitmaps[index]);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
