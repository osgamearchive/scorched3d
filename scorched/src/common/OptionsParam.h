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

#if !defined(__INCLUDE_OptionsParamh_INCLUDE__)
#define __INCLUDE_OptionsParamh_INCLUDE__

#include <common/OptionEntry.h>

class OptionsParam
{
public:
	static OptionsParam *instance();

	int getSeed() { return seed_.getValue(); }
	const char *getConnect() { return connect_.getValue(); }
	const char *getServerFile() { return server_.getValue(); }
	const char *getClientFile() { return client_.getValue(); }
	const char *getSaveFile() { return save_.getValue(); }
	const char *getUserName() { return username_.getValue(); }
	const char *getPassword() { return password_.getValue(); }
	const char *getSettingsDir() { return settingsdir_.getValue(); }
	bool getConsole() { return console_.getValue(); }
	bool getScreenSaverMode() { return screensaver_.getValue(); }
	bool getLoadModFiles() { return loadmodfiles_.getValue(); }

	enum Action
	{
		ActionNone = 0,
		ActionRunServer,
		ActionRunClient,
		ActionError,
		ActionHelp
	};
	Action getAction();
	void clearAction();

	bool getConnectedToServer() { return (getConnect()[0] != '\0'); }
	bool getDedicatedServer() { return (getServerFile()[0] != '\0'); }
	bool getSinglePlayer() { return (getClientFile()[0] != '\0'); }
	bool &getSDLInitVideo() { return SDLInitVideo_; }

	std::list<OptionEntry *> &getOptions();

protected:
	static OptionsParam *instance_;
	std::list<OptionEntry *> options_;

	OptionEntryString connect_;
	OptionEntryString server_;
	OptionEntryString client_;
	OptionEntryString password_;
	OptionEntryString username_;
	OptionEntryString save_;
	OptionEntryString settingsdir_;
	OptionEntryBool loadmodfiles_;
	OptionEntryBool screensaver_;
	OptionEntryBool nooptions_;
	OptionEntryBool console_;
	OptionEntryBool help_;
	OptionEntryInt seed_;
	bool SDLInitVideo_;
	bool screenSaverMode_;

private:
	OptionsParam();
	virtual ~OptionsParam();

};

#endif
