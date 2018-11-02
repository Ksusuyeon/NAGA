#pragma warning(disable:4996)

#include "stdafx.h"
#include "program.h"
using namespace std;

std::list<pprogram> softwares;
std::list<pprogram> removers;


//	���Ŵ�� ��ϰ� ��ġ�� ���α׷��� ���ϱ� ���� �Լ�
list<pprogram> compare_lists() {
	get_installed_program();
	read_json();

	std::list<pprogram> my_list;
	
	int result;

	for (auto remover : removers)
	{
		for (auto software : softwares) {
			result = wcscmp(software->id(), remover->id());
			if (result == 0) {
				pprogram temp = new program(software->id(), software->name(), software->version(), software->version(), software->uninstaller());
				my_list.push_back(temp);
			}
		}
	}

	return my_list;
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

// const char *�� const wchar_t *�� ��ȯ�ϴ� �Լ�
const wchar_t *convert_char(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);
	return wc;
}

// json ������ �о�� �Ľ��ϴ� �Լ�
void read_json(void) {

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
									MbsToWcsEx(programObj->stringOf("version")).c_str(),
									MbsToWcsEx(programObj->stringOf("version")).c_str(),
									MbsToWcsEx(programObj->stringOf("uninstaller")).c_str());
		removers.push_back(temp);
		delete programObj;
	}
	delete programs;

	getchar();

}
