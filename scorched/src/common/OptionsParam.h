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

	const char *getConnect() { return connect_.getValue(); }
	void setConnect(const char *value) { connect_.setValue(value); }

	const char *getServerFile() { return server_.getValue(); }
	void setServerFile(const char *value) { server_.setValue(value); }

	const char *getName() { return name_.getValue(); }
	void setName(const char *value) { name_.setValue(value); }

	const char *getSkin() { return skin_.getValue(); }
	void setSkin(const char *value) { skin_.setValue(value); }

	enum Action
	{
		RunServer,
		RunClient,
		NoAction
	};
	Action getAction();

	bool getConnectedToServer() { return (getConnect()[0] != '\0'); }
	bool getOnServer() { return (getServerFile()[0] != '\0'); }
	bool &getSinglePlayer() { return singlePlayer_; }
	bool &getSDLInitVideo() { return SDLInitVideo_; }

	std::list<OptionEntry *> &getOptions();

protected:
	static OptionsParam *instance_;
	std::list<OptionEntry *> options_;

	OptionEntryString connect_;
	OptionEntryString server_;
	OptionEntryString name_;
	OptionEntryString skin_;
	OptionEntryBool nooptions_;
	bool SDLInitVideo_;
	bool onServer_;
	bool singlePlayer_;

private:
	OptionsParam();
	virtual ~OptionsParam();

};

#endif
