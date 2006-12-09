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


#ifndef _ComsTargetStateMessage_h
#define _ComsTargetStateMessage_h

#include <coms/ComsMessage.h>

class ComsTargetStateMessage : public ComsMessage
{
public:
	ComsTargetStateMessage();
	virtual ~ComsTargetStateMessage();

	// Inherited from ComsMessage
    virtual bool writeMessage(NetBuffer &buffer, unsigned int destinationId);
    virtual bool readMessage(NetBufferReader &reader);

private:
	ComsTargetStateMessage(const ComsTargetStateMessage &);
	const ComsTargetStateMessage & operator=(const ComsTargetStateMessage &);

};

#endif // _ComsTargetStateMessage_h

