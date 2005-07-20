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
#include <wx/dir.h>
#include <wx/utils.h>
#include <stdio.h>

ModFiles::ModFiles()
{
}

ModFiles::~ModFiles()
{
	std::map<std::string, ModFileEntry *>::iterator itor;
	for (itor = files_.begin();
		itor != files_.end();
		itor++)
	{
		delete (*itor).second;
	}
	files_.clear();
}

bool ModFiles::fileEnding(const char *file, const char *ext)
{
	int fileLen = strlen(file);
	int extLen = strlen(ext);
	if (fileLen < extLen) return false;

	for (int i=0; i<extLen; i++)
	{
		if (file[fileLen - i - 1] != ext[extLen - i - 1])
		{
			return false;
		}
	}
	return true;
}

bool ModFiles::excludeFile(const char *file)
{
	if (strstr(file, "Thumbs.db")) return true;
	if (strstr(file, "CVS")) return true;
	if (strstr(file, "..")) return true;

	if (!fileEnding(file, ".xml") &&
		!fileEnding(file, ".bmp") &&
		!fileEnding(file, ".txt") &&
		!fileEnding(file, ".gif") &&
		!fileEnding(file, ".ico") &&
		!fileEnding(file, ".ase") &&
		!fileEnding(file, ".wav"))
	{
		Logger::log("Excluding mod file \"%s\"", file);
		return true;
	}

	return false;
}

bool ModFiles::loadModFiles(const char *mod, bool createDir)
{
	{
		// Get and check the user mod directory exists
		const char *modDir = getModFile(mod);
		if (::wxDirExists(modDir))
		{
			if (!loadModDir(modDir, mod)) return false;
		}
		else
		{
			if (createDir) ::wxMkdir(modDir);
		}
	}

	{
		// Get and check global mod directory
		const char *modDir = getGlobalModFile(mod);
		if (::wxDirExists(modDir))
		{
			if (!loadModDir(modDir, mod)) return false;
		}
	}

	// For the default "none" mod load some files that can
	// be downloaded
	if (0 == strcmp("none", mod))
	{
		std::string modDir = getDataFile("");
		loadModFile(getDataFile("data/accessories.xml"), modDir.c_str(), mod);
		loadModFile(getDataFile("data/landscapes.xml"), modDir.c_str(), mod);
		loadModFile(getDataFile("data/landscapesdefn.xml"), modDir.c_str(), mod);
		loadModFile(getDataFile("data/landscapestex.xml"), modDir.c_str(), mod);
	}
	
	{
		unsigned int totalCompSize = 0, totalSize = 0;
		std::map<std::string, ModFileEntry *>::iterator itor;
		for (itor = files_.begin();
			itor != files_.end();
			itor++)
		{
			ModFileEntry *entry = (*itor).second;
			totalCompSize += entry->getCompressedSize();
			totalSize += entry->getUncompressedSize();
		}

		Logger::log( "Loaded mod \"%s\", space required %u (%u) bytes", 
			mod, totalCompSize, totalSize);

		if (!createDir && files_.empty())
		{
			dialogMessage("Mod",
				"Failed to find \"%s\" mod files in directories \"%s\" \"%s\"",
				mod,
				getModFile(mod),
				getGlobalModFile(mod));
			return false;
		}
	}

	return true;
}

bool ModFiles::loadModDir(const char *modDir, const char *mod)
{
	// Load all files contained in this directory
	wxArrayString files;
	wxDir::GetAllFiles(modDir, &files);
	wxString *strings = files.GetStringArray();
	for (int i=0; i<(int) files.Count(); i++)
	{
		// Get the name of the current file
		wxString &current = strings[i];
		const char *fullFileName = current.c_str();

		// Load the file
		if (!loadModFile(fullFileName, modDir, mod))
		{
			return false;
		}
	}

	return true;
}

bool ModFiles::loadModFile(const char *fullFileName,
	const char *modDir, const char *mod)
{
	std::string shortFileNameStr(fullFileName);
	const char *shortFileName = shortFileNameStr.c_str();

	// Check to see if we ignore this file
	if (excludeFile(fullFileName)) return true;

	// Turn it into a unix style path and remove the 
	// name of the directory that contains it
	int modDirLen = strlen(modDir);
	shortFileName += modDirLen;
	::wxDos2UnixFilename((char *) shortFileName);
	while (shortFileName[0] == '/') shortFileName++;

	// Check that all files are lower case
	std::string oldFileName = shortFileName;
	_strlwr((char *) shortFileName);
	if (strcmp(oldFileName.c_str(), shortFileName) != 0)
	{
		dialogMessage("Mod",
			"ERROR: All mod files must have lower case filenames.\n"
			"File \"%s,%s\" has upper case charaters in it",
			oldFileName.c_str(),
			shortFileName);
		return false;
	}

	// Check we don't have this file already
	if (files_.find(shortFileName) != files_.end()) return true;

	// Create the new mod file and load the file
	ModFileEntry *file = new ModFileEntry();
	if (!file->loadModFile(fullFileName))
	{
		dialogMessage("Mod",
			"Error: Failed to load file \"%s\" mod directory \"%s\" in the \"%s\" mod",
			fullFileName,
			modDir,
			mod);
		return false;
	}

	// Store for future
	file->setFileName(shortFileName);
	files_[shortFileName] = file;
	return true;
}

bool ModFiles::writeModFiles(const char *mod)
{
	const char *modDir = getModFile(mod);
	if (!::wxDirExists(modDir))
	{
		::wxMkdir(modDir);
	}

	std::map<std::string, ModFileEntry *>::iterator itor;
	for (itor = files_.begin();
		itor != files_.end();
		itor++)
	{
		ModFileEntry *entry = (*itor).second;
		if (!entry->writeModFile(entry->getFileName(), mod)) return false;
	}
	return true;
}

