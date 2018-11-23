#include "stdafx.h"
#include "prefetch.h"
#include <io.h>
#include "FileIoHelper.h"

#include <iostream>

#define ARRAYSIZE(A) sizeof(A) / sizeof((A)[0])

list<wstring> volume_list;
map<wstring, wstring> volume_serial_list;
wstring result_path = L"\"C:\\Temp\\result\\";


bool get_prefetch_info(map<string,string> *csv_map) 
{

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

	if (!read_csv(file_name, csv_map))
	{
		log_err "read_csv() failed. file_name=%ws",
			file_name
			log_end;

		free(file_name);
		return false;
	}
	free(file_name);
	
	if (!check_recently_used(csv_map)) {
		log_err "check_recently_used() failed." log_end;
	}



	//
	// 사용자 PC의 모든 volume의 목록을 받아온다.
	//
	get_volume_name();

	//
	//	volume의 이름과 serial을 맵의 형태로 만든다.
	//
	get_volume_serial();
	
	parse_volume_serial(csv_map);
	for (map<string, string>::iterator iter = csv_map->begin(); iter != csv_map->end(); iter++)
	{
		log_info "Key : %s  - Value : %s", iter->first.c_str(), iter->second.c_str() log_end;
	}

	return true;
}

/// @brief PECmd.exe를 실행하고 csv 파일을 생성한다.
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

/// @brief CSV파일을 파싱하기 위해 임시로 생성한 폴더와 파일을 제거한다.
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

/// @brief Timeline.csv 파일을 찾는다.
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

/// @brief CSV파일을 읽어와 파싱한 후 중복된 값을 제거한다.
bool read_csv(wchar_t *filename, map<string, string> *pdata)
{
	// README 
	// stl stream 이 기본 설정으로는 utf8 문자열 처리를 제대로 못함
	// 방법이 있긴 한데, 어케 하는지 모름
	// 
	// 그냥 무식하게 txt 파일 열어서, \r\n (줄바꿈)을 찾아서 라인단위로 끊어 읽고,
	// 처리함
	//

	std::wstringstream strm;
	strm << L"C:\\Temp\\result\\" << filename;

	PFILE_CTX file_context = nullptr;
	if (true != OpenFileContext(strm.str().c_str(), true, file_context))
	{
		log_err "Can not open file. file=%ws", filename log_end;
		return false;
	}
	log_info "Open file. file=%ws", strm.str().c_str() log_end;
	SmrtFileCtx context_guard(file_context);
	
	//
	// read line (0x0d0a)
	//
	char* buf = file_context->FileView;
	uint32_t prev = 0;
	uint32_t curr = 0;
	while (curr < file_context->FileSize)
	{
		if (buf[curr] == 0x0D && buf[curr + 1] == 0x0A)
		{
			uint32_t length = (curr - prev) * sizeof(char);
			char_ptr utf8_line((char*)malloc(length + sizeof(char)), [](char* p) {if (nullptr != p) free(p); });
			if (nullptr == utf8_line.get())
			{
				log_err "not enough memory. give up." log_end;
				return false;
			}
			memcpy(utf8_line.get(), &buf[prev], length);
			utf8_line.get()[length] = 0x00;

			//
			// update position pointers
			//
			curr += 2;
			prev = curr;
			
			//
			// UTF-8 string --> Wide Char string (windows default)
			//
			std::wstring wcs_string = Utf8MbsToWcsEx(utf8_line.get());					
			std::string utf8_string = utf8_line.get();
		
			
			//
			// ToDo. 
			// line 문자열 파싱해서 필요한 작업하기 
			//
			std::string comma_string = ",";
			int location = utf8_string.find(comma_string);

			string date = utf8_string.substr(0, location);
			string name = utf8_string.substr(location + 1);
			pdata->insert(std::pair<string, string>(name, date));
		}
		else
		{
			++curr;
		}		
	};

	//
	// 파일의 끝이 \r\n 으로 종료되지 않는 경우 마지막 라인이 존재할 수 있음
	//
	if (prev < curr)
	{
		uint32_t length = (curr - prev) * sizeof(char);
		char_ptr utf8_line((char*)malloc(length + sizeof(char)), [](char* p) {if (nullptr != p) free(p); });
		if (nullptr == utf8_line.get())
		{
			log_err "not enough memory. give up." log_end;
			return false;
		}		
		memcpy(utf8_line.get(), &buf[prev], length);
		utf8_line.get()[length] = 0x00;

		//
		// UTF-8 string --> Wide Char string (windows default)
		//
		std::wstring wcs_string = Utf8MbsToWcsEx(utf8_line.get());

		//
		// TODO.
		// line 문자열 파싱해서 필요한 작업하기 
		//

		//
		// ToDo. 
		// line 문자열 파싱해서 필요한 작업하기 
		//
		std::string comma_string = ",";
		stringstream stm;
		stm << WcsToMbsUTF8Ex(wcs_string.c_str());
		int location = stm.str().find(comma_string);

		string date = stm.str().substr(0, location);
		string name = stm.str().substr(location + 1);
		pdata->insert(std::pair<string, string>(name, date));
	}

	return true;
}

