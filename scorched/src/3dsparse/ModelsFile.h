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

#if !defined(__INCLUDE_ModelsFileh_INCLUDE__)
#define __INCLUDE_ModelsFileh_INCLUDE__

#include <list>
#include <string>
#include <3dsparse/Model.h>

class ModelsFile
{
public:
	ModelsFile(const char *fileName);
	virtual ~ModelsFile();

	bool getSuccess() { return success_; }
	const char *getName() { return name_.c_str(); }

	Vector &getMax() { return max_; }
	Vector &getMin() { return min_; }
	int getNumberFaces();
	std::list<Model *> &getModels() { return models_; }

	void centre();
	void centreBottom();
	void scale(float scalef);

protected:
	bool success_;
	std::list<Model *> models_;
	std::string name_;
	Vector max_, min_;

};

#endif
