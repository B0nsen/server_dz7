#include "CodingThreadDlg.h"



CodingThreadDlg* CodingThreadDlg::ptr = NULL;

const int MAXSTRLEN = 255;
WSADATA wsaData;
SOCKET _socket;
SOCKET acceptSocket;
sockaddr_in addr;

using namespace std;



CodingThreadDlg::CodingThreadDlg(void)
{
	ptr = this;
}

CodingThreadDlg::~CodingThreadDlg(void)
{
	closesocket(acceptSocket);
	closesocket(_socket);
	WSACleanup();
}

void CodingThreadDlg::Cls_OnClose(HWND hwnd)
{

}

void CodingThreadDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	char buf[MAXSTRLEN];
	int i;
	wchar_t str[MAXSTRLEN];
	if (id == IDC_BUTTON1)
	{
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		addr.sin_family = AF_INET;
		inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
		addr.sin_port = htons(20000);
		connect(_socket, (SOCKADDR*)&addr, sizeof(addr));
		i = recv(acceptSocket, buf, MAXSTRLEN, 0);
		buf[i] = '\0';
		MultiByteToWideChar(CP_UTF8, 0, buf, -1, str, MAXSTRLEN);
		SetWindowText(hEdit2, str);
	}
	if (id == IDC_BUTTON2)
	{
		GetWindowText(hEdit1, str, MAXSTRLEN);
		WideCharToMultiByte(CP_UTF8, 0, str, -1, buf, MAXSTRLEN, NULL, NULL);
		send(acceptSocket, buf, strlen(buf), 0);
		DestroyWindow(hwnd);
	}
}

BOOL CodingThreadDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	hDialog = hwnd;
	hEdit1 = GetDlgItem(hwnd, IDC_EDIT1);
	hEdit2 = GetDlgItem(hwnd, IDC_EDIT2);
	return TRUE;
}


BOOL CALLBACK CodingThreadDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}