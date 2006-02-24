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

#include <3dsparse/ModelID.h>

// The model used for this tank
class TargetModelIdRenderer;
class ScorchedContext;
class TankType;
class TargetModelId
{
public:
	enum TargetModelType 
	{
		eNone,
		eTankModel,
		eTargetModel,
		eTreeModel
	};

	TargetModelId(const char *tankModelName); // Used for tanks
	TargetModelId(ModelID &targetModel); // Used for targets
	TargetModelId(const char *treeModel, bool tree); // Used for trees
	TargetModelId(const TargetModelId &);
	virtual ~TargetModelId();

	const TargetModelId & operator=(const TargetModelId &);

	TargetModelType getTargetType() { return targetType_; }

	// The name of the model that should be used for this tank
	const char *getTankModelName() { return tankModelName_.c_str(); }

	// The model that should be used for this target
	ModelID &getTargetModel() { return targetModel_; }

	// The tree model that should be used for this target
	const char *getTreeModel() { return treeModel_.c_str(); }

	// The type of this tank (not for targets)
	TankType *getTankType(ScorchedContext &context);

	// Class that can be used to add rendering functionality
	// to the tank
	TargetModelIdRenderer *getModelIdRenderer() { return modelIdRenderer_; }
	void setModelIdRenderer(TargetModelIdRenderer *ptr) { modelIdRenderer_ = ptr; }

	// Serialize the modelid
    bool writeMessage(NetBuffer &buffer);
    bool readMessage(NetBufferReader &reader);

protected:
	std::string treeModel_; // Model for trees
	std::string tankModelName_; // Model for tank
	ModelID targetModel_; // Model for targets
	TargetModelType targetType_; // Says which model to use
	TargetModelIdRenderer *modelIdRenderer_;
	TankType *tankType_;

};

#endif // _TargetModelId_h

