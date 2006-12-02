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

#include <graph/ModelRenderer.h>
#include <graph/OptionsDisplay.h>
#include <graph/TextureStore.h>
#include <3dsparse/ModelMaths.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLTexture.h>
#include <GLEXT/GLInfo.h>

ModelRenderer::ModelRenderer(Model *model) : 
	model_(model)
{
	setup();
}

ModelRenderer::~ModelRenderer()
{
	while (!boneTypes_.empty())
	{
		BoneType *type = boneTypes_.back();
		boneTypes_.pop_back();
		delete type;
	}
	while (!meshInfos_.empty())
	{
		MeshInfo info = meshInfos_.back();
		meshInfos_.pop_back();
		glDeleteLists(info.displayList, 1);
	}
}

void ModelRenderer::setup()
{
	currentFrame_ = (float) model_->getStartFrame();

	std::vector<BoneType *> &baseTypes = model_->getBaseBoneTypes();
	std::vector<BoneType *>::iterator itor;
	for (itor = baseTypes.begin();
		itor != baseTypes.end();
		itor++)
	{
		boneTypes_.push_back(new BoneType(*(*itor)));
	}

	MeshInfo info;
	for (unsigned int m=0; m<model_->getMeshes().size(); m++)
	{
		meshInfos_.push_back(info);
	}
}

void ModelRenderer::drawBottomAligned(float LOD)
{
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -model_->getMin()[2]);
		draw(LOD);
	glPopMatrix();
}

void ModelRenderer::draw(float LOD)
{
	for (unsigned int m=0; m<model_->getMeshes().size(); m++)
	{
		Mesh *mesh = model_->getMeshes()[m];
		drawMesh(m, mesh, mesh->getReferencesBones(), LOD);
	}
}

void ModelRenderer::simulate(float frameTime)
{
	currentFrame_ += frameTime;
	if (currentFrame_ > (float) model_->getTotalFrames())
	{
		currentFrame_ = (float) model_->getStartFrame();
	}

	for (unsigned int b=0; b<boneTypes_.size(); b++)
	{
		Bone *bone = model_->getBones()[b];
		BoneType *type = boneTypes_[b];

		unsigned int posKeys = bone->getPositionKeys().size();
		unsigned int rotKeys = bone->getRotationKeys().size();
		if (posKeys == 0 && rotKeys == 0)
		{
			memcpy(type->final_, type->absolute_, sizeof(BoneMatrixType));
			continue;
		}

		BoneMatrixType m;
		bone->getRotationAtTime(currentFrame_, m);
				
		Vector &pos = bone->getPositionAtTime(currentFrame_);
		m[0][3] = pos[0];
		m[1][3] = pos[1];
		m[2][3] = pos[2];

		ModelMaths::concatTransforms(type->relative_, m, type->relativeFinal_);
		if (type->parent_ == -1)
		{
			memcpy(type->final_, type->relativeFinal_, sizeof(BoneMatrixType));
		}
		else
		{
			BoneType *parent = boneTypes_[type->parent_];
			ModelMaths::concatTransforms(parent->final_, type->relativeFinal_, type->final_);
		}
	}
}

void ModelRenderer::drawMesh(unsigned int m, Mesh *mesh, bool dontCache, float LOD)
{
	MeshInfo &meshInfo = meshInfos_[m];

	bool useTextures =
		(!OptionsDisplay::instance()->getNoSkins() &&
		mesh->getTextureName()[0]);
	unsigned state = GLState::TEXTURE_OFF;
	if (useTextures)
	{
		state = GLState::TEXTURE_ON;
		if (!meshInfo.texture)
		{
			meshInfo.texture = 
				TextureStore::instance()->loadTexture(
 					mesh->getTextureName(), mesh->getATextureName());

		}

		meshInfo.texture->draw();
		if (mesh->getSphereMap())
		{
			state |= GLState::NORMALIZE_ON;

			glEnable(GL_TEXTURE_GEN_S);						
			glEnable(GL_TEXTURE_GEN_T);	
			glEnable(GL_TEXTURE_GEN_R);
			glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGenf(GL_R, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		}
	}
	bool vertexLighting = OptionsDisplay::instance()->getNoModelLighting();
	if (!vertexLighting)
	{
		state |= 
			GLState::NORMALIZE_ON | 
			GLState::LIGHTING_ON | 
			GLState::LIGHT1_ON;

		if (useTextures)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mesh->getAmbientColor());
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mesh->getDiffuseColor());
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mesh->getSpecularColor());
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mesh->getEmissiveColor());
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mesh->getShininessColor());
		}
		else
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mesh->getAmbientNoTexColor());
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mesh->getDiffuseNoTexColor());
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mesh->getSpecularNoTexColor());
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mesh->getEmissiveNoTexColor());
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mesh->getShininessColor());
		}
	}

	GLState glState(state);
	if (dontCache)
	{
		drawVerts(m, mesh, LOD, vertexLighting);
	}
	else
	{
		unsigned int lastState = meshInfo.lastCachedState;
		unsigned int displayList = meshInfo.displayList;
		if (lastState != state)
		{
			if (displayList != 0)
			{
				glDeleteLists(displayList, 1);
				displayList = 0;
			}
			meshInfo.lastCachedState = state;
		}

		if (!displayList)
		{
			glNewList(displayList = glGenLists(1), GL_COMPILE);
				drawVerts(m, mesh, LOD, vertexLighting);
			glEndList();

			meshInfo.displayList = displayList;
		}

		glCallList(displayList);
		GLInfo::addNoTriangles((int) mesh->getFaces().size());
	}

	if (useTextures)
	{
		if (mesh->getSphereMap())
		{
			glDisable(GL_TEXTURE_GEN_S);						
			glDisable(GL_TEXTURE_GEN_T);	
			glDisable(GL_TEXTURE_GEN_R);
		}
	}
}

