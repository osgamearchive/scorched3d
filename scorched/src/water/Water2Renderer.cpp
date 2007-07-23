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

#include <water/Water2Renderer.h>
#include <water/Water2.h>
#include <water/WaterMapPoints.h>
#include <GLEXT/GLImageFactory.h>
#include <GLEXT/GLStateExtension.h>
#include <GLEXT/GLImageFactory.h>
#include <GLEXT/GLTextureCubeMap.h>
#include <client/ScorchedClient.h>
#include <landscape/Landscape.h>
#include <landscape/Sky.h>
#include <landscapemap/LandscapeMaps.h>
#include <landscapedef/LandscapeTex.h>
#include <landscapedef/LandscapeDefn.h>
#include <graph/MainCamera.h>
#include <graph/OptionsDisplay.h>

#include <water/Water2Constants.h>

Water2Renderer::Water2Renderer() : 
	waterShader_(0),
	currentPatch_(0), totalTime_(0.0f),
	noShaderWaterTexture_(0)
{
}

Water2Renderer::~Water2Renderer()
{
}

void Water2Renderer::simulate(float frameTime)
{
	totalTime_ += frameTime * 24.0f;
}

void Water2Renderer::draw(Water2 &water2, WaterMapPoints &points)
{
	GAMESTATE_PERF_COUNTER_START(ScorchedClient::instance()->getGameState(), "WATER_PATCHSETUP");
	// Choose correct water frame
	Water2Patches &currentPatch = water2.getPatch(totalTime_);
	if (&currentPatch != currentPatch_)
	{
		currentPatch_ = &currentPatch;

		// Set the normal map for the current water frame
		if (GLStateExtension::hasShaders())
		{
			normalTexture_.replace(currentPatch_->getNormalMap());
		}
	}
	GAMESTATE_PERF_COUNTER_END(ScorchedClient::instance()->getGameState(), "WATER_PATCHSETUP");

	// Draw Water
	if (GLStateExtension::hasShaders())
	{
		drawWaterShaders(water2);
	}
	else
	{
		drawWaterNoShaders(water2);
	}

	GAMESTATE_PERF_COUNTER_START(ScorchedClient::instance()->getGameState(), "WATER_DRAWPOINTS");
	drawPoints(points);
	GAMESTATE_PERF_COUNTER_END(ScorchedClient::instance()->getGameState(), "WATER_DRAWPOINTS");
}

void Water2Renderer::drawPoints(WaterMapPoints &points)
{
	if (!currentPatch_) return;

	// Draw Points
	points.draw(*currentPatch_);
}

