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
	connect_(options_, "connect", 
		"The name of the server to connect to, starts a NET/LAN client", 0, ""),
	server_(options_, "startserver",
		"Starts a scorched 3d server, requires the name of the server settings file e.g. data/server.xml", 0, ""),
	client_(options_, "startclient",
		"Starts a scorched 3d client, requires the name of the client settings file e.g. data/singlecustom.xml", 0, ""),
	save_(options_, "loadsave",
		"Continues a scorched 3d client game, requires the name of the saved game.", 0, ""),
	password_(options_, "password",
		"The password of the NET/LAN server", 0, ""),
	help_(options_, "starthelp",
		"Show the Scorched3D help dialog", 0, false),
	nooptions_(options_, "nooptions",
		"Only provided for backward compatability, has no current meaning", 0, false),
	console_(options_, "console",
		"Run the server in a console (non-gui) mode", 0, false)
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
	if (getConnect()[0] || getClientFile()[0] || getSaveFile()[0])
	{
		if (getServerFile()[0]) return ActionError;
		if (getConnect()[0] && getClientFile()[0]) return ActionError;
		if (getSaveFile()[0] && getClientFile()[0]) return ActionError;
		if (getSaveFile()[0] && getConnect()[0]) return ActionError;

		server_.setValue("");
		return ActionRunClient;
	}
	else if (getServerFile()[0])
	{
		return ActionRunServer;
	}

	if (help_.getValue()) return ActionHelp;
	return ActionNone;
}

void OptionsParam::clearAction()
{
	connect_.setValue("");
	client_.setValue("");
	server_.setValue("");
}
