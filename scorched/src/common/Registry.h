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


// Registry.h: interface for the Registry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__INCLUDE_Registryh_INCLUDE__)
#define __INCLUDE_Registryh_INCLUDE__

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

class Registry  
{
public:
	Registry(LPSTR localKeyName, HKEY baseKey = HKEY_CURRENT_USER);
	virtual ~Registry();

	LPSTR getString(
		LPSTR section,
		LPSTR name,
		LPSTR defaultValue = 0);
	bool setString(
		LPSTR section,
		LPSTR name,
		LPSTR value);

	DWORD getInt(
		LPSTR section,
		LPSTR name,
		DWORD defaultValue = -1);
	bool setInt(
		LPSTR section,
		LPSTR name,
		DWORD value);

protected:
	HKEY hBaseKey_;
	bool errored_;

	HKEY openOrCreateSubKey(LPSTR section);

private:
	Registry(const Registry &);
	Registry &operator=(const Registry&);

};

#endif // __INCLUDE_Registryh_INCLUDE__
