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

#include <sprites/TankDeadRenderer.h>
#include <sound/Sound.h>
#include <tank/TankContainer.h>
#include <client/ScorchedClient.h>
#include <engine/ParticleEmitter.h>
#include <engine/ScorchedContext.h>
#include <engine/ParticleEngine.h>

TankDeadRenderer::TankDeadRenderer(
	Weapon *weapon,
	unsigned int killedPlayerId, unsigned int firedPlayerId) :
	weapon_(weapon), 
	killedPlayerId_(killedPlayerId), firedPlayerId_(firedPlayerId)
{
}

TankDeadRenderer::~TankDeadRenderer()
{
}

void TankDeadRenderer::draw(Action *action)
{
}

void TankDeadRenderer::simulate(Action *action, float frametime, bool &removeAction)
{
	Tank *killedTank = ScorchedClient::instance()->
		getTankContainer().getTankById(killedPlayerId_);                                                                           
	if (killedTank)
	{
		// Play Dead Tank Sound
		CACHE_SOUND(sound, (char *) getDataFile("data/wav/explosions/tank.wav"));
		SoundSource *source = Sound::instance()->createSource();
		source->setPosition(killedTank->getPhysics().getTankPosition());
		source->play(sound);
		Sound::instance()->manageSource(source);

		// Emmit explosion ring
		Vector position = 
			killedTank->getPhysics().getTankPosition();
		ParticleEmitter emmiter;
		emmiter.setAttributes(
			0.6f, 0.6f, // Life
			0.5f, 0.5f, // Mass
			0.0f, 0.0f, // Friction
			Vector(), Vector(), // Velocity
			Vector(0.0f, 0.0f, 0.8f), 0.9f, // StartColor1
			Vector(0.2f, 0.2f, 0.9f), 1.0f, // StartColor2
			Vector(0.6f, 0.6f, 0.95f), 0.0f, // EndColor1
			Vector(0.8f, 0.8f, 1.0f), 0.1f, // EndColor2
			0.2f, 0.2f, 0.5f, 0.5f, // Start Size
			1.5f, 1.5f, 3.0f, 3.0f, // EndSize
			Vector(0.0f, 0.0f, 0.0f), // Gravity
			true,
			false);
		emmiter.emitExplosionRing(
			400, position, 
			ScorchedClient::instance()->getParticleEngine(), 
			ParticleRendererQuads::getInstance());
	}

	// Remove this action on the first itteration
	removeAction = true;
}
