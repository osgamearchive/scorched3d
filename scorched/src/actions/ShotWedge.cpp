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

#include <actions/ShotWedge.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <sprites/SphereActionRenderer.h>
#include <tank/TankContainer.h>

REGISTER_ACTION_SOURCE(ShotWedge);

ShotWedge::ShotWedge()
{
}

ShotWedge::ShotWedge(Vector &position, Vector &direction,
		unsigned int playerId,
		Weapon *weapon) : 
	position_(position), direction_(direction),
	playerId_(playerId), weapon_(weapon)
{
}

ShotWedge::~ShotWedge()
{
}

void ShotWedge::init()
{

}

void ShotWedge::simulate(float frameTime, bool &remove)
{
	Vector dir = direction_.Normalize();
	Vector start = position_;
	int steps = 3;
	float distance = 2.0f;
	float distanceIncrease = 2.0f;
	float size = 1.0f;
	float sizeIncrease = 1.0f;
	for (int i=0; i<steps; i++)
	{
		ActionRenderer *renderer = 
			new SphereActionRenderer(start, size);
		Action *action = new SpriteAction(renderer);
		context_->actionController->addAction(action);

		start += dir * distance;
		size += sizeIncrease;
		distance += distanceIncrease;
	}

	remove = true;
	ActionMeta::simulate(frameTime, remove);
}

bool ShotWedge::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(playerId_);
	buffer.addToBuffer(position_);
	buffer.addToBuffer(direction_);
	Weapon::write(buffer, weapon_);
	return true;
}

bool ShotWedge::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(playerId_)) return false;
	if (!reader.getFromBuffer(position_)) return false;
	if (!reader.getFromBuffer(direction_)) return false;
	weapon_ = Weapon::read(reader); if (!weapon_) return false;
	return true;
}

