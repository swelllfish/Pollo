// Pollo.cpp
// Date:2015-11-8
// Version: 1.0v
// Owner: Leonard
// Instructions: The first time to begning this project.The pet even have no appearance.
//				 It can only move and behave follow some specified action.

#include "stdafx.h"
#include <windows.h>
#include "Action.h"

#define ID_TIMER	1

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void SetWindowUnvisible(HWND);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Pollo");
	MSG msg;
	HWND hwnd;
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW|CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if(!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("Ni TM Zl Db Wo!"), szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow(szAppName, TEXT("Pollo"), WS_OVERLAPPEDWINDOW,
	0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
	NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cxClient, cyClient;
	static HBITMAP hBitMap;
	static POINT tmppt;
	static Action action;
	POINT currCursor;
	PAINTSTRUCT ps;
	HDC hdc, hdcBuffer;
	HINSTANCE hInstance;

	switch(message)
	{
	case WM_CREATE:
		SetTimer(hwnd, ID_TIMER, TIMER_CLK, NULL);
		SetWindowUnvisible(hwnd);

		//hBitMap = CreateBitmap(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 1, 1, NULL);
		hdc = GetDC(hwnd);
		hBitMap = CreateCompatibleBitmap(hdc, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		ReleaseDC(hwnd, hdc);
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		action.GetBitMap(hInstance);

		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		action.SurfaceChange(cxClient, cyClient);
		return 0;

	case WM_TIMER:
		GetCursorPos(&currCursor);
		action.GetCurrCursor(currCursor);
		action.CirCleMove(hBitMap);
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_PAINT:		
		hdc = BeginPaint(hwnd, &ps);
		hdcBuffer = CreateCompatibleDC(hdc);
		SelectObject(hdcBuffer, hBitMap);
		BitBlt(hdc, 0, 0, cxClient, cyClient, hdcBuffer, 0, 0, SRCCOPY);
		DeleteDC(hdcBuffer);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		KillTimer(hwnd, ID_TIMER);
		DeleteObject(hBitMap);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void SetWindowUnvisible(HWND hwnd)
{			
	LONG dwStyle, dwNewStyle, dwNewExStyle, dwExStyle;
	//设置窗口背景和边框透明
	dwStyle = GetWindowLong(hwnd, GWL_STYLE);
	dwNewStyle = WS_OVERLAPPED | WS_VISIBLE| WS_SYSMENU |WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	dwNewStyle &= dwStyle;
	SetWindowLong(hwnd,GWL_STYLE,dwNewStyle);
	dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	dwNewExStyle = WS_EX_LEFT |WS_EX_LTRREADING |WS_EX_RIGHTSCROLLBAR;
	dwNewExStyle &= dwExStyle;
	SetWindowLong(hwnd,GWL_EXSTYLE,dwNewExStyle);//设置新的扩展样式
	SetWindowPos(hwnd, NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	SetWindowLong(hwnd,GWL_EXSTYLE,GetWindowLong(hwnd,GWL_EXSTYLE)|WS_EX_LAYERED );  
	SetLayeredWindowAttributes(hwnd, RGB(255,255,255),255,LWA_ALPHA | LWA_COLORKEY);
}