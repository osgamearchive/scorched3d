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

#include <stdio.h>
#include <3dsparse/ASEFile.h>
#include <common/Defines.h>

extern FILE *asein;
extern int aseparse(void);
extern int aselineno;

ASEFile *ASEFile::current_ = 0;

ASEFile::ASEFile(const char *fileName) : ModelsFile(fileName)
{
	success_ = loadFile(fileName);
	if (success_)
	{
		centre();
	}
}

ASEFile::~ASEFile()
{

}

ASEFile *ASEFile::getCurrent()
{
	return current_;
}

ASEModel *ASEFile::getCurrentModel()
{
	if (!models_.empty())
	{
		return (ASEModel *) models_.back();
	}

	return 0;
}

void ASEFile::addModel(char *modelName)
{
	models_.push_back(new ASEModel(modelName));
}

bool ASEFile::loadFile(const char *fileName)
{
	asein = fopen(fileName, "r");
	if (!asein)
	{
		return false;
	}

	// Reset variables for next parser
	current_ = this;
	aselineno = 0;
	return (aseparse() == 0);
}

