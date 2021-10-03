// SP_lab6.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SP_lab6.h"
#include <string> 
#include <atlstr.h>

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
    LoadStringW(hInstance, IDC_SPLAB6, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLAB6));

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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLAB6));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLAB6);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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


static HWND hHour;
static HWND hMinute;
static HWND hSecond;

SYSTEMTIME getTime()
{
    SYSTEMTIME sm;
    TCHAR dateTime[32];
    GetSystemTime(&sm);
    wsprintf(dateTime, _T("%d.%d.%d %d:%d:%d:%d"), sm.wDay, sm.wMonth, sm.wYear, sm.wHour, sm.wMinute, sm.wSecond, sm.wMilliseconds);
    return sm;
}

DWORD CALLBACK drowHour(HWND hWnd) {
    while (TRUE) {
       
        int hour = getTime().wHour;
        TCHAR buf[3];
        _itot_s(hour, buf, 10);
        SetWindowText(hHour, buf);
        Sleep(10000);
    }
    return 0;
}
DWORD CALLBACK drowMinute(HWND hWnd) {
    while (TRUE) {
        int hour = getTime().wMinute;
        TCHAR buf[3];
        _itot_s(hour, buf, 10);
        SetWindowText(hMinute, buf);
        Sleep(1000);
    }
    return 0;
}
DWORD CALLBACK drowSecond(HWND hWnd) {
    while (TRUE) {
        int hour = getTime().wSecond;
        TCHAR buf[3];
        _itot_s(hour, buf, 10);
        SetWindowText(hSecond, buf);
        Sleep(100);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hStart;
    static HWND hStop;

    static HANDLE hThread1;
    static HANDLE hThread2;
    static HANDLE hThread3;

    switch (message)
    {
    case WM_CREATE:
    {
        hStart = CreateWindow(L"button", L"Start",
            WS_CHILD | WS_VISIBLE |
            BS_PUSHBUTTON,
            100, 100, 100, 30, hWnd, (HMENU)101, hInst, NULL);
        hStop = CreateWindow(L"button", L"Stop",
            WS_CHILD | WS_VISIBLE |
            BS_PUSHBUTTON,
            200, 100, 100, 30, hWnd, (HMENU)102, hInst, NULL);
        hHour = CreateWindow(TEXT("STATIC"), TEXT("Statika"),
            WS_VISIBLE | WS_CHILD,
            50, 200, 90, 22,
            hWnd, (HMENU)103, NULL, NULL
        );
        hMinute = CreateWindow(TEXT("STATIC"), TEXT("Statika"),
            WS_VISIBLE | WS_CHILD,
            150, 200, 90, 22,
            hWnd, (HMENU)103, NULL, NULL
        );
        hSecond = CreateWindow(TEXT("STATIC"), TEXT("Statika"),
            WS_VISIBLE | WS_CHILD,
            250, 200, 90, 22,
            hWnd, (HMENU)103, NULL, NULL
        );

       

        hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)drowHour, hWnd, CREATE_SUSPENDED, NULL);
        hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)drowMinute, hWnd, CREATE_SUSPENDED, NULL);
        hThread3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)drowSecond, hWnd, CREATE_SUSPENDED, NULL);
 

    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case 101:
                ResumeThread(hThread1);
                ResumeThread(hThread2);
                ResumeThread(hThread3);
                break;
            case 102:
                SuspendThread(hThread1);
                SuspendThread(hThread2);
                SuspendThread(hThread3);
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
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
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
