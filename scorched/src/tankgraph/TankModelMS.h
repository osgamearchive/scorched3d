#pragma once

#include <tankgraph/TankModel.h>
#include <tankgraph/TankMesh.h>

class TankModelMS : public TankModel
{
public:
	TankModelMS(TankModelId &id,
		const char *meshName);
	virtual ~TankModelMS();

	void draw(bool drawS, float angle, Vector &position, 
		float fireOffSet, float rotXY, float rotXZ);
	int getNoTris();

protected:
	std::string meshName_;
	bool init_;
	TankMesh *tankMesh_;

};
