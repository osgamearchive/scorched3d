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


// FileList.cpp: implementation of the FileList class.
//
//////////////////////////////////////////////////////////////////////

#ifndef WIN32
#include <fnmatch.h>
#include <dirent.h>
#else
#define WIN32_LEAN_AND_MEAN	
#include <windows.h>
#endif
#include <common/FileList.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FileList::FileList(char *directory, char *filter) 
	: directory_(directory), filter_(filter)
{
	status_ = readFiles();
}

FileList::~FileList()
{

}

bool FileList::getStatus()
{
	return status_;
}

bool FileList::readFiles()
{
#ifndef WIN32
	DIR *dirp;
   	struct dirent *direntp;
   	dirp = opendir( directory_.c_str());

	while ( (direntp = readdir( dirp )) != NULL )
   	{
		if (fnmatch(filter_.c_str(), (const char *)direntp->d_name,0))
			continue;
		
		files_.push_back(directory_ + "/" + std::string(direntp->d_name));

   	}

	closedir( dirp );
#else
	WIN32_FIND_DATA findFileData;
	HANDLE han = INVALID_HANDLE_VALUE;
	ZeroMemory(&findFileData, sizeof(findFileData));
	std::string fullName = directory_ + "/" + filter_;

	han = FindFirstFile(fullName.c_str(), &findFileData);
	if (han == INVALID_HANDLE_VALUE) return false;

	files_.push_back(directory_ + "/" + std::string(findFileData.cFileName));
	while (FindNextFile(han, &findFileData))
	{
		files_.push_back(directory_ + "/" + std::string(findFileData.cFileName));
	}
	FindClose(han);
#endif

	return true;
}
