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

#include <sprites/MissileActionRenderer.h>
#include <3dsparse/ModelsFile.h>
#include <actions/ShotProjectile.h>
#include <landscape/Landscape.h>
#include <common/OptionsDisplay.h>
#include <tankgraph/TankModelRenderer.h>
#include <engine/ScorchedContext.h>

std::map<std::string, MissileMesh *> MissileActionRenderer::loadedMeshes_;

MissileActionRenderer::MissileActionRenderer(int flareType, float scale) : 
	flareType_(flareType), counter_(0.1f, 0.1f), mesh_(0), scale_(scale), rotation_(0.0f)
{

}

MissileActionRenderer::MissileActionRenderer(int flareType) : 
	flareType_(flareType), counter_(0.1f, 0.1f), mesh_(0), scale_(1.0f), rotation_(0.0f)
{

}

MissileActionRenderer::~MissileActionRenderer()
{
}

void MissileActionRenderer::simulate(Action *action, float timepassed, bool &remove)
{
	if (counter_.nextDraw(timepassed))
	{
		Vector &actualPos = ((PhysicsParticle *)action)->getCurrentPosition();
		Landscape::instance()->getSmoke().
			addSmoke(actualPos[0], actualPos[1], actualPos[2], 
				0.0f, 0.0f, 0.0f, 0.7f);
	}
	rotation_ += ((PhysicsParticle *)action)->getCurrentVelocity().Magnitude();
}

void MissileActionRenderer::draw(Action *action)
{
	ShotProjectile *shot = (ShotProjectile *) action;

	// Do we have a loaded mesh
	if (!mesh_)
	{
		// Set the default model to use if neither the tank
		// or weapon have one
		static ModelID defaultModelId;
		if (!defaultModelId.modelValid())
		{
			defaultModelId.initFromString(
				"MilkShape",
				"data/accessories/v2missile/v2missile.txt",
				"");
		}

		// Set the model to use as the default model id
		ModelID *usedModelId = &defaultModelId;

		// Get the model to use from the weapon (if there is one)
		if (shot->getWeapon()->getModelID().modelValid())
		{
			usedModelId = &shot->getWeapon()->getModelID();
		}
		else
		{
			// The weapon does not have a model defined for it
			// check the player to see if they have a default model
			Tank *currentPlayer = action->getScorchedContext()->tankContainer.getTankById(
				shot->getPlayerId());
			if (currentPlayer)
			{
				TankModelRenderer *model = (TankModelRenderer *) 
						currentPlayer->getModel().getModelIdRenderer();
				if (model && 
					model->getModel()->getProjectileModelID().modelValid())
				{
					usedModelId = &model->getModel()->getProjectileModelID();
				}
			}
		}

		// Load or find the correct missile mesh
		const char *name = usedModelId->getStringHash();
		std::map<std::string, MissileMesh *>::iterator itor =
			loadedMeshes_.find(name);
		if (itor == loadedMeshes_.end())
		{
			ModelsFile *newFile = usedModelId->getModelsFile();
			if (!newFile) return;

			mesh_ = new MissileMesh(*newFile, 
				!OptionsDisplay::instance()->getNoSkins(), 1.0f);
			loadedMeshes_[name] = mesh_;
		}
		else
		{
			// Find
			mesh_ = (*itor).second;
		}
	}

	Vector &actualPos = shot->getCurrentPosition();
	Vector &actualdir = shot->getCurrentVelocity();

	mesh_->setScale(scale_);
	mesh_->draw(actualPos, actualdir, flareType_, rotation_);
}
