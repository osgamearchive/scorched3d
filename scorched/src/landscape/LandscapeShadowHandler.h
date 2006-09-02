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

#if !defined(__INCLUDE_LandscapeShadowHandlerh_INCLUDE__)
#define __INCLUDE_LandscapeShadowHandlerh_INCLUDE__

#include <engine/GameStateI.h>
#include <GLEXT/GLShadowFrameBuffer.h>

class LandscapeShadowHandler : public GameStateI
{
public:
	static LandscapeShadowHandler *instance();

	GLTexture &getShadowTexture() { return shadowTexture_; }
	GLdouble *getLightModelMatrix() { return lightModelMatrix_; }
	GLdouble *getLightProjMatrix() { return lightProjMatrix_; }

	// Inherited from GameStateI
	virtual void draw(const unsigned state);
	virtual void simulate(const unsigned state, float frameTime);

protected:
	bool initialized_;
	GLTexture shadowTexture_;
	GLShadowFrameBuffer frameBuffer_;
	GLdouble lightModelMatrix_[16];
	GLdouble lightProjMatrix_[16];

	void initialize();
	void drawLandscape();

private:
	LandscapeShadowHandler();
	virtual ~LandscapeShadowHandler();
};

#endif // __INCLUDE_LandscapeShadowHandlerh_INCLUDE__
