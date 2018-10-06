
#include "stdafx.h"
#include "log.h"

bool get_installed_program(void)
{

	//
	//	���� �α׸� �ʱ�ȭ�Ѵ�. 
	// 
	std::wstring current_dir = get_current_module_dirEx();
	std::wstringstream strm;
	strm << current_dir << L"\\MouseTrap.log";
	if (true != initialize_log(log_mask_all,
		log_level_debug,
		log_to_file | log_to_con | log_to_ods,
		strm.str().c_str()))
	{
		fwprintf(stderr, L"initialize_log() fail. give up! \n");
		return -1;
	}

	//
	//	�α��� ��� ������ �����Ѵ�. 
	//
	set_log_format(true,false,false,false);


	HKEY hUninstKey = NULL;
	HKEY hAppKey = NULL;
	TCHAR sAppKeyName[1024];
	TCHAR sSubKey[1024];
	TCHAR sDisplayName[1024];
	const TCHAR *sRoot = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	long lResult = ERROR_SUCCESS;
	DWORD dwType = KEY_ALL_ACCESS;
	DWORD dwBufferSize = 0;

	// uninstall key �޾ƿ���
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, sRoot, 0, KEY_READ, &hUninstKey) != ERROR_SUCCESS)
	{
		return false;
	}

	for (DWORD dwIndex = 0; lResult == ERROR_SUCCESS; dwIndex++)
	{
		//��� ����Ű �޾ƿ���
		dwBufferSize = sizeof(sAppKeyName);
		if ((lResult = RegEnumKeyEx(hUninstKey, dwIndex, sAppKeyName,
			&dwBufferSize, NULL, NULL, NULL, NULL)) == ERROR_SUCCESS)
		{
			//����Ű ����
			wsprintf(sSubKey, L"%s\\%s", sRoot, sAppKeyName);
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, sSubKey, 0, KEY_READ, &hAppKey) != ERROR_SUCCESS)
			{
				RegCloseKey(hAppKey);
				RegCloseKey(hUninstKey);
				return false;
			}

			//����Ű�� �̸� ������!
			dwBufferSize = sizeof(sDisplayName);
			if (RegQueryValueEx(hAppKey, L"DisplayName", NULL,
				&dwType, (unsigned char*)sDisplayName, &dwBufferSize) == ERROR_SUCCESS)
			{
				fwprintf(stderr, L"%s\n", sDisplayName);
//				log_info "%s",sDisplayName log_end;
			}
			else {
//				 ����!
			}

			RegCloseKey(hAppKey);
		}
	}

	RegCloseKey(hUninstKey);

	//
	//	�α� ����� �����Ѵ�. 
	//	
	finalize_log();


	return true;
}