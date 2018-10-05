//#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <Windows.h>
#include <TlHelp32.h>

int _tmain(int argc, TCHAR *argv[]) {
	//���� ��� ���μ��� ���¸� ����.
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		_tprintf(_T("CreateToolhelp32Snapshot error\n"));
		exit(EXIT_FAILURE);
	}

	//���μ����� ���¸� ���� ����ü ����
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32)) {
		_tprintf(_T("Process32First error!\n"));
		CloseHandle(hProcessSnap);
		exit(EXIT_FAILURE);
	}

	_tprintf(_T("\t[Process name] \t\t[PID] \t[ThreadId] \t[PPID] \n"));

	do {
		_tprintf(_T("%25s %8d %8d %8d \n"), pe32.szExeFile, pe32.th32ProcessID, pe32.cntThreads, pe32.th32ParentProcessID);
	} while (Process32Next(hProcessSnap, &pe32));

	return 0;
}