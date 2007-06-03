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

#if !defined(__INCLUDE_Water2Rendererh_INCLUDE__)
#define __INCLUDE_Water2Rendererh_INCLUDE__

#include <GLEXT/GLTexture.h>
#include <GLEXT/GLFrameBufferObject.h>
#include <GLSL/GLSLShaderSetup.h>

class Water2;
class Water2Patches;
class WaterMapPoints;
class LandscapeTexBorderWater;
class ProgressCounter;
class Vector;
class Water2Renderer
{
public:
	Water2Renderer();
	~Water2Renderer();

	void draw(Water2 &water2, WaterMapPoints &points);
	void simulate(float frameTime);
	void generate(LandscapeTexBorderWater *water, ProgressCounter *counter = 0);

	void bindWaterReflection() { reflectionBuffer_.bind(); }
	void unBindWaterReflection() { reflectionBuffer_.unBind(); }
	void drawPoints(WaterMapPoints &points);

protected:
	float totalTime_;
	float waterHeight_;
	GLTexture foamTexture_;
	GLTexture reflectionTexture_;
	GLTexture normalTexture_;
	GLTextureBase *noShaderWaterTexture_;
	GLFrameBufferObject reflectionBuffer_;

	Water2Patches *currentPatch_;
	GLSLShaderSetup *waterShader_;

	void drawWaterShaders(Water2 &water2);
	void drawWaterNoShaders(Water2 &water2);
	void drawWater(Water2 &water2);
};

#endif // __INCLUDE_Water2Rendererh_INCLUDE__
