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

#include <actions/Animation.h>
#include <engine/MetaClass.h>
#include <engine/ScorchedContext.h>
#include <engine/ActionController.h>
#include <sprites/MetaActionRenderer.h>
#include <common/Defines.h>

Animation::Animation(unsigned int playerId,
		Vector &position, 
		Vector &velocity,
		const char *rendererName,
		const char *data) : 
	position_(position), playerId_(playerId), velocity_(velocity),
	rendererName_(rendererName), data_(data)
{
}

Animation::~Animation()
{
}

void Animation::init()
{
#ifndef S3D_SERVER
	if (!context_->serverMode)
	{
		MetaActionRenderer *renderer = (MetaActionRenderer *) 
			MetaClassRegistration::getNewClass(rendererName_.c_str());

		if (renderer)
		{
			renderer->init(playerId_, position_, velocity_, data_.c_str());
			context_->actionController->addAction(
				new SpriteAction(renderer));
		}
		else
		{
			dialogMessage("Animation", formatString(
						  "No renderer named \"%s\"",
						  rendererName_.c_str()));
		}
	}
#endif // #ifndef S3D_SERVER
}

void Animation::simulate(float frameTime, bool &removeAction)
{
	ActionReferenced::simulate(frameTime, removeAction);
	if (!renderer_) removeAction = true;
}
