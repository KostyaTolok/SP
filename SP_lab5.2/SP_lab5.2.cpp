// SP_lab5.2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SP_lab5.2.h"

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
    LoadStringW(hInstance, IDC_SPLAB52, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLAB52));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLAB52));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLAB52);
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

typedef struct data {
    int color;
    int figure;
    BOOL isDrawable;
}DATA;

DATA data{1, 1, true};
int x = 0, y = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONUP:
    {
        //MessageBox(hWnd, L"click", L"SP_lab5.2", MB_OK);
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        HWND hServer = FindWindow(NULL, L"SP_lab5.1");
        if (!hServer) {
            MessageBox(hWnd, L"Error", L"SP_lab5.2", MB_OK);
            break;
        }
        SendMessage(hServer, 101, (WPARAM)hWnd, 0L);
    }
    break;
    case WM_COPYDATA:
    {
        RECT rc;
        GetClientRect(hWnd, &rc);
        PCOPYDATASTRUCT pMyDS = (PCOPYDATASTRUCT)lParam;
        data = *(DATA*)(pMyDS->lpData);
        if (data.isDrawable)
        {
            InvalidateRect(hWnd, &rc, TRUE);
        }

        //InvalidateRect(hWnd, &rc, TRUE);

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

        if (data.isDrawable) {
            SelectObject(hdc, GetStockObject(DC_BRUSH));
            switch (data.color)
            {
            case 1:
                SetDCBrushColor(hdc, RGB(200, 0, 0));
                break;
            case 2:
                SetDCBrushColor(hdc, RGB(0, 200, 0));
                break;
            case 3:
                SetDCBrushColor(hdc, RGB(0, 0, 200));
                break;
            default:
                break;
            }
            switch (data.figure)
            {
            case 1:
                Rectangle(hdc, x - 100, y - 100, x + 100, y + 100);
                break;
            case 2:
                POINT points[5];
                points[0] = { x - 80, y };
                points[1] = { x , y + 80 };
                points[2] = { x + 80, y };
                points[3] = { x , y - 80 };
                points[4] = { x - 80, y };
                Polygon(hdc, points, 5);
                break;
            case 3:
                Ellipse(hdc, x - 100, y - 100, x + 100, y + 100);
                break;
            case 4:
                POINT points2[11];
                points2[0] = { x - 50, y - 10 };
                points2[1] = { x - 15, y - 10 };
                points2[2] = { x, y - 50 };
                points2[3] = { x + 15, y - 10 };
                points2[4] = { x + 50, y - 10 };
                points2[5] = { x + 18, y + 10 };
                points2[6] = { x + 40, y + 50 };
                points2[7] = { x, y + 30 };
                points2[8] = { x - 40, y + 50 };
                points2[9] = { x - 18, y + 10 };
                points2[10] = { x - 50, y - 10 };
                Polygon(hdc, points2, 11);
                break;
            default:
                break;
            }
        }
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
