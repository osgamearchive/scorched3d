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

#include <engine/ModFiles.h>
#include <common/Defines.h>
#include <common/Logger.h>
#include <zlib/zlib.h>
#include <wx/dir.h>
#include <wx/utils.h>

ModFile::ModFile() : crc_(0)
{
}

ModFile::~ModFile()
{
}

bool ModFile::loadModFile(const char *filename)
{
	NetBuffer fileContents;
	{
		// Load the file into a coms buffer
		FILE *file = fopen(filename, "rb");
		if (!file) return false;
		int newSize = 0;
		unsigned char buffer[256];
		do
		{
			newSize = fread(buffer, sizeof(unsigned char), sizeof(buffer), file);
			fileContents.addDataToBuffer(buffer, newSize);
		}
		while (newSize > 0);
		fclose(file);
	}

	if (fileContents.getBufferUsed() > 0)
	{
		// Allocate the needed space for the compressed file
		unsigned long destLen = fileContents.getBufferUsed() + 100;
		file_.allocate(destLen);
		file_.reset();

		// Compress the file into the new buffer
		if (compress2(
				(unsigned char *) file_.getBuffer(), &destLen, 
				(unsigned char *) fileContents.getBuffer(), fileContents.getBufferUsed(), 
				6) != Z_OK)
		{
			return false;
		}
		file_.setBufferUsed(destLen);

		// Get the crc for the new file
		crc_ =  crc32(0L, Z_NULL, 0);
		crc_ = crc32(crc_, (unsigned char *) file_.getBuffer(), file_.getBufferUsed());
	}

	return true;
}

ModFiles::ModFiles()
{
}

ModFiles::~ModFiles()
{
}

bool ModFiles::loadModFiles(const char *mod)
{
	if (!mod || !mod[0]) return true;

	// Get and check the mod directory exists
	const char *modDir = getModFile(mod);
	int modDirLen = strlen(modDir);
	if (!::wxDirExists(modDir))
	{
		dialogMessage("Mod",
			"Error: Failed to find \"%s\" mod directory \"%s\"",
			mod,
			modDir);
		return false;
	}

	// Load all files contained in this directory
	unsigned int totalSize = 0;
	wxArrayString files;
	wxDir::GetAllFiles(modDir, &files);
	wxString *strings = files.GetStringArray();
	for (int i=0; i<(int) files.Count(); i++)
	{
		// Get the name of the current file
		wxString &current = strings[i];
		const char *fileName = current.c_str();

		// Create the new mod file and load the file
		ModFile *file = new ModFile();
		if (!file->loadModFile(fileName))
		{
			dialogMessage("Mod",
				"Error: Failed to load file \"%s\" mod directory \"%s\" in the \"%s\" mod",
				fileName,
				modDir,
				mod);
			return false;
		}

		// Turn it into a unix style path and remove the 
		// name of the directory that contains it
		fileName += modDirLen;
		::wxDos2UnixFilename((char *) fileName);
		while (fileName[0] == '/') fileName++;

		// Store for future
		files_[fileName] = file;
		totalSize += file->getFileSize();
	}

	Logger::log(0, "Loaded mod \"%s\", space required %u bytes", 
		mod, totalSize);

	return true;
}
