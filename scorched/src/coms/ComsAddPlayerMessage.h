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

#ifndef _ComsAddPlayerMessage_h
#define _ComsAddPlayerMessage_h

#include <coms/ComsMessage.h>
#include <common/Vector.h>

class ComsAddPlayerMessage : public ComsMessage
{
public:
	ComsAddPlayerMessage(
		unsigned int playerId = 0,
		const char *playerName = "",
		Vector playerColor = Vector(),
		const char *modelName = "",
		unsigned int destinationId = 0,
		unsigned int playerTeam = 0,
		const char *playerType = "");
	virtual ~ComsAddPlayerMessage();

	const char *getPlayerName() { return playerName_.c_str(); }
	const char *getPlayerType() { return playerType_.c_str(); }
	const char *getModelName() { return modelName_.c_str(); }
	unsigned int getPlayerId() { return playerId_; }
	Vector &getPlayerColor() { return playerColor_; }
	unsigned int getDestinationId() { return destinationId_; }
	unsigned int getPlayerTeam() { return playerTeam_; }

	unsigned int getPlayerIconSize() { return playerIconSize_; }
	unsigned char *getPlayerIcon() { return playerIcon_; }
	void setPlayerIcon(unsigned char *icon,
		unsigned int size) { playerIcon_ = icon; playerIconSize_ = size; }

	// Inherited from ComsMessage
	virtual bool writeMessage(NetBuffer &buffer);
	virtual bool readMessage(NetBufferReader &reader);

protected:
	unsigned int playerId_;
	unsigned int destinationId_;
	unsigned int playerTeam_;
	std::string playerName_;
	std::string modelName_;
	std::string playerType_;
	Vector playerColor_;
	unsigned int playerIconSize_;
	unsigned char *playerIcon_;

private:
	ComsAddPlayerMessage(const ComsAddPlayerMessage &);
	const ComsAddPlayerMessage & operator=(const ComsAddPlayerMessage &);

};

#endif // _ComsAddPlayerMessage_h