void ModelRenderer::drawVerts(unsigned int m, Mesh *mesh, float LOD, bool vertexLighting)
{
	Vector vec;
	glBegin(GL_TRIANGLES);

	int maxIndex = int(float(mesh->getVertexes().size()) * LOD);
	int faceVerts[3];
	if (mesh->getCollapseMap().empty() &&
		!mesh->getFaces().empty() &&
		!OptionsDisplay::instance()->getNoModelLOD())
	{
		mesh->setupCollapse();
	}
	
	std::vector<Face *>::iterator itor;
	for (itor = mesh->getFaces().begin();
		itor != mesh->getFaces().end();
		itor++)
	{
		Face *face = *itor;

		for (int i=0; i<3; i++)
		{
			int index = face->v[i];
			if (!OptionsDisplay::instance()->getNoModelLOD() &&
				LOD < 1.0f)
			{
				while (index > maxIndex)
				{
					index = mesh->getCollapseMap()[index];
				}
			}

			faceVerts[i] = index;
		}
		
		if (faceVerts[0] != faceVerts[1] &&
			faceVerts[1] != faceVerts[2] &&
			faceVerts[0] != faceVerts[2])
		{
			GLInfo::addNoTriangles(1);
			for (int i=0; i<3; i++)
			{
				Vertex *vertex = mesh->getVertex(faceVerts[i]);

				if (vertexLighting)
				{
					if (GLState::getState() & GLState::TEXTURE_OFF) 
					{
						glColor3f(
							mesh->getDiffuseNoTexColor()[0] * vertex->lightintense[0],
							mesh->getDiffuseNoTexColor()[1] * vertex->lightintense[1],
							mesh->getDiffuseNoTexColor()[2] * vertex->lightintense[2]);
					}
					else
					{
						glColor3fv(vertex->lightintense);
					}
				}

				glTexCoord2f(face->tcoord[i][0], face->tcoord[i][1]);
				glNormal3fv(face->normal[i]);

				if (vertex->boneIndex != -1)
				{
					BoneType *type = boneTypes_[vertex->boneIndex];

					// Note: Translation of MS to S3D coords
					Vector newPos, newVec;
					newPos[0] = vertex->position[0];
					newPos[1] = vertex->position[2];
					newPos[2] = vertex->position[1];

					ModelMaths::vectorRotate(newPos, type->final_, newVec);
					vec[0] = newVec[0];
					vec[1] = newVec[2];
					vec[2] = newVec[1];

					vec[0] += type->final_[0][3] + vertexTranslation_[0];
					vec[1] += type->final_[2][3] + vertexTranslation_[1];
					vec[2] += type->final_[1][3] + vertexTranslation_[2];
					
				}
				else
				{
					vec[0] = vertex->position[0] + vertexTranslation_[0];
					vec[1] = vertex->position[1] + vertexTranslation_[1];
					vec[2] = vertex->position[2] + vertexTranslation_[2];
				}

				glVertex3fv(vec);
			}
		}
	}
	glEnd();

	/*
	// draw normals
	{
		glBegin(GL_LINES);
		for (itor = mesh->getFaces().begin();
			itor != mesh->getFaces().end();
			itor++)
		{
			Face *face = *itor;

			for (int i=0; i<3; i++)
			{
				int index = face->v[i];
				if (!OptionsDisplay::instance()->getNoModelLOD() &&
					LOD < 1.0f)
				{
					while (index > maxIndex)
					{
						index = mesh->getCollapseMap()[index];
					}
				}

				faceVerts[i] = index;
			}
			
			if (faceVerts[0] != faceVerts[1] &&
				faceVerts[1] != faceVerts[2] &&
				faceVerts[0] != faceVerts[2])
			{
				GLInfo::addNoTriangles(1);
				for (int i=0; i<3; i++)
				{
					Vertex *vertex = mesh->getVertex(faceVerts[i]);

					glColor3f(1.0f, 0.0f, 0.0f);
					if (vertex->boneIndex != -1)
					{
						BoneType *type = boneTypes_[vertex->boneIndex];

						// Note: Translation of MS to S3D coords
						Vector newPos, newVec;
						newPos[0] = vertex->position[0];
						newPos[1] = vertex->position[2];
						newPos[2] = vertex->position[1];

						ModelMaths::vectorRotate(newPos, type->final_, newVec);
						vec[0] = newVec[0];
						vec[1] = newVec[2];
						vec[2] = newVec[1];

						vec[0] += type->final_[0][3] + vertexTranslation_[0];
						vec[1] += type->final_[2][3] + vertexTranslation_[1];
						vec[2] += type->final_[1][3] + vertexTranslation_[2];
						
					}
					else
					{
						vec[0] = vertex->position[0] + vertexTranslation_[0];
						vec[1] = vertex->position[1] + vertexTranslation_[1];
						vec[2] = vertex->position[2] + vertexTranslation_[2];
					}

					glVertex3fv(vec);
					glVertex3fv(vec + face->normal[i] * 2.0f);
				}
			}
		}
		glEnd();
	}
	*/
}
