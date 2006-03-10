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

#if !defined(__INCLUDE_TankModelh_INCLUDE__)
#define __INCLUDE_TankModelh_INCLUDE__

#include <3dsparse/ModelID.h>
#include <common/Vector.h>
#include <set>
#include <string>

class TankMesh;
class TankType;
class TankModel
{
public:
	TankModel(const char *name, ModelID &modelId, TankType *type);
	virtual ~TankModel();

	virtual void draw(bool drawS, float angle, Vector &position, 
		float fireOffSet, float rotXY, float rotXZ, 
		bool absCenter = false, float scale = 1.0f, float LOD = 1.0f);
	virtual int getNoTris();

	virtual bool lessThan(TankModel *other);
	void clear();

	const char *getName() { return name_.c_str(); }
	ModelID &getTankModelID() { return modelId_; }
	ModelID &getProjectileModelID() { return projectileModelId_; }
	TankMesh *getTankMesh() { return tankMesh_; }
	TankType *getTankType() { return tankType_; }

	bool isOfCatagory(const char *catagory);
	void addCatagory(const char *catagory);

	void addTeam(int team);
	bool isOfTeam(int team);

protected:
	bool init_;
	std::string name_;
	ModelID modelId_;
	ModelID projectileModelId_;
	TankMesh *tankMesh_;
	TankType *tankType_;
	std::set<std::string> catagories_;
	std::set<int> teams_;

};

#endif
