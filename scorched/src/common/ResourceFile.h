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

#ifndef _RESOURCEFILE_H_
#define _RESOURCEFILE_H_

#include <common/Vector.h>
#include <string>
#include <map>

class XMLNode;
class ResourceFile  
{
public:
	ResourceFile();
	virtual ~ResourceFile();

	bool initFromFile(const char *fileName);

	void setRandomModule();
	bool setModule(const char *name);

	Vector *getVectorResource(const char *name);
	const char *getStringResource(const char *name);

protected:
	struct ResourceModule
	{
		ResourceModule(const char *name, 
			ResourceModule *i) : moduleName(name), inherits(i) { };

		ResourceModule *inherits;
		std::string moduleName;
		std::map<std::string, void *> moduleValues;
	};

	ResourceModule *currentModule_;
	std::map<std::string, ResourceModule *> modules_;
	void *getResource(ResourceModule *module, const char *name);
	void *parseType(const char *type, 
		const char *fileName,
		 XMLNode *moduleNode,
		 XMLNode *valueNode);

};

#endif /* _RESOURCEFILE_H_ */
