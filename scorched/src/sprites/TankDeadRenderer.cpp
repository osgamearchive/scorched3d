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
#include <target/TargetContainer.h>
#include <client/ScorchedClient.h>
#include <engine/ScorchedContext.h>
#include <common/Defines.h>

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
	Target *killedTarget = ScorchedClient::instance()->
		getTargetContainer().getTargetById(killedPlayerId_);                                                                           
	if (killedTarget)
	{
		Vector position = killedTarget->getTargetPosition();
		ringRenderer_.init(0, position, Vector::nullVector, "data/wav/explosions/tank.wav");
		ringRenderer_.simulate(action, frametime, removeAction);
	}

	// Remove this action on the first itteration
	removeAction = true;
}
