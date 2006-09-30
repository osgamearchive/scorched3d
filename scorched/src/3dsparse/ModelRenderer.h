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

#if !defined(__INCLUDE_ModelRendererh_INCLUDE__)
#define __INCLUDE_ModelRendererh_INCLUDE__

#include <3dsparse/Model.h>

class ModelRenderer
{
public:
	ModelRenderer(Model *model);
	virtual ~ModelRenderer();

	void draw(float LOD = 1.0f);
	void drawBottomAligned(float LOD = 1.0f);
	void simulate(float frameTime);

	Model *getModel() { return model_; }

protected:
	Model *model_;
	float currentFrame_;
	std::vector<BoneType *> boneTypes_;
	std::vector<unsigned int> displayLists_;
	std::vector<unsigned int> lastCachedState_;
	Vector vertexTranslation_;

	virtual void drawMesh(unsigned int m, Mesh *mesh, bool dontCache, float LOD);
	virtual void drawVerts(unsigned int m, Mesh *mesh, float LOD, bool vertexLighting);
	virtual void setup();
};

#endif // __INCLUDE_ModelRendererh_INCLUDE__
