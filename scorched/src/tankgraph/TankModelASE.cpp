#include <tankgraph/TankModelASE.h>
#include <tankgraph/TankModelStore.h>
#include <common/Defines.h>

TankModelASE::TankModelASE(TankModelId &id,
					 const char *meshName, const char *skinName) :
	TankModel(id),
	meshName_(meshName), skinName_(skinName?skinName:""),
	tankMesh_(0), tankSkin_(0), init_(false)
{

}

TankModelASE::~TankModelASE()
{
}

void TankModelASE::draw(bool drawS, float angle, 
	Vector &position, float fireOffSet, 
	float rotXY, float rotXZ)
{
	if (!init_)
	{
		init_ = true;
		tankMesh_ = TankModelStore::instance()->loadMesh(meshName_.c_str(), true);
		if (!tankMesh_)
		{
			dialogMessage("Scorched3D", "ERROR: Failed to load mesh \"%s\"",
				meshName_.c_str());
			return;
		}

		if (skinName_.c_str()[0])
		{
			tankSkin_ = TankModelStore::instance()->loadTexture(skinName_.c_str());
			if (!tankSkin_)
			{
				dialogMessage("Scorched3D", "ERROR: Failed to load skin \"%s\"",
					skinName_.c_str());
				return;
			}
		}
	}

	GLState *currentState = 0;
	if (tankSkin_)
	{
		currentState = new GLState(GLState::TEXTURE_ON);
		tankSkin_->draw();
	}
	else
	{
		currentState = new GLState(GLState::TEXTURE_OFF);
	}

	if (tankMesh_)
	{
		tankMesh_->draw(drawS, angle, position, fireOffSet, rotXY, rotXZ);
	}
	delete currentState;
}

int TankModelASE::getNoTris()
{ 
	if (!tankMesh_) return 0;
	return tankMesh_->getNoTris(); 
}
