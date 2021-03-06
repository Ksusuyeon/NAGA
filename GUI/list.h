#pragma once

#include <conio.h>
#include <winioctl.h>
#include <winsvc.h>
#include <string>
#include <BaseWindowsHeader.h>
#include "stdafx.h"


typedef class blackp
{
public:
	blackp(_In_ const wchar_t* id,
		_In_ const wchar_t* name,
		_In_ const wchar_t* vender,
		_In_ const wchar_t* version,
		_In_ const wchar_t* uninstaller,
		_In_ const wchar_t* update,
		_In_ const wchar_t* bank) :
		_id(id),
		_name(name),
		_vender(vender),
		_version(version),
		_uninstaller(uninstaller),
		_update(update),
		_bank(bank)
	{}
public:
	const wchar_t* id() { return _id.c_str(); }
	const wchar_t* name() { return _name.c_str(); }
	const wchar_t* vendor() { return _vender.c_str(); }
	const wchar_t* version() { return _version.c_str(); }
	const wchar_t* uninstaller() { return _uninstaller.c_str(); }
	const wchar_t* update() { return _update.c_str(); }
	const wchar_t* bank() { return _bank.c_str(); }
	void setId(_In_ std::wstring id) { _id = id; }
	void setName(_In_ std::wstring name) { _name = name; }
	void setVendor(_In_ std::wstring vender) { _vender = vender; }
	void setVersion(_In_ std::wstring version) { _version = version; }
	void setUninstaller(_In_ std::wstring uninstaller) {_uninstaller = uninstaller;	}
	void setUpdate(_In_ std::wstring update) {_update = update; }
	void setBank(_In_ std::wstring bank) { _bank = bank; }
private:
	std::wstring _id;
	std::wstring _name;
	std::wstring _vender;
	std::wstring _version;
	std::wstring _uninstaller;
	std::wstring _update;
	std::wstring _bank;
} *pblackp;


typedef class unknownp
{
public:
	unknownp(_In_ const wchar_t* id,
		_In_ const wchar_t* lastuse,
		_In_ const wchar_t* version,
		_In_ const wchar_t* cert,
		_In_ const wchar_t* uninstaller) :
		_id(id),
		_lastuse(lastuse),
		_version(version),
		_cert(cert),
		_uninstaller(uninstaller),
		_valid(TRUE)
	{}
public:
	const wchar_t* id() { return _id.c_str(); }
	const wchar_t* lastuse() { return _lastuse.c_str(); }
	const wchar_t* version() { return _version.c_str(); }
	const wchar_t* cert() { return _cert.c_str(); }
	bool isValid() { return _valid; };
	const wchar_t* uninstaller() { return _uninstaller.c_str(); }
	void setId(_In_ std::wstring id) { _id = id; }
	void setUse(_In_ std::wstring lastuse) { _lastuse = lastuse; }
	void setVersion(_In_ std::wstring version) { _version = version; }
	void setCert(_In_ std::wstring cert) { _cert = cert; }
	void setUninstaller(_In_ std::wstring uninstaller) { _uninstaller = uninstaller; }
	void setValid(_In_ bool valid) { _valid = valid; };
private:
	std::wstring _id;
	std::wstring _lastuse;
	std::wstring _version;
	std::wstring _cert;
	std::wstring _uninstaller;
	bool _valid;
} *punknownp;


