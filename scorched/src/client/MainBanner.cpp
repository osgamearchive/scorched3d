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

#include <client/MainBanner.h>
#include <client/ScorchedClient.h>
#include <tank/TankContainer.h>
#include <GLEXT/GLViewPort.h>
#include <GLEXT/GLState.h>
#include <common/OptionsDisplay.h>

MainBanner* MainBanner::instance_ = 0;

MainBanner* MainBanner::instance()
{
	if (!instance_)
	{
		instance_ = new MainBanner;
	}

	return instance_;
}

MainBanner::MainBanner() : 
	banner_(0)
{
	banner_ = new GLFontBanner(140, 0, 1024, OptionsDisplay::instance()->getBannerRows());
	Logger::addLogger(this);
}

MainBanner::~MainBanner()
{

}

void MainBanner::logMessage(
		const char *time,
		const char *message,
		unsigned int playerId)
{
	Tank *source = ScorchedClient::instance()->getTankContainer().getTankById(playerId);
	banner_->addLine(source?source->getColor():GLFontBanner::defaultColor, message);
}

void MainBanner::simulate(const unsigned state, float frameTime)
{
	banner_->simulate(frameTime);
}

void MainBanner::draw(const unsigned state)
{
	GLState currentState(GLState::TEXTURE_OFF);
	glColor3f(1.0f, 1.0f, 0.0f);

	GLsizei width = GLViewPort::getWidth();
	banner_->setX(width / 2.0f);
	banner_->draw();
} 