/// @brief 최근에 사용된 파일은
/// (prefetch.h에 선언된 DAYCOUNT 기준)목록에서 제거한다.
bool check_recently_used(map<string, string> *csv_map) {

	csv_map->erase(string("ExecutableName"));				//	cSV 파일 첫 문장 제거

	//	MyLib 활용, 현재 system 시간을 string으로 변환
	string cur_time = time_now_to_str(true, false);
	log_info "cur time : %s", cur_time.c_str() log_end;

	//	FILETIME 연산을 위한 선언
	__int64 IN_DAY = (__int64)10000000 * 60 * 60 * 24;

	//	string 형태의 시간을 FILETIME으로 변환 (csv에서 읽어온 시간을 계산하기 위함)
	FILETIME point_time = str_to_filetime(cur_time);
	LARGE_INTEGER temp_time;
	memcpy(&temp_time, &point_time, sizeof(FILETIME));
	temp_time.QuadPart -= IN_DAY * DAYCONTROL;
	memcpy(&point_time, &temp_time, sizeof(FILETIME));
	
	//
	//	최근에 사용된 exe 파일의 목록을 제거하기 위한 리스트 생성
	//	마지막 사용시간이 기준시간 이후 일 때 MAP 에서 삭제해줌
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

	/*
	for (auto remove : remove_list) {
		remove_list.remove(remove);
	}
	remove_list.clear();
	*/

	return true;
}


/// @brief  `2018-11-13 00:54:24 포맷 시간 문자열을 입력받는다. 
///			FILETIME의 형태로 반환한다.
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


void get_volume_name() {
	HANDLE find_handle = INVALID_HANDLE_VALUE;
	WCHAR volume_name[MAX_PATH] = L"";
	WCHAR device_name[MAX_PATH] = L"";
	size_t index = 0;
	DWORD char_count = 0;
	BOOL success = FALSE;


	find_handle = FindFirstVolumeW(volume_name, ARRAYSIZE(volume_name));
	if (find_handle == INVALID_HANDLE_VALUE) {
		log_err "FindFirstVolumeW failed" log_end;
	}

	for (;;) {
		index = wcslen(volume_name) - 1;
		if (volume_name[0] != L'\\' ||
			volume_name[1] != L'\\' ||
			volume_name[2] != L'?' ||
			volume_name[3] != L'\\' ||
			volume_name[index] != L'\\')
		{
			log_err "FindFirstVolumeW/FindNextVolumeW returned a bad path: %s\n", volume_name log_end;
			break;
		}
		//
		//  QueryDosDeviceW does not allow a trailing backslash,
		//  so temporarily remove it.
		volume_name[index] = L'\0';

		char_count = QueryDosDeviceW(&volume_name[4], device_name, ARRAYSIZE(device_name));

		volume_name[index] = L'\\';

		if (char_count == 0)
		{
			log_err "QueryDosDeviceW failed with error code \n" log_end;
			break;
		}
		get_volume_path(volume_name);
		//
		//  Move on to the next volume.
		success = FindNextVolumeW(find_handle, volume_name, ARRAYSIZE(volume_name));

		if (!success)
		{
			log_err "FindNextVolumeW failed with error code" log_end;
			break;
		}
	}

	FindVolumeClose(find_handle);
	find_handle = INVALID_HANDLE_VALUE;
}

