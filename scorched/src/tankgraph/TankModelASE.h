#pragma once

#include <tankgraph/TankMesh.h>
#include <tankgraph/TankModel.h>
#include <GLEXT/GLTexture.h>
#include <string>

class TankModelASE : public TankModel
{
public:
	TankModelASE(TankModelId &id,
		const char *meshName, 
		const char *skinName = 0);
	virtual ~TankModelASE();

	void draw(bool drawS, float angle, Vector &position, 
		float fireOffSet, float rotXY, float rotXZ);

	int getNoTris();

protected:
	std::string meshName_;
	std::string skinName_;

	bool init_;
	TankMesh *tankMesh_;
	GLTexture *tankSkin_;

};
