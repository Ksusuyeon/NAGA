#include "stdafx.h"
#include "prefetch.h"
#include <io.h>
#include <time.h>

std::wstring result_path = L"\"C:\\Temp\\result\\";

bool get_prefetch_info() 
{
	map<string, string> csv_map;


	bool ret = run_PECmd();
	if (true != ret)
	{
		log_err "run_PECmd() failed." log_end;
		return false;
	}

	wchar_t *file_name = find_timeline_file(result_path);
	if (nullptr == file_name)
	{
		log_err "find_timeline_file() failed. result_path=%ws",
			result_path
			log_end;
		return false;
	}

	if (!read_csv(file_name, &csv_map))
	{
		log_err "read_csv() failed. file_name=%ws",
			file_name
			log_end;

		free(file_name);
		return false;
	}
	free(file_name);
	
	if (!check_recently_used(&csv_map)) {
		log_err "check_recently_used() failed." log_end;
	}

	
	for (map<string, string>::iterator iter = csv_map.begin(); iter != csv_map.end(); iter++)
	{
		log_info "Key : %s  - Value : %s", iter->first.c_str(), iter->second.c_str() log_end;
	}
	
	return true;
}

/// @brief PECmd.exe�� �����ϰ� csv ������ �����Ѵ�.
bool run_PECmd(void) 
{
	std::wstring current_dir = get_current_module_dirEx();
	std::wstringstream strm;
	strm << current_dir << L"\\PECmd.exe -d \"C:\\Windows\\Prefetch\" --csv " << result_path;

	STARTUPINFO startupInfo = { 0 };
	PROCESS_INFORMATION processInfo;
	startupInfo.cb = sizeof(STARTUPINFO);
	
	if (!CreateProcess(NULL,
					  (LPWSTR)strm.str().c_str(),
					   NULL,
					   NULL,
					   FALSE,
					   CREATE_NO_WINDOW,			
					   NULL,
					   NULL,
					   &startupInfo,
					   &processInfo))
	{
		log_err "CreateProcess() failed. cmd=%ws, gle=0x%08x",
			strm.str().c_str(),
			GetLastError()
			log_end;
		return false;
	}

	WaitForSingleObject(processInfo.hProcess, INFINITE);
	CloseHandle(processInfo.hThread); 
	CloseHandle(processInfo.hProcess);
	
	return true;
}

/// @brief CSV������ �Ľ��ϱ� ���� �ӽ÷� ������ ������ ������ �����Ѵ�.
int delete_all_csv(LPCWSTR szDir, int recur)
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	int res = 1;

	TCHAR DelPath[MAX_PATH];
	TCHAR FullPath[MAX_PATH];
	TCHAR TempPath[MAX_PATH];

	lstrcpy(DelPath, szDir);
	lstrcpy(TempPath, szDir);
	if (lstrcmp(DelPath + lstrlen(DelPath) - 4, _T("\\*.*")) != 0) {
		lstrcat(DelPath, _T("\\*.*"));
	}

	hSrch = FindFirstFile(DelPath, &wfd);
	if (hSrch == INVALID_HANDLE_VALUE) {
		if (recur > 0) RemoveDirectory(TempPath);
		return -1;
	}

	while (res) {
		wsprintf(FullPath, _T("%s\\%s"), TempPath, wfd.cFileName);

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
			SetFileAttributes(FullPath, FILE_ATTRIBUTE_NORMAL);
		}

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (lstrcmp(wfd.cFileName, _T("."))
				&& lstrcmp(wfd.cFileName, _T(".."))) {
				recur++;
				delete_all_csv(FullPath, recur);
				recur--;
			}
		}
		else {
			DeleteFile(FullPath);
		}

		res = FindNextFile(hSrch, &wfd);
	}

	FindClose(hSrch);

	if (recur > 0) 
		RemoveDirectory(TempPath);

	return 0;
}