void get_volume_path(
	__in PWCHAR VolumeName
)
{
	DWORD  CharCount = MAX_PATH + 1;
	PWCHAR Names = NULL;
	PWCHAR NameIdx = NULL;
	BOOL   Success = FALSE;


	for (;;)
	{
		//
		//  Allocate a buffer to hold the paths.


		Names = (PWCHAR) new BYTE[CharCount * sizeof(WCHAR)];

		if (!Names)
		{
			//
			//  If memory can't be allocated, return.
			return;
		}

		//
		//  Obtain all of the paths
		//  for this volume.
		Success = GetVolumePathNamesForVolumeNameW(
			VolumeName, Names, CharCount, &CharCount
		);

		if (Success)
		{
			break;
		}

		if (GetLastError() != ERROR_MORE_DATA)
		{
			break;
		}

		//
		//  Try again with the
		//  new suggested size.
		delete[] Names;
		Names = NULL;
	}

	if (Success)
	{
		//
		//  Display the various paths.
		for (NameIdx = Names;
			NameIdx[0] != L'\0';
			NameIdx += wcslen(NameIdx) + 1)
		{
			wprintf(L"  %s", NameIdx);
			log_info "%s",NameIdx log_end;
			wstring str(NameIdx);
			volume_list.push_back(str);
		}
		wprintf(L"\n");
	}

	if (Names != NULL)
	{
		delete[] Names;
		Names = NULL;
	}

	return;
}

void get_volume_serial() {

	for (auto volume : volume_list) {
		DWORD VolumeSerialNumber = 0;

		GetVolumeInformation(volume.c_str(), NULL, NULL, &VolumeSerialNumber, NULL, NULL, NULL, NULL);
		char Hex_output[500];
		itoa(VolumeSerialNumber, Hex_output, 16);

		wstringstream strm;
		strm << Hex_output;
		volume_serial_list.insert(pair<wstring, wstring>(strm.str().c_str(), volume.c_str()));
		log_info "%ws, %ws", strm.str().c_str(), volume.c_str() log_end;
	}
	
}

void parse_volume_serial(map<string, string> *csv_map) {
	for (map<string, string>::iterator iter = csv_map->begin(); iter != csv_map->end(); iter++)
	{
		for (map<wstring, wstring>::iterator serial_iter = volume_serial_list.begin(); serial_iter != volume_serial_list.end(); serial_iter++) {
			//
			//	volume_serial_list은 serial, name 의 형태로 저장되어 있다.
			//	ex) 122a601d, C:\
			
			//
			//	csv_map은 pull path, time 의 형태로 저장되어 있다.
			//	ex) \VOLUME{01d2cb8a2a2d3680-122a601d}\USERS\HEAT\APPDATA\LOCAL\TEMP\IS-5KMTA.TMP\DELFINOUNLOADER-G3.EXE, 2018-10-19 06:59:05

			
			if (iter->first.find("\\") == string::npos) {		//	full path를 알 수 없는 파일인 경우
				log_info "No full path file : %s", iter->first.c_str() log_end;
				csv_map->erase(iter->first);					//	우선은 목록에서 제거
				continue;
			}

			stringstream str_serial;
			str_serial << WcsToMbsUTF8Ex(serial_iter->first.c_str());
						
			int location = iter->first.find(str_serial.str());
			if (location != string::npos) {
				int len = str_serial.str().length();

				stringstream str_name;
				str_name << WcsToMbsUTF8Ex(serial_iter->second.c_str());

				string temp = iter->first;
				temp.replace(0, location + len + 2, str_name.str().c_str());

				// 원래의 데이터를 map 에서 삭제하고 새롭게 추가함
				csv_map->insert(std::pair<string, string>(temp, iter->second));
				csv_map->erase(iter->first);
			}
				
		}
	}
}