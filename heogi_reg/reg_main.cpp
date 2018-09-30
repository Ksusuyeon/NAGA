#include<stdio.h>
#include<windows.h>
#include<tchar.h>	//for TCHAR
#include<crtdbg.h> //for _ASSERTE
#include<malloc.h>
#include<winreg.h>
#include<stdlib.h>

/*
���� ���� �ǹ̰� ��ȣ�ϴ�. V
�Է°� ������ ����. V

������ ���
����ϴ� ������ ��������. 
������ ����� �ʱ�ȭ����. 

���ҽ� ����
�Ҵ��� �޸𸮴� �ݵ�� �Ҹ�����. V
�ý��� ���ҽ� (������Ʈ�� Ű �ڵ� ��)�� �ݵ�� ��ȯ�ؾ� �Ѵ�. V
�����ڵ� �Լ��� �Ƚ��ڵ� �Լ�, ������ ��������.
char* ������ �Է����� �޴� �Լ��� RegOpenKeyExA()
wchar* ������ �Է����� �޴� �Լ��� RegOpenKeyExW()
�������� TCHAR* Ÿ���� ������ ����ϰ�, RegOpenKeyEx()
�翬�� printf �� _tprintf() �� ����ؾ� �մϴ�. V
char*, wchar* �� ��Ȯ�� �������� �������¿��� ȥ���ϸ� �ȵ� 
�����α׸� ���涧�� �ּ��� � ������ ������ ������ ������ �ʿ��� �ܼ��� ������. 
���ϰ� �Ǵ� GetLastError() �ڵ� ������ �������. V
�����α״� ���� ������ ���� ����� ���̴�. 
���ϰ��� �ִ� �ܺ� �Լ��� ȣ�������� �ݵ�� ���� ���� Ȯ������. V

�Լ� ���� ���� ���� 
value ���� output �Ķ���ͷ� ���ǰ� ������
value ���� Ÿ���� ��𿡼��� ��Ȯ�� ��� ���� �ʰ� �ִ�(string ����, dword ����). 
��ü���� �������δ� string Ÿ���̶�� �����ϰ� �ۼ��� �� ���׿�.
���� �ɰ��� �����ε�, �Ҹ�� �޸𸮿� �����ϴ� ������ �ֽ��ϴ�. 
out-parameter �� ���� value �� ����Ű�� �����ʹ� �̹� �Ҹ�Ǿ����ϴ�.

*/


// ASTX : "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{19DD1D8D-927F-45DF-ADF4-75D38267848D}"

bool Reg_Read(_In_z_ const char* subkey, _Outptr_ TCHAR* value) { // SAL ?? �Լ� ������ �뵵������ �ּ�

	_ASSERTE(subkey != nullptr);
	_ASSERTE(value == nullptr);
	if (subkey == nullptr || value != nullptr) {
		return false;
	}

	LONG ret;
	DWORD data_size = 1024;
	DWORD data_type;
	TCHAR* data_buffer = (TCHAR*)malloc(data_size);
	HKEY hKey;

	RtlZeroMemory(data_buffer, sizeof(data_buffer));

	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		subkey, 0, KEY_READ, &hKey);

	if (ret != ERROR_SUCCESS) {
		_tprintf("RegOpenKey Failed! \n ");
		_tprintf("error code %d\n", GetLastError());
		return false;
	}

	_ASSERTE(hKey != nullptr);
	_ASSERTE(data_buffer != nullptr);

	RegQueryValueEx(hKey, "UninstallString", 0, &data_type, (LPBYTE)data_buffer, (DWORD*)&data_size);
	RegCloseKey(hKey);

	value = data_buffer;
	_tprintf("Value : %p ##  %s\n", value, value);
	data_buffer = NULL;
	return true;
}

int main() {
	TCHAR* value = nullptr;
	const char* subkey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{19DD1D8D-927F-45DF-ADF4-75D38267848D}";
	if (Reg_Read(subkey, value) == false) {
		_tprintf("Reg_Read error! : %d\n", GetLastError());
		return false;
	}
	//_ASSERTE(value != nullptr);
	_tprintf("Value : %p ##  %s\n", value, value); //������ value�� �Ҹ�Ȱǰ�?  value = data_buffer; �ϰ� free ���ߴµ�.
	free(value);
	getchar();

	return true;
}