void Water2Renderer::drawWaterShaders(Water2 &water2)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	Vector cameraPos = MainCamera::instance()->getTarget().getCamera().getCurrentPos();
	cameraPos[2] = -waterHeight_;
	waterShader_->use();
	waterShader_->set_uniform("viewpos", cameraPos);

	// Tex 3
	waterShader_->set_gl_texture(currentPatch_->getAOF(), "tex_foamamount", 3);

	// Tex 2
	if (Landscape::instance()->getShadowFrameBuffer().bufferValid())
	{
		glActiveTextureARB(GL_TEXTURE2_ARB);
		glEnable(GL_TEXTURE_2D);
		waterShader_->set_gl_texture(Landscape::instance()->getShadowFrameBuffer(), 
			"tex_shadow", 2);
		glMatrixMode(GL_TEXTURE);
		glLoadMatrixf(Landscape::instance()->getShadowTextureMatrix());
		glMatrixMode(GL_MODELVIEW);

		waterShader_->set_uniform("use_shadow", 1.0);
	}
	else
	{
		waterShader_->set_uniform("use_shadow", 0.0);
	}

	// texture units / coordinates:
	// tex0: noise map (color normals) / matching texcoords
	// tex1: reflection map / matching texcoords
	// tex2: foam
	// tex3: amount of foam

	// set up scale/translation of foam and noise maps
	// we do not use the texture matrix here, as this would be overkill
	// and would be clumsy
	/* how to compute that.
	   We would need to compute a closed path over a tile (modulo tile size).
	   This can be any path with curves etc., but lets take a linear movement.
	   Start and end point must be the same in the tile (modulo tilesize)
	   after a given time t.
	   To achieve this we need to know how many tiles we move horizontally and vertically
	   for the path. This are numbers A and B. We move with velocity V (meters/second),
	   thus we get as time t for the path:
	   t = tile_size * sqrt(a*a + b*b) / V
	   and for the direction vector of movement:
	   D = (V/sqrt(a*a + b*b)) * (a, b)
	   Example, if we would like to move 2 tiles right and 5 down, with 0.5m/s, we get
	   t = 256m * sqrt(2*2+5*5) / 0.5m/s = 2757.2s
	   and D = (0.186, 0.464)
	   So we have to take mytime module t and use that as multiplier with D to get
	   the current position (plus initial offset S).
	   In the shader the vertex position is multiplied with factor z here and then
	   xy are added. So z translates meters to texture coordinates.
	   For noise #0 scale is 8/256m, so we have 8 tiles of the texture per wave tile.
	   So one noise tile is 256m/8 = 1/z = 32m long. That is the tile size we have
	   to use for the computation above.
	   32m with V=2m/sec -> t = 86.162 and D = (0.743, 1.857)
	*/
	static const int a_0 = 2, b_0 = 5;
	static const int a_1 = -4, b_1 = 3;
	static const float s_0 = sqrtf(a_0*a_0 + b_0*b_0), s_1 = sqrtf(a_1*a_1 + b_1*b_1);
	static const float V_0 = 2.0f, V_1 = 1.0f;
	// maybe: remove hardwired scale factors of 8 and 32, but looks best with that values.
	static const float t_0 = wavetile_length / 8.0f * s_0 / V_0, t_1 = wavetile_length / 32.0f * s_1 / V_1;
	// need to divide by noise tile size here too (tex coordinates are in [0...1], not meters)
	static Vector D_0 = Vector(a_0, b_0) * (V_0/s_0/32.0f); // noise tile is 256/8=32m long
	static Vector D_1 = Vector(a_1, b_1) * (V_1/s_1/8.0f);  // noise tile is 256/32=8m long

	float mytime = totalTime_ / 24.0f;
	Vector noise_0_pos = Vector(0, 0) + D_0 * myfmod(mytime, t_0);
	Vector noise_1_pos = Vector(0, 0) + D_1 * myfmod(mytime, t_1);
	noise_0_pos[2] = wavetile_length_rcp * 8.0f;
	noise_1_pos[2] = wavetile_length_rcp * 32.0f;

	//fixme: do we have to treat the viewer offset here, like with tex matrix
	//       setup below?!

	// Tex 0
	glActiveTexture(GL_TEXTURE0);
	waterShader_->set_uniform("noise_xform_0", noise_0_pos);
	waterShader_->set_uniform("noise_xform_1", noise_1_pos);
	waterShader_->set_gl_texture(normalTexture_, "tex_normal", 0);

	const float noisetilescale = 1.0f/32.0f;//meters (128/16=8, 8tex/m).
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(noisetilescale, noisetilescale, 1.0f);	// fixme adjust scale
	glMatrixMode(GL_MODELVIEW);

	// Tex 1
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	waterShader_->set_gl_texture(reflectionTexture_, "tex_reflection", 1);

	// Draw Water
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	drawWater(water2);

	// Cleanup	
	waterShader_->use_fixed();

	glActiveTexture(GL_TEXTURE2);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE1);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glPopAttrib();
}

void Water2Renderer::drawWaterNoShaders(Water2 &water2)
{
	glPushAttrib(GL_TEXTURE_BIT);

	// Set up texture coordinate generation for reflections
	static float PlaneS[] = { 0.0f, 1.0f / 20.0f, 0.0f, 0.0f };
	static float PlaneT[] = { 1.0f / 20.0f, 0.0f, 0.0f, 0.0f };

	if (GLStateExtension::hasMultiTex())
	{
		// Set up texture coordinate generation for base texture
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_GEN_S); 
		glEnable(GL_TEXTURE_GEN_T);
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_S, GL_OBJECT_PLANE, PlaneS);
		glTexGenfv(GL_T, GL_OBJECT_PLANE, PlaneT);
		reflectionTexture_.draw(true);

		glActiveTexture(GL_TEXTURE0);
	}

	// Turn lighting on (if enabled)
	unsigned int state = 0;
	if (!OptionsDisplay::instance()->getNoModelLighting())
	{
		state = 
			GLState::LIGHTING_ON | 
			GLState::LIGHT1_ON;

		Vector4 ambientColor(0.4f, 0.4f, 0.4f, 1.0f);
		Vector4 diffuseColor(0.6f, 0.6f, 0.6f, 1.0f);
		Vector4 specularColor(1.0f, 1.0f, 1.0f, 1.0f);
		Vector4 emissiveColor(0.0f, 0.0f, 0.0f, 1.0f);
		float shininess = 1.0f;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissiveColor);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

		Landscape::instance()->getSky().getSun().setLightPosition();
	}

	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	if (GLStateExtension::hasCubeMap())
	{
		GLState currentState(state | GLState::TEXTURE_OFF | GLState::BLEND_ON | GLState::CUBEMAP_ON);

		// Set up texture coordinate generation for cubemap reflections
		glEnable(GL_TEXTURE_GEN_S); 
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
		glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
		glTexGenf(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);

		drawWater(water2);
	}
	else if (GLStateExtension::hasSphereMap())
	{
		GLState currentState(state | GLState::TEXTURE_ON | GLState::BLEND_ON);
		noShaderWaterTexture_->draw();

		// Set up texture coordinate generation for spheremap reflections
		glEnable(GL_TEXTURE_GEN_S); 
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGenf(GL_R, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

		drawWater(water2);
	}
	else
	{
		GLState currentState(state | GLState::TEXTURE_ON | GLState::BLEND_ON);
		noShaderWaterTexture_->draw();

		// Set up texture coordinate generation for linear reflections
		glEnable(GL_TEXTURE_GEN_S); 
		glEnable(GL_TEXTURE_GEN_T);
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_S, GL_OBJECT_PLANE, PlaneS);
		glTexGenfv(GL_T, GL_OBJECT_PLANE, PlaneT);

		drawWater(water2);
	}

	if (GLStateExtension::hasMultiTex())
	{
		glActiveTexture(GL_TEXTURE1);
		glDisable(GL_TEXTURE_GEN_S); 
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		glDisable(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE0);
		glDisable(GL_TEXTURE_GEN_S); 
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
	}

	glPopAttrib();
}

