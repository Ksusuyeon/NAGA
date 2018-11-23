#pragma once
#pragma warning(disable:4996)

#include <iostream>
#include <string>
#include <Msi.h>
#include <stdio.h>

#include <map>
#include <CkJsonObject.h>
#include <CkJsonArray.h>


#include <fstream>
#include <sstream>
#include <istream>
#include <stdlib.h>


using namespace std;

#pragma comment(lib, "msi.lib")
#pragma comment(lib,"ChilkatRelDll_x64.lib")

#define DAYCONTROL 2


/// prefetch ���� �Լ�
bool get_prefetch_info(map<string, string> *csv_map);
bool run_PECmd(void);
int delete_all_csv(LPCWSTR szDir, int recur);
wchar_t * find_timeline_file(wstring strm);
bool read_csv(wchar_t *filename, map<string, string> *pdata);
bool check_recently_used(map<string, string> *csv_map);
FILETIME str_to_filetime(string &sTime);

/// volume�� ���õ� �Լ�
void get_volume_name();
void get_volume_serial();
void get_volume_path(__in PWCHAR VolumeName);
void parse_volume_serial(map<string, string> *csv_map);