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


#if !defined(__INCLUDE_ComsTextMessageh_INCLUDE__)
#define __INCLUDE_ComsTextMessageh_INCLUDE__

#include <coms/ComsMessage.h>
#include <common/Vector.h>

class ComsTextMessage : public ComsMessage
{
public:
	ComsTextMessage(const char *text = "",
		unsigned int playerId = 0,
		bool showAsMessage = false,
		bool teamOnlyMessage = false,
		unsigned int infoLen = 0);
	virtual ~ComsTextMessage();

	const char *getText() { return text_.c_str(); }
	unsigned int getPlayerId() { return playerId_; }
	unsigned int getInfoLen() { return infoLen_; }
	bool getShowAsMessage() { return showAsMessage_; }
	bool getTeamOnlyMessage() { return teamOnlyMessage_; }

	// Inherited from ComsMessage
    virtual bool writeMessage(NetBuffer &buffer, unsigned int destinationId);
    virtual bool readMessage(NetBufferReader &reader);

protected:
	std::string text_;
	unsigned int playerId_;
	unsigned int infoLen_;
	bool showAsMessage_;
	bool teamOnlyMessage_;

private:
	ComsTextMessage(const ComsTextMessage &);
	const ComsTextMessage & operator=(const ComsTextMessage &);

};


#endif