void Water2Renderer::drawWater(Water2 &water2)
{
	// Draw Water
	Vector &cameraPos = 
		MainCamera::instance()->getTarget().getCamera().getCurrentPos();
	water2.getVisibility().draw(
		*currentPatch_, water2.getIndexs(), cameraPos, landscapeSize_, waterShader_);
}

void Water2Renderer::generate(LandscapeTexBorderWater *water, ProgressCounter *counter)
{
	currentPatch_ = 0;
	if (GLStateExtension::hasShaders())
	{
		// Load shaders
		if (!waterShader_) 
		{
			waterShader_ = new GLSLShaderSetup(
				formatString(getDataFile("data/shaders/water.vshader")),
				formatString(getDataFile("data/shaders/water.fshader")));
		}
	}

	// Set the water height
	waterHeight_ = water->height;

	// fixme: color depends also on weather. bad weather -> light is less bright
	// so this will be computed implicitly. Environment can also change water color
	// (light blue in tropic waters), water depth also important etc.
	// this depends on sky color...
	// good weather
	//	color wavetop = color(color(10, 10, 10), color(18, 93, 77), light_brightness);
	//	color wavebottom = color(color(10, 10, 20), color(18, 73, 107), light_brightness);
	// rather bad weather
	//fixme: multiply with light color here, not only light brightness.
	//dim yellow light should result in dim yellow-green upwelling color, not dim green.
	Vector4 light_color(water->wavelight, 1.0f);
	Vector4 wavetopA(water->wavetopa, 0.0f);
	Vector4 wavetopB(water->wavetopb, 0.0f);
	Vector4 wavebottomA(water->wavebottoma, 0.0f);
	Vector4 wavebottomB(water->wavebottomb, 0.0f);
	Vector4 wavetop = light_color.lerp(wavetopA, wavetopB);
	Vector4 wavebottom = light_color.lerp(wavebottomA, wavebottomB);

	// Create textures
	if (GLStateExtension::hasFBO() &&
		GLStateExtension::hasShaders() &&
		!OptionsDisplay::instance()->getNoWaterReflections())
	{
		reflectionTexture_.createBufferTexture(512, 512, false);
		reflectionBuffer_.create(reflectionTexture_, true);
	}
	else
	{
		GLImageHandle loadedBitmapWater = 
			GLImageFactory::loadImageHandle(getDataFile(water->texture.c_str()));
		GLImageHandle bitmapWater2 = loadedBitmapWater.createResize(128, 128);
		reflectionTexture_.create(bitmapWater2, true); // Not the reflection in this case
	}

	GLImageHandle map = GLImageFactory::createBlank(128, 128, false, 0);
	normalTexture_.create(map, false);

	LandscapeDefn &defn = *ScorchedClient::instance()->getLandscapeMaps().
		getDefinitions().getDefn();
	landscapeSize_ = Vector(defn.landscapewidth, defn.landscapeheight);

	if (GLStateExtension::hasShaders())
	{
		Vector landfoam;
		Vector upwelltop(wavetop[0], wavetop[1], wavetop[2]);
		Vector upwellbot(wavebottom[0], wavebottom[1], wavebottom[2]);
		Vector upwelltopbot = upwelltop - upwellbot;

		waterShader_->use();
		waterShader_->set_uniform("upwelltop", upwelltop);
		waterShader_->set_uniform("upwellbot", upwellbot);
		waterShader_->set_uniform("upwelltopbot", upwelltopbot);
		waterShader_->set_uniform("landfoam", landfoam);
		waterShader_->set_uniform("landscape_size", landscapeSize_);
		waterShader_->use_fixed();
	}
	else
	{
		// Load the water reflection bitmap
		// Create water cubemap texture
		GLImageHandle loadedBitmapWater = 
			GLImageFactory::loadImageHandle(getDataFile(water->reflection.c_str()));
		GLImageHandle bitmapWater2 = loadedBitmapWater.createResize(256, 256);
		delete noShaderWaterTexture_;
		if (GLStateExtension::hasCubeMap())
		{
			GLTextureCubeMap *waterCubeMap = new GLTextureCubeMap();
			waterCubeMap->create(bitmapWater2, false);
			noShaderWaterTexture_ = waterCubeMap;
		}
		else 
		{
			GLTexture *waterNormalMap = new GLTexture();
			waterNormalMap->create(bitmapWater2, false);
			noShaderWaterTexture_ = waterNormalMap;
		}
	}
}
