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

#include <actions/WallHit.h>
#include <sprites/WallActionRenderer.h>
#include <common/OptionsParam.h>
#include <engine/ScorchedContext.h>

REGISTER_ACTION_SOURCE(WallHit);

WallHit::WallHit() : firstTime_(true)
{
}

WallHit::WallHit(Vector &position, 
		OptionsTransient::WallSide type) :
	firstTime_(true),
	position_(position), type_(type)
{

}

WallHit::~WallHit()
{
}

void WallHit::init()
{
	if (!context_->serverMode)
	{
		setActionRender(new WallActionRenderer(position_, type_));
	}
}

void WallHit::simulate(float frameTime, bool &remove)
{
	if (!renderer_) remove = true;
	Action::simulate(frameTime, remove);
}

bool WallHit::writeAction(NetBuffer &buffer)
{
	buffer.addToBuffer(position_[0]);
	buffer.addToBuffer(position_[1]);
	buffer.addToBuffer(position_[2]);
	buffer.addToBuffer((int) type_);
	return true;
}

bool WallHit::readAction(NetBufferReader &reader)
{
	if (!reader.getFromBuffer(position_[0])) return false;
	if (!reader.getFromBuffer(position_[1])) return false;
	if (!reader.getFromBuffer(position_[2])) return false;
	int t = 0;
	if (!reader.getFromBuffer(t)) return false;
	type_ = (OptionsTransient::WallSide) t;
	return true;
}
