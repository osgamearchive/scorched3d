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

std::map<std::string, MissileMesh *> MissileActionRenderer::loadedMeshes_;

MissileActionRenderer::MissileActionRenderer(int flareType, float scale) : 
	flareType_(flareType), counter_(0.1f, 0.1f), mesh_(0), scale_(scale)
{

}

MissileActionRenderer::MissileActionRenderer(int flareType) : 
	flareType_(flareType), counter_(0.1f, 0.1f), mesh_(0)
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
}

void MissileActionRenderer::draw(Action *action)
{
	ShotProjectile *shot = (ShotProjectile *) action;

	// Do we have a loaded mesh
	if (!mesh_)
	{
		// Load or find the correct missile mesh
		const char *name = shot->getWeapon()->getModelID().getStringHash();
		std::map<std::string, MissileMesh *>::iterator itor =
			loadedMeshes_.find(name);
		if (itor == loadedMeshes_.end())
		{
			ModelsFile *newFile = shot->getWeapon()->getModelID().getNewFile();
			if (!newFile) return;

			mesh_ = new MissileMesh(*newFile, 
				!OptionsDisplay::instance()->getNoSkins(), 1.0f);
			delete newFile;
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
	mesh_->draw(actualPos, actualdir);
}
