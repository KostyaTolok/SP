// SP_lab8.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SP_lab8.h"
#include <TlHelp32.h>
#include <windowsx.h>

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
    LoadStringW(hInstance, IDC_SPLAB8, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLAB8));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLAB8));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLAB8);
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


HMENU hPopupMenu;

int procCount = 0;
PROCESSENTRY32 procs[1000];

int moduleCount = 0;
MODULEENTRY32 modules[1000];


void ShowProcs(HWND listBox)
{
    HANDLE toolHelp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    procCount = 0;
    bool result;
    do
    {
        procs[procCount].dwSize = sizeof(PROCESSENTRY32);
        result = Process32Next(toolHelp, &procs[procCount]);
        procCount++;
    } while (result);

    while (SendMessage(listBox, LB_GETCOUNT, 0, NULL))
        SendMessage(listBox, LB_DELETESTRING, 0, NULL);

    for (int i = 0; i < procCount; i++)
        SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)procs[i].szExeFile);

    CloseHandle(toolHelp);
}

void GetModules(DWORD pid, HWND listBox)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (snapshot != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 module;
        module.dwSize = sizeof(MODULEENTRY32);
        Module32First(snapshot, &module);

        do {
            SendMessage(listBox, LB_ADDSTRING, 0, (LPARAM)module.szModule);
        } while (Module32Next(snapshot, &module));
    }

    CloseHandle(snapshot);
}

#define IDLE 1
#define NORMAL 2
#define HIGH 3
#define REALTIME 4

void SetPriority(DWORD priorityClass, HWND listBox) {
    int itemId = SendMessage(listBox, LB_GETCURSEL, 0, NULL);
    if (itemId != -1) {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procs[itemId].th32ProcessID);
        SetPriorityClass(hProcess, priorityClass);
        CloseHandle(hProcess);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    static HWND listBox1, listBox2;

    switch (message)
    {
    case WM_CREATE:
    {
        listBox1 = CreateWindow(L"LISTBOX", L"Processes", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
            100, 20, 200, 500, hWnd, (HMENU)101, hInst, NULL);
        listBox2 = CreateWindow(L"LISTBOX", L"Modules", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL,
            350, 20, 200, 500, hWnd, (HMENU)102, hInst, NULL);

        ShowProcs(listBox1);
    }
    break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 101:
            if (HIWORD(wParam) == LBN_SELCHANGE)
            {
                int itemId = SendMessage(listBox1, LB_GETCURSEL, 0, NULL);
                if (itemId != -1) {
                    SendMessage(listBox2, LB_RESETCONTENT, 0, NULL);
                    GetModules(procs[itemId].th32ProcessID, listBox2);
                }
            }
            break;
        case IDLE:
            SetPriority(IDLE_PRIORITY_CLASS, listBox1);
            break;
        case NORMAL:
            SetPriority(NORMAL_PRIORITY_CLASS, listBox1);
            break;
        case HIGH:
            SetPriority(HIGH_PRIORITY_CLASS, listBox1);
            break;
        case REALTIME:
            SetPriority(REALTIME_PRIORITY_CLASS, listBox1);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_CONTEXTMENU:
        if ((HWND)wParam == listBox1) {
            int itemId = SendMessage(listBox1, LB_GETCURSEL, 0, NULL);

            if (itemId != -1) {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procs[itemId].th32ProcessID);
                int pC = GetPriorityClass(hProcess);

                hPopupMenu = CreatePopupMenu();
                AppendMenu(hPopupMenu, pC == IDLE_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, IDLE, L"Idle");
                AppendMenu(hPopupMenu, pC == NORMAL_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, NORMAL, L"Normal");
                AppendMenu(hPopupMenu, pC == HIGH_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, HIGH, L"High");
                AppendMenu(hPopupMenu, pC == REALTIME_PRIORITY_CLASS ? MF_CHECKED : MF_STRING, REALTIME, L"Real time");

                TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, hWnd, NULL);

                CloseHandle(hProcess);
            }
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code here...
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
