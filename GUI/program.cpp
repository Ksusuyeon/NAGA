#pragma warning(disable:4996)

#include "stdafx.h"
#include "program.h"
using namespace std;

std::list<pprogram> softwares;
std::list<pprogram> removers;



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
	//			delete temp;
			}
		}
	}

	return my_list;
}

bool get_installed_program(void)
{

	softwares.clear();
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
	}

	//
	//	�α��� ��� ������ �����Ѵ�. 
	//
	set_log_format(false, false, false, false);

	get_installed_programs(softwares);

	for (auto software : softwares)
	{
		log_info
			",%ws,%ws,%ws,%ws",
			software->id(),
			software->name(),
			software->version(),
			software->uninstaller()
			log_end;
	//	delete software;
	}
	getchar();

	//
	//	�α� ����� �����Ѵ�. 
	//	
	finalize_log();


	return true;
}


const wchar_t *convert_char(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);
	return wc;
}


void read_json(void) {

	removers.clear();

	//
	//	���� �α׸� �ʱ�ȭ�Ѵ�. 
	// 
	std::wstring current_dir = get_current_module_dirEx();
	std::wstringstream strm;
	strm << current_dir << L"\\JsonInfo.log";

	if (true != initialize_log(log_mask_all,
		log_level_debug,
		log_to_file | log_to_con | log_to_ods,
		strm.str().c_str()))
	{
		fwprintf(stderr, L"initialize_log() fail. give up! \n");
	}
	//
	//	�α��� ��� ������ �����Ѵ�. 
	//
	set_log_format(false, false, false, false);

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
		const wchar_t *guid = convert_char(programObj->stringOf("guid"));
		const wchar_t *name = convert_char(programObj->stringOf("name"));
		const wchar_t *version = convert_char(programObj->stringOf("version"));
		const wchar_t *uninstaller = convert_char(programObj->stringOf("uninstaller"));

		pprogram temp = new program(guid, name, version, version, uninstaller);
		removers.push_back(temp);
		delete programObj;
	}
	delete programs;

	for (auto remover : removers)
	{
		log_info
			"%ws,%ws,%ws,%ws",
			remover->id(),
			remover->name(),
			remover->version(),
			remover->uninstaller()
			log_end;
//		delete remover;
	}
	//
	//	�α� ����� �����Ѵ�. 
	//	
	finalize_log();

	getchar();

}
