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
#include <common/OptionsDisplay.h>
#include <3dsparse/ModelsFile.h>

TankModel::TankModel(TankModelId &id, ModelID &modelId) :
	init_(false),
	id_(id), modelId_(modelId)
{
	catagories_.push_back("All");
}

TankModel::~TankModel()
{
}

TankModelId &TankModel::getId() 
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
	float rotXY, float rotXZ, bool absCenter, float scale)
{
	if (!init_)
	{
		init_ = true;

		ModelsFile *newFile = modelId_.getModelsFile();
		if (!newFile) return;

		// Get the model detail
		float detail = 
			float(OptionsDisplay::instance()->getMaxModelTriPercentage()) / 100.0f;

		// Create tank mesh
		tankMesh_ = new TankMesh(
			*newFile, !OptionsDisplay::instance()->getNoSkins(), detail);
	}

	if (tankMesh_)
	{
		tankMesh_->draw(drawS, angle, position, fireOffSet, 
			rotXY, rotXZ, absCenter, scale);
	}
}

int TankModel::getNoTris()
{ 
	if (!tankMesh_) return 0;
	return tankMesh_->getNoTris(); 
}

bool TankModel::lessThan(TankModel *other)
{
	return (strcmp(id_.getModelName(), other->id_.getModelName()) < 0);
}

bool TankModel::isOfCatagory(const char *catagory)
{
	std::list<std::string>::iterator itor;
	for (itor = catagories_.begin();
		 itor != catagories_.end();
		 itor++)
	{
		if ((*itor) == catagory) return true;
	}
	return false;
}
