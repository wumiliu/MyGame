// MyGame.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "resource.h"
#include "GameGraphic.h"
#include "PlatFormInterface.h "
#include "TestBaseCode.h"
#include "MySignal.h"
#include "GameMain.h"
#include "BaseFunc.h"


#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


using namespace  XPG;

// this signal stuff is pretty old, not sure if it should be updated 
class vaApplicationSignals
{
public:
	Signal0< void >                   event_Started;
	Signal0< void >                   event_Stopped;
	Signal0< void >                   event_BeforeStopped;
	Signal0< void >                   event_MouseCaptureChanged;

	Signal0< void >                   event_BeforeWindowResized;              // width, height, fullscreen
	Signal3< int, int, bool >         event_WindowResized;                    // width, height, fullscreen

	Signal1< float >                  event_Tick;
	Signal0< void >                   event_Render;

	//static LRESULT CALLBACK WndProcStatic( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	typedef LRESULT & LRESULTREF;
	typedef bool & boolRef;
	Signal6< boolRef, LRESULTREF, HWND, UINT, WPARAM, LPARAM >
		event_WinProcOverride;
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	TestPlatformGlobalFunction();

	vaApplicationSignals a;
	a.event_Render.Connect(TestBaseCode::GetInstancePtr(),&TestBaseCode::RunTest);
	a.event_Render.Disconnect(TestBaseCode::GetInstancePtr(), &TestBaseCode::RunTest);

	a.event_Render();

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	DWORD dwTime = ::GetTickCount();
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			g_gameMain.ProcessGame();
			UINT uiUse = ::GetTickCount() - dwTime;
			if (uiUse < 20)
			{
				::Sleep(20 - uiUse);
			}
			dwTime = ::GetTickCount();
		}
	}
	g_gameMain.DestoryGameMain();
	UnregisterClass(szWindowClass, hInstance);
	return 0;
/*
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYGAME));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;*/
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	int WINDOW_WIDTH = DEFAULT_SCREEN_WIDTH;
	int WINDOW_HEIGHT = DEFAULT_SCREEN_HEIGHT;



   hInst = hInstance; // 将实例句柄存储在全局变量中

   DEVMODE CurrentMode;
   EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &CurrentMode);
   int x = (CurrentMode.dmPelsWidth - WINDOW_WIDTH) / 2;
   int y = (CurrentMode.dmPelsHeight - WINDOW_HEIGHT) / 2;

  /* HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);*/

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 
	   x, y, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   RECT rect;
   ::GetClientRect(hWnd, &rect);

   RECT rectMain;
   GetWindowRect(hWnd, &rectMain);
   int nWidthJudge = WINDOW_WIDTH + (WINDOW_WIDTH - rect.right);
   int nHeightJudge = WINDOW_HEIGHT + (WINDOW_HEIGHT - rect.bottom);
   ::MoveWindow(hWnd, x, y, nWidthJudge, nHeightJudge, TRUE);
   ::GetClientRect(hWnd, &rect);
   GetWindowRect(hWnd, &rectMain);
   if (!g_gameMain.InitGameMain(hInst, hWnd, WINDOW_WIDTH, WINDOW_HEIGHT, NULL))
   {
	   LogMyMsg("g_gameMain.InitGameMain error");
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
const int WINDOWS_MOUSE_CLICK_INDEX = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
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
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
		g_objTestBaseCode.Release();
        break;
	case WM_MOUSEWHEEL:
		g_gameMain.OnMouseWheel((int)(short)HIWORD(wParam));
		break;
	case WM_LBUTTONDBLCLK:
		g_gameMain.OnLButtonDblClk((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_LBUTTONDOWN:
		g_gameMain.OnLButtonDown((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), WINDOWS_MOUSE_CLICK_INDEX);
		break;
	case WM_LBUTTONUP:
		g_gameMain.OnLButtonUp((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), WINDOWS_MOUSE_CLICK_INDEX);
		break;
	case WM_RBUTTONUP:
		g_gameMain.OnRButtonUp((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_MOUSEMOVE:
		g_gameMain.OnMouseMove((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), WINDOWS_MOUSE_CLICK_INDEX);
		break;
	case WM_KEYDOWN:
		g_gameMain.OnKeyDown(wParam);
		break;
	case WM_KEYUP:
		g_gameMain.OnKeyUp(wParam);
		break;
	case WM_ACTIVATE:
		g_gameMain.OnActivate(LOWORD(wParam), HIWORD(wParam));
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		return (INT_PTR)TRUE;
	}

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
