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

#include <tank/TankContainer.h>
#include <landscape/Landscape.h>
#include <actions/Napalm.h>
#include <actions/ShotProjectile.h>
#include <sprites/NapalmRenderer.h>
#include <sprites/ExplosionTextures.h>
#include <client/ScorchedClient.h>

NapalmRenderer::NapalmRenderer() : 
	textureNo_(0), totalTime_(0.0f), counter_(0.1f, 0.1f)
{

}

NapalmRenderer::~NapalmRenderer()
{
}

void NapalmRenderer::simulate(Action *action, float timepassed, bool &remove)
{
	// Cannot simulate textures in here
	// as we need a faster framerate than the simulation
	// loop runs at

	if (counter_.nextDraw(timepassed))
	{
		Napalm *napalm = (Napalm *) action;
		int count = int(RAND * float(napalm->getPoints().size()));

		std::list<Napalm::NapalmEntry *>::iterator itor;
		std::list<Napalm::NapalmEntry *>::iterator endItor = 
			napalm->getPoints().end();
		for (itor = napalm->getPoints().begin();
				itor != endItor;
				itor++, count--)
		{
			if (count == 0)
			{
				Napalm::NapalmEntry *entry = (*itor);

				float posZ = 
					ScorchedClient::instance()->getLandscapeMaps().getHMap().getHeight(
					entry->posX, entry->posY);
				Landscape::instance()->getSmoke().
					addSmoke(float(entry->posX), float(entry->posY), posZ, 
					0.0f, 0.0f, 0.0f, 1.0f);
				break;
			}
		}
	}
}

void NapalmRenderer::draw(Action *action)
{
	const float StepTime = 0.05f; // 20 fps

	totalTime_ += ScorchedClient::instance()->getMainLoop().getDrawTime();
	while (totalTime_ > StepTime)
	{
		totalTime_ -= StepTime;
		textureNo_++;
	}

	Napalm *napalm = (Napalm *) action;

	// Setup the bilboard
	int noTextures = ExplosionTextures::instance()->flames.getNoTextures();

	std::list<Napalm::NapalmEntry *>::iterator itor;
	std::list<Napalm::NapalmEntry *>::iterator endItor = 
		napalm->getPoints().end();
	for (itor = napalm->getPoints().begin();
			itor != endItor;
			itor++)
	{
		Napalm::NapalmEntry *entry = (*itor);
		int ix = entry->posX;
		int iy = entry->posY;
		float fz = ScorchedClient::instance()->getLandscapeMaps().getHMap().getHeight(ix, iy);
		
		// Check that this point is still above the water
		if (fz >= Landscape::instance()->getWater().getHeight())
		{
			// Draw the bilboard texture for this napalm
			// particle

			// Particle 1
			entry->renderEntry1.posZ = fz + 2.5f;
			entry->renderEntry1.texture = ExplosionTextures::instance()->flames.getTexture(
				(textureNo_ + entry->offset + 0) % noTextures);
			GLBilboardRenderer::instance()->addEntry(&entry->renderEntry1);

			// Particle 2
			entry->renderEntry2.posZ = fz + 2.5f;
			entry->renderEntry2.texture = ExplosionTextures::instance()->flames.getTexture(
				(textureNo_ + entry->offset + 10) % noTextures);
			GLBilboardRenderer::instance()->addEntry(&entry->renderEntry2);

			// Particle 3
			entry->renderEntry3.posZ = fz + 2.5f;
			entry->renderEntry3.texture = ExplosionTextures::instance()->flames.getTexture(
				(textureNo_ + entry->offset + 20) % noTextures);
			GLBilboardRenderer::instance()->addEntry(&entry->renderEntry3);
		}

		Vector position(
			entry->renderEntry1.posX, 
			entry->renderEntry1.posY,
			entry->renderEntry1.posZ);
		ShotProjectile::addLookAtPosition(
			position, napalm->getPlayerId(), *napalm->getScorchedContext());
	}
}
