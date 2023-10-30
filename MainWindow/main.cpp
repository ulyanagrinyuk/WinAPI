#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<string>
#include<vector>
#include"resource.h"

#define IDC_COMBO   1001
#define  IDC_BUTTON_APPLY   1002

CONST CHAR g_sz_WINDOW_CLASS[] = "My Window Class";
CONST CHAR* g_CURSOR[] = { "Busy.ani", "Working in Background", "Link Select", "Move.ani"};

std::vector<std::string> LoadCursorsFromDir(const std::string& directory);
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	wc.style = 0;

	//wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_CPU));
	//wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_RAM));
	
	wc.hIcon = (HICON)LoadImage(hInstance, "audio.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImage(hInstance, "print.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wc.hCursor = (HCURSOR)LoadImage(hInstance, "star.ani", IMAGE_CURSOR, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hbrBackground = HBRUSH(COLOR_WINDOW + 1);

	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_sz_WINDOW_CLASS;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Class registration filed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_WINDOW_CLASS, // CLASS NAME
		g_sz_WINDOW_CLASS,// WINDOW NAME
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HCURSOR hCursor;
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hCombo = CreateWindowEx
		(
			NULL,
			"ComboBox",
			"",
			WS_CHILD | WS_VISIBLE | CBN_DROPDOWN,
			10, 10,
			200, 200,
			hwnd,
			(HMENU)IDC_COMBO,
			GetModuleHandle(NULL),
			NULL
		);
	/*	for (int i = 0; i < sizeof(g_CURSOR) / sizeof(g_CURSOR[0]); i++)
		{
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)g_CURSOR[i]);
		}*/

		CHAR sz_cuurent_directory[MAX_PATH]{};
		GetCurrentDirectory(MAX_PATH, sz_cuurent_directory);
		MessageBox(hwnd, sz_cuurent_directory, "Current dir", MB_OK );
		std::vector<std::string> cursors = LoadCursorsFromDir("starcraft-original\\*");
		for (int i = 0; i < cursors.size(); i++)
		{
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)cursors[i].c_str());
		}

		HWND hButton = CreateWindowEx
		(
			NULL,
			"Button",
			"Apply",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			220, 10,
			100, 24,
			hwnd,
			(HMENU)IDC_BUTTON_APPLY,
			GetModuleHandle(NULL),
			NULL
		);
	}
	break;	
	break;
	case WM_COMMAND:
	{
		
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_APPLY:
		{
			HWND hCombo = GetDlgItem(hwnd, IDC_COMBO);
			int i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
			CHAR sz_filename[_MAX_FNAME]{};
			CHAR sz_filepath[_MAX_PATH] = "starcraft-original\\";
			SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)sz_filename);
			strcat(sz_filepath, sz_filename);
			//MessageBox(hwnd, sz_filepath, "Info", MB_OK);
			//HCURSOR
				hCursor = (HCURSOR)LoadImage(
				GetModuleHandle(NULL),
				sz_filepath,
				IMAGE_CURSOR,
				LR_DEFAULTSIZE, LR_DEFAULTSIZE,
				LR_LOADFROMFILE);		
			SetClassLong(hwnd, GCL_HCURSOR, (LONG)hCursor);
			SetClassLong(GetDlgItem(hwnd, IDC_BUTTON_APPLY), GCL_HCURSOR, (LONG)hCursor);
			SetClassLong(GetDlgItem(hwnd, IDC_COMBO), GCL_HCURSOR, (LONG)hCursor);
			//SetCursor(hCursor);
			return FALSE;
		}
		break;
		}
	}
		break;	
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE: DestroyWindow(hwnd); break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return NULL;
}
std::vector<std::string> LoadCursorsFromDir(const std::string& directory)
{
	std::vector<std::string> files;
	WIN32_FIND_DATA data;
	for (
		HANDLE hFind = FindFirstFile(directory.c_str(), &data); 
		
		 FindNextFile(hFind, &data);
		)
	{
		if(
			strcmp(strrchr(data.cFileName, '.'), ".ani") == 0 ||
			strcmp(strrchr(data.cFileName, '.'), ".cur") == 0			
			)
			files.push_back(data.cFileName);
	 }

	/*HANDLE hFile = FindFirstFile(directory.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{

		} while (FindNextFile(hFind, &data));
	}
	return files;*/

	return files;
}


