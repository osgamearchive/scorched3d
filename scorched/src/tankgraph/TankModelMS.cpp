#include <tankgraph/TankModelMS.h>
#include <tankgraph/TankModelStore.h>
#include <common/Defines.h>

TankModelMS::TankModelMS(TankModelId &id,
					 const char *meshName) :
	TankModel(id),
	meshName_(meshName), 
	tankMesh_(0), init_(false)
{

}

TankModelMS::~TankModelMS()
{
}

void TankModelMS::draw(bool drawS, float angle, 
	Vector &position, float fireOffSet, 
	float rotXY, float rotXZ)
{
	if (!init_)
	{
		init_ = true;
		tankMesh_ = TankModelStore::instance()->loadMesh(meshName_.c_str(), false);
		if (!tankMesh_)
		{
			dialogMessage("Scorched3D", "ERROR: Failed to load mesh \"%s\"",
				meshName_.c_str());
			return;
		}
	}

	if (tankMesh_)
	{
		tankMesh_->draw(drawS, angle, position, fireOffSet, rotXY, rotXZ);
	}
}

int TankModelMS::getNoTris()
{ 
	if (!tankMesh_) return 0;
	return tankMesh_->getNoTris(); 
}
