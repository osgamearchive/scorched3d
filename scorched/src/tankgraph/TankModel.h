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

#include <tank/TankModelId.h>
#include <3dsparse/ModelID.h>
#include <tankgraph/TankMesh.h>
#include <common/Vector.h>
#include <list>
#include <string>

class TankModel
{
public:
	TankModel(TankModelId &id, ModelID &modelId);
	virtual ~TankModel();

	virtual void draw(bool drawS, float angle, Vector &position, 
		float fireOffSet, float rotXY, float rotXZ);
	virtual int getNoTris();

	virtual bool lessThan(TankModel *other);
	void clear();

	TankModelId &getId();
	ModelID &getProjectileModelID();

	bool isOfCatagory(const char *catagory);
	std::list<std::string> &getCatagories() { return catagories_; }

protected:
	bool init_;
	TankModelId id_;
	ModelID modelId_;
	ModelID projectileModelId_;
	TankMesh *tankMesh_;
	std::list<std::string> catagories_;

};

#endif
