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

#ifndef _tankModelStore_h
#define _tankModelStore_h

#include <vector>
#include <map>
#include <tankgraph/TankModel.h>
#include <tankgraph/TankMesh.h>
#include <GLEXT/GLTexture.h>

class TankModelStore
{
public:
	static TankModelStore* instance();

	bool loadTankMeshes();

	TankModel *getModelByName(const char *name);
	TankModel *getRandomModel();
	std::vector<TankModel *> &getModels() { return models_; }

	GLTexture *loadTexture(const char *name);
	TankMesh *loadMesh(const char *name, bool aseFile);

protected:
	static TankModelStore* instance_;
	std::vector<TankModel *> models_;
	std::map<std::string, TankMesh *> meshes_;
	std::map<std::string, GLTexture *> skins_;

private:
	TankModelStore();
	virtual ~TankModelStore();

	TankModelStore(const TankModelStore &);
	const TankModelStore & operator=(const TankModelStore &);

};

#endif // _tankModelStore_h

