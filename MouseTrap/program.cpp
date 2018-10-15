
#include "stdafx.h"
#include "log.h"

#include <errno.h>

#include <iostream>
#include <string>
#include <Msi.h>
#pragma comment(lib, "msi.lib")

using namespace std;


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
		return false;
	}

	//
	//	�α��� ��� ������ �����Ѵ�. 
	//
	set_log_format(false, false, false, false);

	//
	//	MyLib version
	//

	std::list<pprogram> softwares;
	//_ASSERTE(true == get_installed_programs(softwares));
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
		delete software;
	}
	getchar();
	//
	//	�α� ����� �����Ѵ�. 
	//	
	finalize_log();


	return true;
}
