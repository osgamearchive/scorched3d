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
#include <GLEXT/GLBitmap.h>
#include <landscape/Landscape.h>
#include <landscape/Sky.h>
#include <graph/MainCamera.h>
#include <graph/OptionsDisplay.h>
#include <landscapedef/LandscapeTex.h>

#include <water/Water2Constants.h>

Water2Renderer::Water2Renderer() : 
	waterShader_(0), vattr_aof_index_(0), currentPatch_(0), totalTime_(0.0f)
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
	Vector cameraPos = MainCamera::instance()->getTarget().getCamera().getCurrentPos();

	Water2Patches &currentPatch = water2.getPatch(totalTime_);
	if (&currentPatch != currentPatch_)
	{
		currentPatch_ = &currentPatch;
		normalTexture_.replace(currentPatch_->getNormalMap());
	}

	// Set shader
	cameraPos[2] = -waterHeight_;
	waterShader_->use();
	waterShader_->set_uniform("viewpos", cameraPos);

	// Setup textures
	setup_textures(water2);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	water2.getVisibility().draw(
		currentPatch, water2.getIndexs(), cameraPos, vattr_aof_index_);

	// Cleanup textures
	cleanup_textures();

	points.draw(currentPatch);
}

void Water2Renderer::setup_textures(Water2 &water2)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	waterShader_->use();
	waterShader_->set_gl_texture(foamTexture_, "tex_foam", 2);
	waterShader_->set_gl_texture(foamAmountTexture_, "tex_foamamount", 3);

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

	glActiveTexture(GL_TEXTURE0);
	waterShader_->set_uniform("noise_xform_0", noise_0_pos);
	waterShader_->set_uniform("noise_xform_1", noise_1_pos);
	waterShader_->set_gl_texture(normalTexture_, "tex_normal", 0);

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);

	waterShader_->set_gl_texture(reflectionTexture_, "tex_reflection", 1);

}

void Water2Renderer::cleanup_textures()
{
	waterShader_->use_fixed();

	glActiveTexture(GL_TEXTURE0);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glActiveTexture(GL_TEXTURE1);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glActiveTexture(GL_TEXTURE0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glPopAttrib();
}


void Water2Renderer::generate(LandscapeTexBorderWater *water, ProgressCounter *counter)
{
	// Load shaders
	if (!waterShader_) 
	{
		waterShader_ = new GLSLShaderSetup(
			formatString(getDataFile("data/shaders/water.vshader")),
			formatString(getDataFile("data/shaders/water.fshader")));

		waterShader_->use();
		vattr_aof_index_ = waterShader_->get_vertex_attrib_index("amount_of_foam");
		waterShader_->use_fixed();
	}

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
	Vector4 light_color(1.0f, 1.0f, 1.0f, 1.0f);

	Vector4 wavetop = light_color.lerp(Vector4(0, 0, 0, 0), Vector4(49.0f/100.0f, 83.0f/100.0f, 94.0f/100.0f));
	Vector4 wavebottom = light_color.lerp(Vector4(0, 0, 0, 0), Vector4(29.0f/100.0f, 56.0f/100.0f, 91.0f/100.0f));

	Vector upwelltop(wavetop[0], wavetop[1], wavetop[2]);
	Vector upwellbot(wavebottom[0], wavebottom[1], wavebottom[2]);
	Vector upwelltopbot = upwelltop - upwellbot;

	waterShader_->use();
	waterShader_->set_uniform("upwelltop", upwelltop);
	waterShader_->set_uniform("upwellbot", upwellbot);
	waterShader_->set_uniform("upwelltopbot", upwelltopbot);
	waterShader_->use_fixed();
	waterHeight_ = water->height;

	// Create textures
	GLImageHandle loadedBitmapWater = 
		GLImageFactory::loadImageHandle(getDataFile("data/textures/landscape/default/water.bmp"));
	GLImageHandle bitmapWater2 = loadedBitmapWater.createResize(128, 128);
	reflectionTexture_.create(bitmapWater2, true);
	GLBitmap map(128, 128, false, 0);
	foamAmountTexture_.create(map, false);
	GLImageHandle loadedFoam = 
		GLImageFactory::loadImageHandle(getDataFile("data/textures/foam.png"));	
	foamTexture_.create(loadedFoam, false);
	foamTexture_.draw(true);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	normalTexture_.create(map, false);
}
