////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2003
//
//    This file is part of Scorched3D.
//
//    Scorched3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Scorched3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Scorched3D; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////////////////////////////////////////////////////////////////////////////////


// Registry.cpp: implementation of the Registry class.
//
//////////////////////////////////////////////////////////////////////

#include <common/Registry.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Registry::Registry(LPSTR localKeyName, HKEY baseKey) : errored_(false)
{
#ifdef _WIN32
	DWORD disposition;
	LONG result = RegCreateKeyEx(
		baseKey,
		localKeyName,
		0,
		0,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		0,
		&hBaseKey_,
		&disposition);
	if (result != ERROR_SUCCESS)
	{
		errored_ = true;
	}
#endif
}

Registry::~Registry()
{
#ifdef _WIN32
	RegCloseKey(hBaseKey_);
#endif
}

LPSTR Registry::getString(LPSTR section,
						LPSTR name,
						LPSTR defaultValue)
{
#ifdef _WIN32
	LPSTR result = defaultValue;
	HKEY subKey = openOrCreateSubKey(section);

	static BYTE buffer[1024];
	DWORD type, size = 1024;
	LONG res = RegQueryValueEx(
		subKey,
		name,
		0,
		&type,
		buffer,
		&size);
	if (res == ERROR_SUCCESS && type == REG_SZ)
	{
		result = (LPSTR) buffer;
	}

	RegCloseKey(subKey);

	if (res == ERROR_FILE_NOT_FOUND && defaultValue)
	{
		setString(section, name, defaultValue);
	}

	return result;
#else
	return false;
#endif
}

bool Registry::setString(LPSTR section,
						LPSTR name,
						LPSTR value)
{
#ifdef _WIN32
	HKEY subKey = openOrCreateSubKey(section);

	bool result = false;
	LONG res = RegSetValueEx(
		subKey,
		name,
		0,
		REG_SZ,
		(LPBYTE) value,
		(DWORD) (strlen(value) + 1));
	if (res == ERROR_SUCCESS)
	{
		result = true;
	}

	RegCloseKey(subKey);
	return result;
#else
        return true;
#endif

}

DWORD Registry::getInt(LPSTR section,
						LPSTR name,
						DWORD defaultValue)
{
#ifdef _WIN32
	DWORD result = defaultValue;
	HKEY subKey = openOrCreateSubKey(section);

	static BYTE buffer[1024];
	DWORD type, size = 1024;
	LONG res = RegQueryValueEx(
		subKey,
		name,
		0,
		&type,
		buffer,
		&size);
	if (res == ERROR_SUCCESS && type == REG_DWORD)
	{
		memcpy(&result, buffer, sizeof(DWORD));
	}

	RegCloseKey(subKey);

	if (res == ERROR_FILE_NOT_FOUND && (defaultValue != ((DWORD) -1)))
	{
		setInt(section, name, defaultValue);
	}

	return result;
#else
        return false;
#endif

}

bool Registry::setInt(LPSTR section,
						LPSTR name,
						DWORD value)
{
#ifdef _WIN32
	HKEY subKey = openOrCreateSubKey(section);

	bool result = false;
	LONG res = RegSetValueEx(
		subKey,
		name,
		0,
		REG_DWORD,
		(LPBYTE) &value,
		sizeof(value));
	if (res == ERROR_SUCCESS)
	{
		result = true;
	}

	RegCloseKey(subKey);
	return result;
#else
        return true;
#endif

}

HKEY Registry::openOrCreateSubKey(LPSTR section)
{
#ifdef _WIN32
	HKEY result;
	DWORD disposition;
	RegCreateKeyEx(
		hBaseKey_,
		section,
		0,
		0,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		0,
		&result,
		&disposition);

	return result;
#else
        return true;
#endif

}
