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

#include <3dsparse/ModelsFile.h>
#include <common/Defines.h>

ModelsFile::ModelsFile(const char *fileName)  : name_(fileName)
{

}

ModelsFile::~ModelsFile()
{
	std::list<Model *>::iterator itor;
	for (itor = models_.begin();
		itor != models_.end();
		itor++)
	{
		delete (*itor);
	}
	models_.clear();
}

int ModelsFile::getNumberFaces()
{
	int faces = 0;
	std::list<Model *>::iterator itor;
	for (itor = models_.begin();
		itor != models_.end();
		itor++)
	{
		faces += (int) (*itor)->getFaces().size();
	}
	return faces;
}

void ModelsFile::scale(float sfactor)
{
	std::list<Model *>::iterator itor;
	for (itor = models_.begin();
		itor != models_.end();
		itor++)
	{
		(*itor)->scale(sfactor);
	}
	min_ *= sfactor;
	max_ *= sfactor;
}

void ModelsFile::centre()
{
	if (!models_.empty())
	{
		max_ = models_.front()->getMax();
		min_ = models_.front()->getMin();

		std::list<Model *>::iterator itor;
		for (itor = models_.begin();
			itor != models_.end();
			itor++)
		{
			max_[0] = MAX(max_[0], (*itor)->getMax()[0]);
			max_[1] = MAX(max_[1], (*itor)->getMax()[1]);
			max_[2] = MAX(max_[2], (*itor)->getMax()[2]);

			min_[0] = MIN(min_[0], (*itor)->getMin()[0]);
			min_[1] = MIN(min_[1], (*itor)->getMin()[1]);
			min_[2] = MIN(min_[2], (*itor)->getMin()[2]);
		}


		Vector centre = (max_ + min_) / 2.0f;
		for (itor = models_.begin();
			itor != models_.end();
			itor++)
		{
			(*itor)->centre(centre);
		}

		min_ -= centre;
		max_ -= centre;
	}
}

void ModelsFile::centreBottom()
{
	if (!models_.empty())
	{
		centre();

		Vector newMin(0.0f, 0.0f, min_[2]);

		std::list<Model *>::iterator itor;
		for (itor = models_.begin();
			itor != models_.end();
			itor++)
		{
			(*itor)->centre(newMin);
		}

		max_ -= newMin;
		min_ -= newMin;
	}
}
