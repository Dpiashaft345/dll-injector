
#include <tchar.h>
#include <windows.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

void Controlls(HWND);

HWND hParDllName;
HWND hProgName;

char DllName[MAX_PATH];
char ParDllName[255];
char ProgName[MAX_PATH];

int WINAPI WinMain(HINSTANCE hThisInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpszArgument,
    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(25,25,25));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx(
        0,                   /* Extended possibilites for variation */
        szClassName,         /* Classname */
        _T("DP dll injector"),       /* Title Text */
        WS_OVERLAPPEDWINDOW, /* default window */
        CW_USEDEFAULT,       /* Windows decides the position */
        CW_USEDEFAULT,       /* where the window ends up on the screen */
        500,                 /* The programs width */
        544,                 /* and height in pixels */
        HWND_DESKTOP,        /* The window is a child-window to desktop */
        NULL,                /* No menu */
        hThisInstance,       /* Program Instance handler */
        NULL                 /* No Window Creation data */
    );

    /* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

#define ID_INJECT 1

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    GetWindowText(hProgName, ProgName, 256);
    GetWindowText(hParDllName, ParDllName, 256);

    switch (message)                  /* handle the messages */
    {
    case WM_CTLCOLORSTATIC:
    {
        HDC hdc = (HDC)wParam;
        SetTextColor(hdc, RGB(255, 255, 255));
        SetBkColor(hdc, RGB(25, 25, 25));
            return (LRESULT)GetStockObject(NULL_BRUSH);

    }
    break;

    case WM_COMMAND:

        switch (wParam)
        {
            case ID_INJECT:
                DWORD pId;
                HWND App = FindWindowA(NULL, ProgName);
                if (ProgName == NULL)
                {
                 MessageBox(hwnd, "Cannot find program name!", "Cannot find program name!", MB_ICONERROR);

                }
                else
                {
                    GetWindowThreadProcessId(App, &pId);
                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);
                    if (hProcess == NULL)
                    {
                        MessageBox(hwnd, "Cannot open process!", "Cannot open process!", MB_ICONERROR);


                    }
                    else
                     {
                        GetFullPathNameA(ParDllName, MAX_PATH, DllName, NULL);
                        LPVOID LoadLib = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
                        LPVOID RemoteString = VirtualAllocEx(hProcess, NULL, strlen(DllName), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
                        WriteProcessMemory(hProcess, RemoteString, DllName, strlen(DllName), NULL);
                        CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLib, (LPVOID)RemoteString, NULL, NULL);
                        MessageBox(hwnd, "Success inject dll!", "Success inject dll!", MB_ICONINFORMATION);
                        CloseHandle(hProcess);
                        Sleep(10000);
                        exit(0);

                    }


                }


            break;
        }




     break;


       case WM_CREATE:
           Controlls(hwnd);

           break;

    case WM_DESTROY:
        PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}


void Controlls(HWND hwnd)
{
    CreateWindowW(L"Button", L"INJECT", WS_CHILD | WS_VISIBLE | WS_BORDER, 170, 400, 180, 40, hwnd,
        HMENU(ID_INJECT), NULL, NULL);

    CreateWindowW(L"Static", L"Program name", WS_CHILD | WS_VISIBLE , 10, 100, 100, 25, hwnd,
        NULL, NULL, NULL);

    CreateWindowW(L"Static", L"Dll name", WS_CHILD | WS_VISIBLE, 40, 250, 100, 25, hwnd,
        NULL, NULL, NULL);

    hProgName = CreateWindowW(L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 120, 100, 300, 30, hwnd,
        NULL, NULL, NULL);

    hParDllName = CreateWindowW(L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 120, 250, 300, 30, hwnd,
        NULL, NULL, NULL);

}
