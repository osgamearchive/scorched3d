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


#include <landscape/DeformTextures.h>
#include <landscape/Landscape.h>
#include <landscape/GlobalHMap.h>
#include <GLEXT/GLBitmapModifier.h>
#include <GLEXT/GLState.h>
#include <common/Defines.h>
#include <math.h>

void DeformTextures::deformLandscape(Vector &pos, float radius, 
									 bool down, DeformLandscape::DeformPoints &map)
{
	HeightMap &hmap = GlobalHMap::instance()->getHMap();
	int iradius = (int) radius + 1;
	if (iradius > 49) iradius = 49;

	// Recalculate landscape
	Landscape::instance()->recalculate((int) pos[0], (int) pos[1], (int) radius);

	// Add a scorched texture to the area
	// Calculate new texture and load it into the gound texture
	GLBitmapModifier::addScorchToBitmap(
		hmap, 
		Landscape::instance()->getSun().getPosition(),
		Landscape::instance()->getMainMap(), Landscape::instance()->getScorchMap(), 
		map, (int) (pos[0] - iradius), (int) (pos[1] - iradius), iradius * 2);

	GLState currentState(GLState::TEXTURE_ON);
	Landscape::instance()->getMainTexture().draw(true);

	float pixelsPerSW = (float)(Landscape::instance()->getMainMap().getWidth() / hmap.getWidth());
	float pixelsPerSH = (float)(Landscape::instance()->getMainMap().getHeight() / hmap.getWidth());

	GLint x = GLint((pos[0] - radius) * pixelsPerSW);
	GLint y = GLint((pos[1] - radius) * pixelsPerSH);
	GLsizei w = GLsizei(pixelsPerSW * 2.0f * radius);
	GLsizei h = GLsizei(pixelsPerSH * 2.0f * radius);

	x = MAX(x, 0);
	y = MAX(y, 0);
	w = MIN(w, Landscape::instance()->getMainMap().getWidth() - x);
	w = MIN(h, Landscape::instance()->getMainMap().getHeight() - y);

	if (x < Landscape::instance()->getMainMap().getWidth() &&
		y < Landscape::instance()->getMainMap().getHeight())
	{
		int width = 3 * Landscape::instance()->getMainMap().getWidth();
		width   = (width + 3) & ~3;	

		GLubyte *bytes = 
			Landscape::instance()->getMainMap().getBits() + ((width * y) + x * 3);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 
			Landscape::instance()->getMainMap().getWidth());
		glTexSubImage2D(GL_TEXTURE_2D, 0, 
						x, y, 
						w, h, 
						GL_RGB, GL_UNSIGNED_BYTE, 
						bytes);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	}
}
