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


// ASEFile.h: interface for the ASEFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASEFILE_H__D1A8093E_9A49_46F5_A38F_FC3F733E568E__INCLUDED_)
#define AFX_ASEFILE_H__D1A8093E_9A49_46F5_A38F_FC3F733E568E__INCLUDED_


#include <list>
#include <string>
#include <3dsparse/ASEModel.h>

class ASEFile  
{
public:
	ASEFile(const char *fileName);
	virtual ~ASEFile();

	bool getSuccess() { return success_; }

	Vector &getMax() { return max_; }
	Vector &getMin() { return min_; }
	std::list<ASEModel> &getModels() { return models_; }

	void centre();
	void scale(float scalef);

	// Used by parser
	static ASEFile *getCurrent();
	void addModel(char *modelName);
	ASEModel *getCurrentModel();
	const char *getName() { return name_.c_str(); }

protected:
	static ASEFile *current_;
	bool success_;
	std::list<ASEModel> models_;
	std::string name_;
	Vector max_, min_;
	
	bool loadFile(const char *fileName);

};

#endif // !defined(AFX_ASEFILE_H__D1A8093E_9A49_46F5_A38F_FC3F733E568E__INCLUDED_)
