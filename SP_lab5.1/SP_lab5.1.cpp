// SP_lab5.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SP_lab5.1.h"

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
    LoadStringW(hInstance, IDC_SPLAB51, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLAB51));

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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLAB51));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SPLAB51);
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

typedef struct data {
    int color;
    int figure;
    BOOL isDrawable;
}DATA;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hwndServer;

    WIN32_FIND_DATA FindFileData;
    HANDLE hFile;

    DATA data;

    static HWND red;
    static HWND green;
    static HWND blue;
    static HWND romb;
    static HWND kvadrat;
    static HWND krug;
    static HWND pi3da;
    static HWND draw;

    switch (message)
    {
        /*  case WM_INITDIALOG:
              hwndServer = FindWindow(NULL, (LPCWSTR)"SP_lab5.2");
              if (!hwndServer)
              {
                  hFile = FindFirstFile((LPCWSTR)"SP_lab5.2.exe", &FindFileData);
                  WinExec("SP_lab5.2.exe", 1);
                  FindClose(hFile);
              }*/
    case WM_CREATE:
    {
        red = CreateWindow(L"button", L"red", WS_CHILD |
            WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT
            | WS_GROUP | WS_TABSTOP,
            100, 100, 70, 30, hWnd, NULL, hInst, NULL);
        green = CreateWindow(L"button", L"green", WS_CHILD |
            WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT,
            200, 100, 70, 30, hWnd, NULL, hInst, NULL);
        blue = CreateWindow(L"button", L"blue", WS_CHILD |
            WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT,
            300, 100, 70, 30, hWnd, NULL, hInst, NULL);
        romb = CreateWindow(L"button", L"romb", WS_CHILD |
            WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT |
            WS_GROUP | WS_TABSTOP,
            100, 140, 70, 30, hWnd, NULL, hInst, NULL);
        kvadrat = CreateWindow(L"button", L"kvadrat", WS_CHILD |
            WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT,
            200, 140, 70, 30, hWnd, NULL, hInst, NULL);
        krug = CreateWindow(L"button", L"krug", WS_CHILD |
            WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT,
            300, 140, 70, 30, hWnd, NULL, hInst, NULL);
        pi3da = CreateWindow(L"button", L"pi3da", WS_CHILD |
            WS_VISIBLE | BS_AUTORADIOBUTTON | BS_LEFTTEXT,
            400, 140, 70, 30, hWnd, NULL, hInst, NULL);
        draw = CreateWindow(L"button", L"draw", WS_CHILD |
            WS_VISIBLE | BS_AUTOCHECKBOX | BS_LEFTTEXT,
            100, 180, 70, 30, hWnd, NULL, hInst, NULL);
    }
    break;
    case 101:
    {
        if (SendMessage(red, BM_GETCHECK, 0, 0L) == BST_CHECKED) {
            data.color = 1;
        }
        if (SendMessage(green, BM_GETCHECK, 0, 0L) == BST_CHECKED) {
            data.color = 2;
        }
        if (SendMessage(blue, BM_GETCHECK, 0, 0L) == BST_CHECKED) {
            data.color = 3;
        }
        if (SendMessage(romb, BM_GETCHECK, 0, 0L) == BST_CHECKED) {
            data.figure = 1;
        }
        if (SendMessage(kvadrat, BM_GETCHECK, 0, 0L) == BST_CHECKED) {
            data.figure = 2;
        }
        if (SendMessage(krug, BM_GETCHECK, 0, 0L) == BST_CHECKED) {
            data.figure = 3;
        }
        if (SendMessage(pi3da, BM_GETCHECK, 0, 0L) == BST_CHECKED) {
            data.figure = 4;
        }
        data.isDrawable = false;
        if (SendMessage(draw, BM_GETCHECK, 0, 0L) == BST_CHECKED)
            data.isDrawable = true;

        COPYDATASTRUCT MyCDS;

        hwndServer = FindWindow(NULL, L"SP_lab5.2");
        if (!hwndServer)
        {
            MessageBox(hWnd, L"Error", L"SP_lab5.1", MB_OK);
            break;
        }
        MyCDS.dwData = 1;
        MyCDS.cbData = sizeof(DATA);
        MyCDS.lpData = &data;

        SendMessage(hwndServer,
            WM_COPYDATA,
            (WPARAM)(HWND)hWnd,
            (LPARAM)(LPVOID)&MyCDS);
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
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
