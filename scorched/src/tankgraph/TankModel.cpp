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
#include <tankgraph/TankModelStore.h>
#include <common/Defines.h>

TankModel::TankModel(TankModelId &id,
					 const char *meshName, const char *skinName) :
	id_(id),
	meshName_(meshName), skinName_(skinName?skinName:""),
	tankMesh_(0), tankSkin_(0), init_(false)
{
	_strlwr((char *) meshName_.c_str());
	_strlwr((char *) skinName_.c_str());
}

TankModel::~TankModel()
{
}

void TankModel::draw(bool drawS, float angle, 
	Vector &position, float fireOffSet, 
	float rotXY, float rotXZ)
{
	if (!init_)
	{
		init_ = true;
		tankMesh_ = TankModelStore::instance()->loadMesh(meshName_.c_str());
		if (!tankMesh_)
		{
			dialogMessage("Scorched3D", "ERROR: Failed to load mesh \"%s\"",
				meshName_.c_str());
			return;
		}

		if (skinName_.c_str()[0])
		{
			tankSkin_ = TankModelStore::instance()->loadTexture(skinName_.c_str());
			if (!tankSkin_)
			{
				dialogMessage("Scorched3D", "ERROR: Failed to load skin \"%s\"",
					skinName_.c_str());
				return;
			}
		}
	}

	GLState *currentState = 0;
	if (tankSkin_)
	{
		currentState = new GLState(GLState::TEXTURE_ON);
		tankSkin_->draw();
	}
	else
	{
		currentState = new GLState(GLState::TEXTURE_OFF);
	}

	if (tankMesh_)
	{
		tankMesh_->draw(drawS, angle, position, fireOffSet, rotXY, rotXZ);
	}
	delete currentState;
}

float TankModel::getTurretHeight()
{ 
	if (!tankMesh_) return 0.0f;
	return tankMesh_->getTurretHeight(); 
}

TankModelId &TankModel::getId() 
{ 
	return id_; 
}

int TankModel::getNoTris()
{ 
	if (!tankMesh_) return 0;
	return tankMesh_->getNoTris(); 
}
