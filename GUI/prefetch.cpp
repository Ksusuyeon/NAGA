#include "stdafx.h"
#include "prefetch.h"
#include <io.h>
#include "FileIoHelper.h"


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

		free(file_name); // <<!
		return false;
	}

	free(file_name); // <<!
	return true;
}

bool run_PECmd(void) 
{
	// ������ �޾ƿ���
	std::wstring current_dir = get_current_module_dirEx();
	std::wstringstream strm;
	strm << current_dir << L"\\PECmd.exe -d \"C:\\Windows\\Prefetch\" --csv " << result_path;

	STARTUPINFO startupInfo = { 0 };
	PROCESS_INFORMATION processInfo;
	startupInfo.cb = sizeof(STARTUPINFO);


	// �����
	/*
	if (!CreateProcess(NULL,
					  (LPWSTR)strm.str().c_str(),
					   NULL,
					   NULL,
					   FALSE,
					   0,
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
	*/

	return true;
}



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

wchar_t *find_timeline_file(wstring path) {

	struct _wfinddata_t file_search;
	long handle;

	wstringstream strm;
	strm << L"C:\\Temp\\result\\" << L"*Timeline.csv";
	handle = _tfindfirst(strm.str().c_str(), &file_search);

	if (handle == -1) {
		log_err "�� ����" log_end;
		return nullptr;
	}
	else {
		log_err "�ֽ�! %ws", file_search.name log_end;

		wchar_t *file_name = (wchar_t*)malloc(sizeof(wchar_t) * 260);
		wcscpy(file_name, file_search.name);
		return file_name;
	}
}


bool read_csv(wchar_t *filename, map<string, string> *pdata)
{
	//pair<map<string, string>::iterator, bool> pr;

	//log_warn "[csv ���� �̸�]" log_end;
	//std::wstringstream strm;
	//strm << L"C:\\Temp\\result\\" << filename;
	//log_info "%ws", strm.str().c_str() log_end;
	//ifstream in_stream;
	//string line;
	//in_stream.open(strm.str().c_str());
	//while (!in_stream.eof()) {
	//	getline(in_stream, line);
	//	if (line.length() <= 0 || line.find(",", 0) == string::npos) {
	//		continue;
	//	}
	//	char *token = strtok(const_cast<char *>(line.c_str()), ",");
	//	char value[30];
	//	strcpy(value, token);
	//	replace_string(line, token, "");
	//	pr = (*pdata).insert(pair<string, string>(line, value));
	//	if (true == pr.second) {
	//		log_info "%ws\n", line log_end;
	//	}
	//	else {
	//		cout << "Already exist ";
	//	}
	//}
	//in_stream.close();


	// README 
	// stl stream �� �⺻ �������δ� utf8 ���ڿ� ó���� ����� ����
	// ����� �ֱ� �ѵ�, ���� �ϴ��� ��
	// 
	// �׳� �����ϰ� txt ���� ���, \r\n (�ٹٲ�)�� ã�Ƽ� ���δ����� ���� �а�,
	// ó����
	//

	PFILE_CTX file_context = nullptr;
	if (true != OpenFileContext(filename, true, file_context))
	{
		log_err "Can not open file. file=%ws", filename log_end;
		return false;
	}
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
			// line ���ڿ� �Ľ��ؼ� �ʿ��� �۾��ϱ� 
			//
			pdata->insert(std::pair<string, string>(utf8_line.get(), utf8_line.get()));
		}
		else
		{
			++curr;
		}		
	};

	//
	// ������ ���� \r\n ���� ������� �ʴ� ��� ������ ������ ������ �� ����
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
		// line ���ڿ� �Ľ��ؼ� �ʿ��� �۾��ϱ� 
		//
		pdata->insert(std::pair<string, string>(utf8_line.get(), utf8_line.get()));
	}

	return true;
}

void replace_string(std::string& subject, const std::string& search, const std::string& replace)
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace); pos += replace.length();
	}
}
