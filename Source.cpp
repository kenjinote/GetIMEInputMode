#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "imm32")

#include <windows.h>

TCHAR szClassName[] = TEXT("Window");

DWORD GetInputScope(HWND hEdit)
{
	DWORD dwConv = 0, dwSent;
	const HIMC hImc = ImmGetContext(hEdit);
	if (!ImmGetOpenStatus(hImc))
	{
		return dwConv;
	}
	ImmGetConversionStatus(hImc, &dwConv, &dwSent);
	ImmReleaseContext(hEdit, hImc);
	return dwConv;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hStatic;
	static HWND hEdit;
	switch (msg)
	{
	case WM_CREATE:
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), 0, WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, 0, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		hStatic = CreateWindow(TEXT("STATIC"), 0, WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, 0, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		SetTimer(hWnd, 0x1234, 500, 0);
		break;
	case WM_TIMER:
	{
		const DWORD dwState = GetInputScope(hEdit);
		TCHAR szText[1024];
		if (dwState)
		{
			wsprintf(szText, TEXT("%s%s%s%s%s%s%s%s%s%s%s"),
				(dwState&IME_CMODE_ALPHANUMERIC) ? TEXT("IME_CMODE_ALPHANUMERIC\r\n") : TEXT(""),
				(dwState&IME_CMODE_NATIVE) ? TEXT("IME_CMODE_NATIVE\r\n") : TEXT(""),
				(dwState&IME_CMODE_CHINESE) ? TEXT("IME_CMODE_CHINESE\r\n") : TEXT(""),
				(dwState&IME_CMODE_HANGUL) ? TEXT("IME_CMODE_HANGUL\r\n") : TEXT(""),
				(dwState&IME_CMODE_JAPANESE) ? TEXT("IME_CMODE_JAPANESE\r\n") : TEXT(""),
				(dwState&IME_CMODE_KATAKANA) ? TEXT("IME_CMODE_KATAKANA\r\n") : TEXT(""),
				(dwState&IME_CMODE_LANGUAGE) ? TEXT("IME_CMODE_LANGUAGE\r\n") : TEXT(""),
				(dwState&IME_CMODE_FULLSHAPE) ? TEXT("IME_CMODE_FULLSHAPE\r\n") : TEXT(""),
				(dwState&IME_CMODE_ROMAN) ? TEXT("IME_CMODE_ROMAN\r\n") : TEXT(""),
				(dwState&IME_CMODE_CHARCODE) ? TEXT("IME_CMODE_CHARCODE\r\n") : TEXT(""),
				(dwState&IME_CMODE_HANJACONVERT) ? TEXT("IME_CMODE_HANJACONVERT\r\n") : TEXT("")
				);
		}
		else
		{
			lstrcpy(szText, TEXT("IMEがOFF"));
		}
		SetWindowText(hStatic, szText);
	}
	break;
	case WM_SIZE:
		MoveWindow(hEdit, 10, 10, 256, 32, TRUE);
		MoveWindow(hStatic, 10, 50, 512, 512, TRUE);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 0x1234);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("IMEの入力モードを取得"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
		);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
