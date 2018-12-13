#pragma warning(disable:4996)

#include "stdafx.h"
#include "program.h"
using namespace std;

std::list<pprogram> softwares;
std::list<pprogram> removers;


//	���Ŵ�� ��ϰ� ��ġ�� ���α׷��� ���ϱ� ���� �Լ�
bool compare_lists(std::list<pprogram> *my_list) {

	get_installed_program();
	read_json();
	
	int result;

	for (auto remover : removers)
	{
		for (auto software : softwares) {
			result = wcscmp(software->id(), remover->id());
			pprogram temp;
			wstring guid;

			if (result == 0) {
				//
				//	yessign ����ó��
				//
				guid = remover->id();
				wstring yes_id = L"yessign7ActiveX";
				if (guid.find(yes_id) != wstring::npos) {
					std::wstringstream path_strm;
					path_strm << L"C:\\Windows\\SysWOW64\\yessign7Clear.exe";
					temp = new program(software->id(), software->name(), software->version(), software->version(), path_strm.str().c_str());
					my_list->push_back(temp);
					continue;
				}

				//
				//	���Ϸ�Ʈ �ɼ��� ������ ��� ��� �ڿ� �ٿ���
				//
				if (wcslen(remover->uninstaller()) > 0) {
					std::wstring path = L"";
					std::wstringstream path_strm;
					path_strm << software->uninstaller() << remover->uninstaller();
					temp = new program(software->id(), software->name(), software->version(), software->version(), path_strm.str().c_str());
				}
				else {
					temp = new program(software->id(), software->name(), software->version(), software->version(), software->uninstaller());
				}
				my_list->push_back(temp);
			}
		}
	}

	for (auto software : softwares) {
		delete software;
	}

	for (auto remover : removers) {
		delete remover;
	}

	softwares.clear();
	removers.clear();

	return true;
}

// ��ġ�� ���α׷��� �޾ƿ��� ���� �Լ�
bool get_installed_program(void)
{
	
	softwares.clear();
	get_installed_programs(softwares);

	log_warn "[ ��ġ�� ���� ��� ]" log_end;

	for (auto software : softwares)
	{
		log_info
			",%ws,%ws,%ws,%ws",
			software->id(),
			software->name(),
			software->version(),
			software->uninstaller()
			log_end;
	}
	getchar();	
	return true;
	
}

bool get_all_program(list<pprogram> *installed_list) {
	get_installed_programs(*installed_list);
	return true;
}

// json ������ �о�� �Ľ��ϴ� �Լ�
void read_json(void) {
	
	log_warn "[ ������ ���� ��� ]" log_end;

	removers.clear();
	
	CkJsonObject json;

	
	// json ���� �о����
	bool success = json.LoadFile("result.json");
	if (success != true) {
		cout << json.lastErrorText() << "\r\n";
		return;
	}

	// ���Ŵ�� ����Ʈ Ȯ���ϱ�
	CkJsonArray *programs = json.ArrayOf("programs");
	if (programs == 0) {
		cout << "not found" << "\r\n";
		return;
	}
	
	// ���Ŵ�� ����
	int numPrograms = programs->get_Size();

	// ���� �о����
	for (int i = 0; i < numPrograms; i++) {
		CkJsonObject *programObj = programs->ObjectAt(i);
		pprogram temp = new program(MbsToWcsEx(programObj->stringOf("guid")).c_str(),
									MbsToWcsEx(programObj->stringOf("name")).c_str(),
									L"",
									L"",
									MbsToWcsEx(programObj->stringOf("silent")).c_str());
		removers.push_back(temp);
		delete programObj;
	}
	delete programs;
	
	json.dispose();
	getchar();

}
