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

#if !defined(__INCLUDE_ModFilesh_INCLUDE__)
#define __INCLUDE_ModFilesh_INCLUDE__

#include <coms/NetBuffer.h>
#include <map>
#include <string>

struct ModIdentifierEntry
{
	ModIdentifierEntry(const char *f = "",
		unsigned int l = 0,
		unsigned int c = 0) :
		fileName(f),
		length(l),
		crc(c)
	{
	};

	std::string fileName;
	unsigned int length;
	unsigned int crc;
};

class ModFileEntry
{
public:
	ModFileEntry();
	virtual ~ModFileEntry();

	bool loadModFile(const char *file);
	bool writeModFile(const char *file);

	void setFileName(const char *name) { fileName_ = name; }
	const char *getFileName() { return fileName_.c_str(); }

	unsigned int getCompressedCrc() { return compressedcrc_; }
	void setCompressedCrc(unsigned int c) { compressedcrc_ = c; }

	unsigned int getCompressedSize() { return compressedfile_.getBufferUsed(); }
	char *getCompressedBytes() { return compressedfile_.getBuffer(); }

	unsigned int getUncompressedSize() { return uncompressedSize_; }
	void setUncompressedSize(unsigned int s) { uncompressedSize_ = s; }

	NetBuffer &getCompressedBuffer() { return compressedfile_; }

protected:
	std::string fileName_;
	NetBuffer compressedfile_;
	unsigned int compressedcrc_;
	unsigned int uncompressedSize_;
};

class ModFiles
{
public:
	ModFiles();
	virtual ~ModFiles();

	bool loadModFiles(const char *mod);
	void clearData();

	std::map<std::string, ModFileEntry *> &getFiles() { return files_; }

protected:
	std::map<std::string, ModFileEntry *> files_;

};

#endif

