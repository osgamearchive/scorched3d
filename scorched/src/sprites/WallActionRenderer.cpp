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

#include <GLEXT/GLState.h>
#include <GLEXT/GLBitmap.h>
#include <sprites/WallActionRenderer.h>
#include <engine/ScorchedContext.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <common/OptionsTransient.h>

GLTexture WallActionRenderer::texture_ = GLTexture();

WallActionRenderer::WallActionRenderer(
	Vector &position, OptionsTransient::WallSide type) :
	position_(position), type_(type), fade_(1.0f), init_(false)
{

}

WallActionRenderer::~WallActionRenderer()
{

}

void WallActionRenderer::init(Action *action)
{
	init_ = true;

	if (!texture_.textureValid())
	{
		std::string file = getDataFile("data/textures/bordershield/hit.bmp");
		GLBitmap map(file.c_str(), file.c_str(), false);
		texture_.create(map, GL_RGBA, true);
	}

	Landscape::instance()->getWall().wallHit(type_);
	Vector pos = position_;
	Vector offSet1, offSet2, offSet3 ,offSet4;

	static float offsetAmount = 0.1f;
	const float offset = offsetAmount;
	offsetAmount += 0.02f;
	if (offsetAmount > 0.4f) offsetAmount = 0.1f;

	switch (type_)
	{
	case OptionsTransient::LeftSide:
		pos[0] = 0.0f;
		xOff_ = -offset; yOff_ = 0.0f;
		offSet1 = Vector(0.0f, 1.0f, 1.0f);
		offSet2 = Vector(0.0f, 1.0f, -1.0f);
		offSet3 = Vector(0.0f, -1.0f, -1.0f);
		offSet4 = Vector(0.0f, -1.0f, 1.0f);
		break;
	case OptionsTransient::RightSide:
		pos[0] = (float) 
			action->getScorchedContext()->landscapeMaps->getHMap().getWidth();
		xOff_ = -offset; yOff_ = 0.0f;
		offSet1 = Vector(0.0f, 1.0f, 1.0f);
		offSet2 = Vector(0.0f, 1.0f, -1.0f);
		offSet3 = Vector(0.0f, -1.0f, -1.0f);
		offSet4 = Vector(0.0f, -1.0f, 1.0f);
		break;
	case OptionsTransient::TopSide:
		pos[1] = 0.0f;
		xOff_ = 0.0f; yOff_ = offset;
		offSet1 = Vector(1.0f, 0.0f, 1.0f);
		offSet2 = Vector(1.0f, 0.0f, -1.0f);
		offSet3 = Vector(-1.0f, 0.0f, -1.0f);
		offSet4 = Vector(-1.0f, 0.0f, 1.0f);
		break;
	default:
	case OptionsTransient::BotSide:
		pos[1] = (float) 
			action->getScorchedContext()->landscapeMaps->getHMap().getWidth();
		xOff_ = 0.0f; yOff_ = offset;
		offSet1 = Vector(1.0f, 0.0f, 1.0f);
		offSet2 = Vector(1.0f, 0.0f, -1.0f);
		offSet3 = Vector(-1.0f, 0.0f, -1.0f);
		offSet4 = Vector(-1.0f, 0.0f, 1.0f);
		break;
	}

	const float size = 20.0f;
	cornerA_ = pos + offSet1 * size;
	cornerB_ = pos + offSet2 * size;
	cornerC_ = pos + offSet3 * size;
	cornerD_ = pos + offSet4 * size;

	color_ = action->getScorchedContext()->optionsTransient->getWallColor();
}

void WallActionRenderer::simulate(Action *action, float frameTime, bool &remove)
{
	fade_ -= frameTime / 2.0f;
	remove = (fade_ < 0.0f);
}

void WallActionRenderer::draw(Action *action)
{
	if (!init_) init(action);

	GLState currentState(GLState::BLEND_ON | GLState::TEXTURE_ON);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		

	texture_.draw();
	glColor4f(color_[0], color_[1], color_[2], fade_);
	glPushMatrix();
		glTranslatef(xOff_, yOff_, 0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3fv(cornerA_);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3fv(cornerB_);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3fv(cornerC_);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3fv(cornerD_);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-xOff_, -yOff_, 0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3fv(cornerD_);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3fv(cornerC_);
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3fv(cornerB_);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3fv(cornerA_);
		glEnd();
	glPopMatrix();
}
