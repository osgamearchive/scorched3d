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

#if !defined(__INCLUDE_ModelStoreh_INCLUDE__)
#define __INCLUDE_ModelStoreh_INCLUDE__

#include <3dsparse/ModelID.h>
#include <GLEXT/GLVertexSet.h>
#include <GLEXT/GLTexture.h>
#include <map>
#include <string>

class ModelStore
{
public:
	static ModelStore *instance();

	GLVertexSet *loadOrGetArray(ModelID &model,
		bool usetextures = false);
	GLTexture *loadTexture(const char *name, 
		const char *aname, bool invert = false);

protected:
	static ModelStore *instance_;
	std::map<std::string, GLVertexSet *> fileMap_;
	std::map<std::string, GLTexture *> skins_;

private:
	ModelStore();
	virtual ~ModelStore();
};

#endif
