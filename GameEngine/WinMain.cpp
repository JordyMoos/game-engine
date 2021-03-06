
//#define _CRTDBG_MAP_ALLOC // For detecting memory leaks
#define WIN32_LEAN_AND_MEAN

#include <memory>
#include <windows.h>
#include <string>
#include <ctime>

#include "Logger.h"

char szWindowClass[] = "game engine";
char gameTitle[] = "Awesome title";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool isAlreadyRunning();
bool createMainWindow(HINSTANCE hInstance, int nCmdShow);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	if (isAlreadyRunning())
	{
		MessageBox(NULL, "There already is an instance running", gameTitle, NULL);
		return 1;
	}

	createMainWindow(hinstance, nCmdShow);

	MSG msg;
	while (true)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//break;
		LOG("IN LOOP");
	}

	return (int)msg.wParam;
}

bool isAlreadyRunning()
{
	CreateMutex(NULL, false, "Awesome game thing");

	return (GetLastError() == ERROR_ALREADY_EXISTS);
}

bool createMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Call to RegisterClassEx failed!", gameTitle, NULL);
		return false;
	}

	HWND hWnd = CreateWindow(
		szWindowClass,
		gameTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		500,
		100,
		NULL,
		NULL,
		hInstance,
		NULL);
	if (!hWnd)
	{
		MessageBox(NULL, "Call to CreateWindow failed!", gameTitle, NULL);
		return false;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		//int seconds = static_cast<int>(time(NULL));
		//std::string greeting = std::to_string(seconds);

		time_t rawtime;
		time(&rawtime);

		std::string timeString = std::string(ctime(&rawtime));

		TextOut(hdc, 10, 10, timeString.c_str(), timeString.size());
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
