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


// ASEFile.cpp: implementation of the ASEFile class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <3dsparse/ASEFile.h>
#include <common/Defines.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern FILE *asein;
extern int aseparse(void);
extern int aselineno;

ASEFile *ASEFile::current_ = 0;

ASEFile::ASEFile(const char *fileName) : name_(fileName)
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
		return &models_.back();
	}

	return 0;
}

void ASEFile::addModel(char *modelName)
{
	models_.push_back(ASEModel(modelName));
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

void ASEFile::scale(float sfactor)
{
	std::list<ASEModel>::iterator itor;
	for (itor = models_.begin();
		itor != models_.end();
		itor++)
	{
		itor->scale(sfactor);
	}
	min_ *= sfactor;
	max_ *= sfactor;
}

void ASEFile::centre()
{
	if (!models_.empty())
	{
		max_ = models_.front().getMax();
		min_ = models_.front().getMin();

		std::list<ASEModel>::iterator itor;
		for (itor = models_.begin();
			itor != models_.end();
			itor++)
		{
			max_[0] = MAX(max_[0], itor->getMax()[0]);
			max_[1] = MAX(max_[1], itor->getMax()[1]);
			max_[2] = MAX(max_[2], itor->getMax()[2]);

			min_[0] = MIN(min_[0], itor->getMin()[0]);
			min_[1] = MIN(min_[1], itor->getMin()[1]);
			min_[2] = MIN(min_[2], itor->getMin()[2]);
		}


		Vector centre = (max_ + min_) / 2.0f;
		for (itor = models_.begin();
			itor != models_.end();
			itor++)
		{
			itor->centre(centre);
		}

		min_ -= centre;
		max_ -= centre;
	}
}
