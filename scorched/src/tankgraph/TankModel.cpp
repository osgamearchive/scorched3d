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

#include <tankgraph/TankModel.h>
#include <tankgraph/TankMesh.h>
#include <3dsparse/ModelStore.h>
#include <3dsparse/Model.h>
#include <tank/TankType.h>

TankModel::TankModel(TargetModelId &id, ModelID &modelId, TankType *tankType) :
	init_(false),
	id_(id), modelId_(modelId), tankMesh_(0), tankType_(tankType)
{
	catagories_.insert("All");
}

TankModel::~TankModel()
{
}

TargetModelId &TankModel::getId() 
{ 
	return id_; 
}

ModelID &TankModel::getTankModelID()
{
	return modelId_;
}

ModelID &TankModel::getProjectileModelID()
{
	return projectileModelId_;
}

void TankModel::clear()
{
	init_ = false;
	delete tankMesh_;
	tankMesh_ = 0;
}

void TankModel::draw(bool drawS, float angle, 
	Vector &position, float fireOffSet, 
	float rotXY, float rotXZ, bool absCenter, float scale, float LOD)
{
	if (!init_)
	{
		init_ = true;

		Model *newFile = ModelStore::instance()->loadModel(modelId_);
		if (!newFile) return;

		// Create tank mesh
		tankMesh_ = new TankMesh(*newFile);
	}

	if (tankMesh_)
	{
		tankMesh_->draw(drawS, angle, position, fireOffSet, 
			rotXY, rotXZ, absCenter, scale, LOD);
	}
}

int TankModel::getNoTris()
{ 
	if (!tankMesh_) return 0;
	return tankMesh_->getNoTris(); 
}

bool TankModel::lessThan(TankModel *other)
{
	return (strcmp(id_.getTankModelName(), other->id_.getTankModelName()) < 0);
}

void TankModel::addCatagory(const char *catagory)
{
	catagories_.insert(catagory);
}

bool TankModel::isOfCatagory(const char *catagory)
{
	std::set<std::string>::iterator itor =
		catagories_.find(catagory);
	return (itor != catagories_.end());
}

void TankModel::addTeam(int team)
{
	teams_.insert(team);
}

bool TankModel::isOfTeam(int team)
{
	if (team == 0) return true; // No Team
	if (teams_.empty()) return true; // Tank not in a team
	std::set<int>::iterator itor =
		teams_.find(team);
	return (itor != teams_.end());
}
