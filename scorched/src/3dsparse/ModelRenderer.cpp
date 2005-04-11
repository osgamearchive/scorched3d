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

#include <3dsparse/ModelRenderer.h>
#include <3dsparse/ModelMaths.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLTexture.h>
#include <common/OptionsDisplay.h>

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
}

void ModelRenderer::drawBottomAligned()
{
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -model_->getMin()[2]);
		draw();
	glPopMatrix();
}

void ModelRenderer::draw()
{
	for (unsigned int m=0; m<model_->getMeshes().size(); m++)
	{
		Mesh *mesh = model_->getMeshes()[m];
		drawMesh(m, mesh);
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

		Vector &pos = bone->getPositionAtTime(currentFrame_);
		Vector &rot = bone->getRotationAtTime(currentFrame_);

		BoneMatrixType m;
		ModelMaths::angleMatrix(rot, m);
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

void ModelRenderer::drawMesh(unsigned int m, Mesh *mesh)
{
	Vector vec;
	bool useTextures =
		(!OptionsDisplay::instance()->getNoSkins() &&
		mesh->getTexture());
	unsigned state = GLState::TEXTURE_OFF;
	if (useTextures)
	{
		state = GLState::TEXTURE_ON;
		mesh->getTexture()->draw();
	}
	GLState glState(state);

	glBegin(GL_TRIANGLES);
	std::vector<Face *>::iterator itor;
	for (itor = mesh->getFaces().begin();
		itor != mesh->getFaces().end();
		itor++)
	{
		Face *face = *itor;
		for (int i=0; i<3; i++)
		{
			Vertex *vertex = mesh->getVertex(face->v[i]);

			if (!useTextures) glColor3f(
				mesh->getColor()[0] * vertex->color[0],
				mesh->getColor()[1] * vertex->color[1],
				mesh->getColor()[2] * vertex->color[2]);
			else
			{
				glColor3fv(vertex->color);
				glTexCoord2f(face->tcoord[i][0], 
					face->tcoord[i][1]);
			}

			if (vertex->boneIndex != -1)
			{
				BoneType *type = boneTypes_[vertex->boneIndex];

				ModelMaths::vectorRotate(vertex->position, type->final_, vec);
				vec[0] += type->final_[0][3] + vertexTranslation_[0];
				vec[1] += type->final_[1][3] + vertexTranslation_[1];
				vec[2] += type->final_[2][3] + vertexTranslation_[2];
				
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
	glEnd();
}
