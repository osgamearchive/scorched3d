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


// FileLines.cpp: implementation of the FileLines class.
//
//////////////////////////////////////////////////////////////////////

#include <common/FileLines.h>
#include <stdio.h>
#include <stdarg.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FileLines::FileLines()
{

}

FileLines::~FileLines()
{

}

const char *FileLines::getRandomLine()
{
	const char *result = 0;

	int count = (int) fileLines_.size() - 1;;
	int i = int((float(rand()) / float(RAND_MAX)) * count);
	result = fileLines_[i].c_str();

	return result;
}

bool FileLines::readFile(char *filename)
{
	FILE *in = fopen(filename, "r");
	if (in)
	{
		char buffer[2048];
		while (fgets(buffer, 100, in))
		{
			char *nl = strchr(buffer, '\n');
			if (nl) *nl = '\0';
			fileLines_.push_back(buffer);
		}
		fclose(in);
	}
	else
	{
		return false;
	}
	
	return true;
}

bool FileLines::writeFile(char *filename)
{
	FILE *out = fopen(filename, "w");
	if (!out) return false;

	std::vector<std::string>::iterator itor;
	for (itor = fileLines_.begin();
		itor != fileLines_.end();
		itor++)
	{
		fprintf(out, "%s\n", (*itor).c_str());
	}

	fclose(out);
	return true;
}

void FileLines::addLine(const char *fmt, ...)
{
	static char text[2048];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	fileLines_.push_back(text);
}

void FileLines::getAsOneLine(std::string &output)
{
	std::vector<std::string>::iterator itor;
	for (itor = fileLines_.begin();
		 itor != fileLines_.end();
		 itor++)
	{
		output += (*itor);
		output += "\n";
	}
}
