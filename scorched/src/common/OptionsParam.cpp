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


#include <common/OptionsParam.h>
#include <common/Defines.h>

OptionsParam *OptionsParam::instance_ = 0;

OptionsParam *OptionsParam::instance()
{
	if (!instance_)
	{
		instance_ = new OptionsParam;
	}

	return instance_;
}

OptionsParam::OptionsParam() :
	SDLInitVideo_(false),
	onServer_(false),
	singlePlayer_(false),
	connect_(options_, "connect", 
		"The name of the server to connect to, starts a NET/LAN client", 0, ""),
	server_(options_, "startserver",
		"Starts a scorched 3d server, requires the name of the server settings file e.g. data/server.xml", 0, ""),
	name_(options_, "name",
		"The name of the NET/LAN player (asked for if not provided)", 0, ""),
	skin_(options_, "skin",
		"The name of the tank for the NET/LAN player (asked for if not provided)", 0, ""),
	nooptions_(options_, "nooptions",
		"Only provided for backward compatability, has no current meaning", 0, false)
{

}

OptionsParam::~OptionsParam()
{
	
}

std::list<OptionEntry *> &OptionsParam::getOptions()
{
	return options_;
}

OptionsParam::Action OptionsParam::getAction()
{
	if (getConnect()[0] || singlePlayer_)
	{
		setServerFile("");
		return RunClient;
	}
	else if (getServerFile()[0])
	{
		return RunServer;
	}

	return NoAction;
}
