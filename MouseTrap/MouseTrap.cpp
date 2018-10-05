/**
 * @file    MouseTrap.cpp
 * @brief   Entry point for MouseTrap application
 *
 * @author  홍식이
 * @date    2018.10.05 17:17 created.
 * @copyright All rights reserved by Yonghwan, Roh.
**/

#include "stdafx.h"
#include "log.h"

/// @brief
int main()
{
	//
	//	파일 로그를 초기화한다. 
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
	//	로그의 출력 형식을 지정한다. 
	//
	set_log_format(true, true, true, true);

	log_dbg "Hello, MouseTrap" log_end; 
	log_info "Hello, MouseTrap" log_end;
	log_warn "Hello, MouseTrap" log_end;
	log_err "Hello, MouseTrap" log_end;


	//
	//	로그 모듈을 종료한다. 
	//	
	finalize_log();
    return 0;
}