void ModFiles::clearData()
{
	 std::map<std::string, ModFileEntry *>::iterator itor;
	 for (itor = files_.begin();
	 	itor != files_.end();
		itor++)
	{
		 ModFileEntry *entry = (*itor).second;
		 entry->getCompressedBuffer().clear();
	}
}

bool ModFiles::exportModFiles(const char *mod, const char *fileName)
{
	FILE *out = fopen(fileName, "wb");
	if (!out) return false;

	// Mod Name
	NetBuffer tmpBuffer;
	tmpBuffer.reset();
	tmpBuffer.addToBuffer(ScorchedProtocolVersion);
	tmpBuffer.addToBuffer(mod);
	fwrite(tmpBuffer.getBuffer(),
		sizeof(unsigned char),
		tmpBuffer.getBufferUsed(), 
		out);	

	// Mod Files
	std::map<std::string, ModFileEntry *>::iterator itor;
	for (itor = files_.begin();
		itor != files_.end();
		itor++)
	{
		ModFileEntry *entry = (*itor).second;
	
		tmpBuffer.reset();
		tmpBuffer.addToBuffer(entry->getFileName());
		tmpBuffer.addToBuffer(entry->getUncompressedSize());
		tmpBuffer.addToBuffer(entry->getCompressedBuffer().getBufferUsed());
		tmpBuffer.addToBuffer(entry->getCompressedCrc());
		tmpBuffer.addDataToBuffer(entry->getCompressedBuffer().getBuffer(),
			entry->getCompressedBuffer().getBufferUsed());

		fwrite(tmpBuffer.getBuffer(),
			sizeof(unsigned char),
			tmpBuffer.getBufferUsed(), 
			out);
	}

	// End of Mod Files
	tmpBuffer.reset();
	tmpBuffer.addToBuffer("*");
	fwrite(tmpBuffer.getBuffer(),
		sizeof(unsigned char),
		tmpBuffer.getBufferUsed(), 
		out);

	fclose(out);
	return true;
}

bool ModFiles::importModFiles(const char **mod, const char *fileName)
{
	FILE *in = fopen(fileName, "rb");
	if (!in) return false;

	// Read Buffer
	NetBuffer tmpBuffer;
	unsigned char readBuf[512];
	while (unsigned int size = fread(readBuf, sizeof(unsigned char), 512, in))
	{
		tmpBuffer.addDataToBuffer(readBuf, size);
	}

	// Mod Name
	static std::string modName;
	std::string version;
	NetBufferReader tmpReader(tmpBuffer);
	if (!tmpReader.getFromBuffer(version)) return false;
	if (!tmpReader.getFromBuffer(modName)) return false;
	*mod = modName.c_str();

	if (0 != strcmp(version.c_str(), ScorchedProtocolVersion))
	{
		dialogMessage("Scorched3D",
			"Failed to import mod, scorched version differs.\n"
			"Please obtain a newer version of this mod.\n"
			"Import version = %s\n"
			"Current version = %s\n",
			version.c_str(),
			ScorchedProtocolVersion);
		return false;
	}

	for (;;)
	{
		// File Name
		std::string name;
		if (!tmpReader.getFromBuffer(name)) return false;
		if (0 == strcmp(name.c_str(), "*")) break;

		// File Header
		unsigned int unCompressedSize;
		unsigned int compressedSize;
		unsigned int compressedCrc;
		tmpReader.getFromBuffer(unCompressedSize);
		tmpReader.getFromBuffer(compressedSize);
		tmpReader.getFromBuffer(compressedCrc);

		// File
		ModFileEntry *entry = new ModFileEntry;
		entry->setFileName(name.c_str());
		entry->setCompressedCrc(compressedCrc);
		entry->setUncompressedSize(unCompressedSize);
		entry->getCompressedBuffer().allocate(compressedSize);
		entry->getCompressedBuffer().setBufferUsed(compressedSize);
		tmpReader.getDataFromBuffer(
			entry->getCompressedBuffer().getBuffer(), 
			compressedSize);
		files_[name] = entry;
	}

	fclose(in);
	return true;
}

ModDirs::ModDirs()
{
}

ModDirs::~ModDirs()
{
}

bool ModDirs::loadModDirs()
{
	dirs_.push_back("none");
	if (!loadModDir(getModFile(""))) return false;
	if (!loadModDir(getGlobalModFile(""))) return false;
	return true;
}
	
bool ModDirs::loadModDir(const char *dirName)
{
	wxDir dir(dirName);
	if (dir.IsOpened())
	{
		wxString filename;
		bool cont = dir.GetFirst(&filename, "", wxDIR_DIRS);
		while (cont)
		{
			if (!loadModFile(filename.c_str())) return false;
			cont = dir.GetNext(&filename);
		}
	}
	return true;
}

bool ModDirs::loadModFile(const char *fileName)
{
	if (strstr(fileName, "CVS")) return true;

	std::string oldFileName(fileName);
	_strlwr((char *) fileName);

	if (strcmp(oldFileName.c_str(), fileName) != 0)
	{
		dialogMessage("ModDirs",
			"ERROR: All mod directories must have lower case filenames.\n"
			"Directory \"%s\" has upper case charaters in it",
			fileName);
		return false;
	}
	
	std::list<std::string>::iterator itor;
	for (itor = dirs_.begin();
		itor != dirs_.end();
		itor++)
	{
		std::string name = (*itor);
		if (0 == strcmp(name.c_str(), fileName)) return true;
	}
	dirs_.push_back(fileName);
	
	return true;
}
