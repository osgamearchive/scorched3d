////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#if !defined(__INCLUDE_LoggerIh_INCLUDE__)
#define __INCLUDE_LoggerIh_INCLUDE__

#include <string>

class GLTexture;
class LoggerInfo
{
public:
	enum LogType
	{
		TypeNormal = 1,
		TypeDeath = 2,
		TypeTalk = 4,
		TypePerformance = 8
	};

	LoggerInfo(
		LogType type = LoggerInfo::TypeNormal,
		const char *message = "", 
		const char *time = "");

	void setMessage(const char *message);
	void setTime(const char *time);
	void setIcon(GLTexture *icon);
	void setInfoLen(unsigned int infoLen);
	void setPlayerId(unsigned int playerId);
	void setOtherPlayerId(unsigned int playerId);

	unsigned int getType() { return type_; }
	const char *getMessage() { return message_.c_str(); }
	const char *getTime() { return time_.c_str(); }
	unsigned int getMessageLen() { return message_.size(); }
	unsigned int getInfoLen() { return infoLen_; }
    unsigned int getPlayerId() { return playerId_; }
	unsigned int getOtherPlayerId() { return otherPlayerId_; }
	GLTexture *getIcon() { return  icon_; }

protected:
	unsigned int type_;
	unsigned int infoLen_;
	unsigned int playerId_;
	unsigned int otherPlayerId_;
	GLTexture *icon_;
	std::string message_;
	std::string time_;
};

class LoggerI
{
public:
	virtual void logMessage(LoggerInfo &info) = 0;
};

#endif // __INCLUDE_LoggerIh_INCLUDE__
