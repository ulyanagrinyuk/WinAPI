#include<Windows.h>
#include"resource.h"

CONST CHAR g_sz_LOGIN_INVITATION[] = "������� ��� ������������";

BOOL CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wPram, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hinstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wPram, LPARAM lParam)
{
	switch (uMsg)
	{
	case IDC_EDIT_LOGIN:
	{
		CONST INT SIZE = 256;
		CHAR sz_buffer[SIZE]{};
		HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
		SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
		if (HIWORD(wParam) == EN_SETFOCUS && strcmp(sz_buffer, g_sz_LOGIN_INVITATION) == 0)
			SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)"");
		if (HIWORD(wParam) == EN_KILLFOCUS && strcmp(sz_buffer, "") == 0)
			SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)g_sz_LOGIN_INVITATION);
	}
		break;
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LRESULT)hIcon);
		//SetFocus(GetDlgItem(hwnd, IDC_EDIT_LOGIN));
		HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
		SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)g_sz_LOGIN_INVITATION);

	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wPram))
		{
			
		case IDC_BUTTON_COPY:
		{		
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			HWND hPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
			SendMessage(hLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDOK: MessageBox(hwnd, "���� ������ ������ OK", "Info", MB_OK | MB_ICONINFORMATION); break;
		case IDCANCEL: EndDialog(hwnd, 0);
		}
		break;
	case WM_CLOSE: EndDialog(hwnd, 0);
	}
	return FALSE;
}