/// @brief Timeline.csv ������ ã�´�.
wchar_t *find_timeline_file(wstring path) {

	struct _wfinddata_t file_search;
	long handle;

	wstringstream strm;
	strm << L"C:\\Temp\\result\\" << L"*Timeline.csv";
	handle = _tfindfirst(strm.str().c_str(), &file_search);

	if (handle == -1) {
		return nullptr;
	}
	else {
		wchar_t *file_name = (wchar_t*)malloc(sizeof(wchar_t) * 260);
		wcscpy(file_name, file_search.name);
		return file_name;
	}
}

/// @brief CSV������ �о�� �Ľ��� �� �ߺ��� ���� �����Ѵ�.
bool read_csv(wchar_t *filename, map<string,string> *pdata)
{
	pair<map<string, string>::iterator, bool> pr;

	std::wstringstream strm;
	strm << L"C:\\Temp\\result\\" << filename;
	
	ifstream in_stream;
	string line;
	in_stream.open(strm.str().c_str());

	if (!in_stream.good()) {
		log_err  "ifstream open err" log_end;
	}
	while (!in_stream.eof()) {
		getline(in_stream, line);
		if (line.length() <= 0 || line.find(",", 0) == string::npos) {
			continue;
		}
		char *token = strtok(const_cast<char *>(line.c_str()), ",");
		char value[30];
		strcpy(value, token);
		token = strtok(NULL, ",");
		pr = (*pdata).insert(pair<string, string>(string(token), string(value)));

		if (pr.second) {}						//	�ߺ��Ǵ� Ű ���� ���� ���	
	}
	in_stream.close();
	return true;
}

/// @brief �ֱٿ� ���� ������
/// (prefetch.h�� ����� DAYCOUNT ����)��Ͽ��� �����Ѵ�.
bool check_recently_used(map<string, string> *csv_map) {

	csv_map->erase(string("ExecutableName"));				//	cSV ���� ù ���� ����

	//	MyLib Ȱ��, ���� system �ð��� string���� ��ȯ
	string cur_time = time_now_to_str(true, false);
	log_info "cur time : %s", cur_time.c_str() log_end;

	//	FILETIME ������ ���� ����
	__int64 IN_DAY = (__int64)10000000 * 60 * 60 * 24;

	//	string ������ �ð��� FILETIME���� ��ȯ (csv���� �о�� �ð��� ����ϱ� ����)
	FILETIME point_time = str_to_filetime(cur_time);
	LARGE_INTEGER temp_time;
	memcpy(&temp_time, &point_time, sizeof(FILETIME));
	temp_time.QuadPart -= IN_DAY * DAYCONTROL;
	memcpy(&point_time, &temp_time, sizeof(FILETIME));

	//
	//	�ֱٿ� ���� exe ������ ����� �����ϱ� ���� ����Ʈ ����
	//	������ ���ð��� ���ؽð� ���� �� �� MAP ���� ��������
	list<string> remove_list;
	for (map<string, string>::iterator iter = csv_map->begin(); iter != csv_map->end(); iter++)
	{
		string temp = iter->second;
		FILETIME last_runtime = str_to_filetime(temp);
		if (!(CompareFileTime(&point_time, &last_runtime) == 1)) {
			remove_list.push_back(iter->first.c_str());		
		}
	}

	for (auto remove : remove_list) {
		csv_map->erase(remove);
	}

	for (auto remove : remove_list) {
		remove_list.remove(remove);
	}
	remove_list.clear();

	return true;
}


/// @brief  `2018-11-13 00:54:24 ���� �ð� ���ڿ��� �Է¹޴´�. 
///			FILETIME�� ���·� ��ȯ�Ѵ�.
///
FILETIME str_to_filetime(string &sTime) {
	istringstream istr(sTime);
	SYSTEMTIME st = { 0 };
	FILETIME ft = { 0 };

	istr >> st.wYear;
	istr.ignore(1, '-');
	istr >> st.wMonth;
	istr.ignore(1, '-');
	istr >> st.wDay;
	istr.ignore(1, ' ');
	istr >> st.wHour;
	istr.ignore(1, ':');
	istr >> st.wMinute;
	istr.ignore(1, ':');
	istr >> st.wSecond;
	00 >> st.wMilliseconds;
	SystemTimeToFileTime(&st, &ft);
	return ft;
}