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

#include <tankgraph/TankModelASE.h>
#include <tankgraph/TankModelStore.h>
#include <3dsparse/ASEFile.h>
#include <common/Defines.h>
#include <common/OptionsDisplay.h>

TankModelASE::TankModelASE(TankModelId &id,
					 const char *meshName, const char *skinName) :
	TankModel(id),
	meshName_(meshName), skinName_(skinName),
	tankMesh_(0), init_(false)
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

		// Load the ASEFile containing the tank definitions
		ASEFile newFile(meshName_.c_str(), skinName_.c_str());
		if (!newFile.getSuccess())
		{
			dialogMessage("ASE File", "Failed to load ASE file \"%s\"", meshName_.c_str());
			return;
		}

		// Make sure the tank is not too large
		const float maxSize = 3.0f;
		float size = (newFile.getMax() - newFile.getMin()).Magnitude();
		if (size > maxSize)
		{
			const float sfactor = 2.2f / size;
			newFile.scale(sfactor);
		}

		// Get the model detail
		float detail = 
			float(OptionsDisplay::instance()->getMaxModelTriPercentage()) / 100.0f;

		// Create tank mesh
		tankMesh_ = new TankMesh(
			newFile, !OptionsDisplay::instance()->getNoSkins(), detail);
	}

	if (tankMesh_)
	{
		tankMesh_->draw(drawS, angle, position, fireOffSet, rotXY, rotXZ);
	}
}

int TankModelASE::getNoTris()
{ 
	if (!tankMesh_) return 0;
	return tankMesh_->getNoTris(); 
}
