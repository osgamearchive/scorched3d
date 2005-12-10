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

#ifndef _TargetModelId_h
#define _TargetModelId_h

#include <coms/NetBuffer.h>

// An abstract class that can be used to derive from
// to define an object that will render the tank
class TankModelIdRenderer
{
public:
	TankModelIdRenderer();
	virtual ~TankModelIdRenderer();
};

// The model used for this tank
class TargetModelId
{
public:
	TargetModelId(const char *modelName);
	TargetModelId(const TargetModelId &);
	virtual ~TargetModelId();

	const TargetModelId & operator=(const TargetModelId &);

	// The name of the model that should be used for this tank
	const char *getModelName() { return modelName_.c_str(); }

	// Class that can be used to add rendering functionality
	// to the tank
	TankModelIdRenderer *getModelIdRenderer() { return modelIdRenderer_; }
	void setModelIdRenderer(TankModelIdRenderer *ptr) { modelIdRenderer_ = ptr; }

	// Serialize the modelid
    bool writeMessage(NetBuffer &buffer);
    bool readMessage(NetBufferReader &reader);

protected:
	std::string modelName_;
	TankModelIdRenderer *modelIdRenderer_;

};

#endif // _TargetModelId_h

