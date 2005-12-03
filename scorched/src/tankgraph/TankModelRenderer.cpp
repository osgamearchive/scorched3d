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

#include <tankgraph/TankModelRenderer.h>
#include <tankgraph/TankModelStore.h>
#include <landscape/Landscape.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/ShadowMap.h>
#include <landscape/Smoke.h>
#include <landscape/Hemisphere.h>
#include <tank/TankLib.h>
#include <actions/TankFalling.h>
#include <client/MainCamera.h>
#include <client/ScorchedClient.h>
#include <common/OptionsDisplay.h>
#include <common/Defines.h>
#include <3dsparse/ModelStore.h>
#include <3dsparse/ModelRenderer.h>
#include <weapons/Shield.h>
#include <weapons/Accessory.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLBitmap.h>
#include <GLEXT/GLTexture.h>
#include <GLEXT/GLViewPort.h>
#include <GLW/GLWFont.h>

float TankModelRendererHUD::timeLeft_ = -1.0f;
float TankModelRendererHUD::percentage_ = -1.0f;
std::string TankModelRendererHUD::textA_ = "";
std::string TankModelRendererHUD::textB_ = "";

Vector TankModelRendererAIM::aimPosition_;
float TankModelRendererAIM::timeLeft_ = -1.0f;

ModelRenderer *TankModelRendererAIM::getAutoAimModel()
{
	static ModelRenderer *array = 0;
	if (!array)
	{
		ModelID id;
		id.initFromString("ase", "data/meshes/autoaim.ase", "none");
		array = new ModelRenderer(
			ModelStore::instance()->loadModel(id));
	}
	return array;
}

TankModelRenderer::TankModelRenderer(Tank *tank) :
	tank_(tank), tankTips_(tank),
	model_(0), canSeeTank_(false),
	smokeTime_(0.0f), smokeWaitForTime_(0.0f),
	fireOffSet_(0.0f), shieldHit_(0.0f),
	posX_(0.0f), posY_(0.0f), posZ_(0.0f), 
	totalTime_(0.0f),
	particleMade_(false)
{
	model_ = TankModelStore::instance()->getModelByName(
		tank->getModel().getModelName());
}

TankModelRenderer::~TankModelRenderer()
{
}

void TankModelRenderer::draw(bool currentTank)
{
	if (TankModelRendererAIM::drawAim())
	{
		GLState texState(GLState::TEXTURE_OFF);
		Vector &pos = TankModelRendererAIM::getAimPosition();

		glPushMatrix();
			glTranslatef(pos[0], pos[1], pos[2]);
			TankModelRendererAIM::getAutoAimModel()->draw();
		glPopMatrix();
	}

	// Check we can see the tank
	canSeeTank_ = true;
	if (!GLCameraFrustum::instance()->
		sphereInFrustum(tank_->getPhysics().getTankPosition(), 1) ||
		(tank_->getState().getState() != TankState::sNormal))
	{
		canSeeTank_ = false;
		return;
	}

	// Store the position in which we should draw the players names
	storeTank2DPos();

	float modelSize = float(OptionsDisplay::instance()->getTankModelSize()) / 100.0f;

	// Add the tank shadow
	GLState currentState(GLState::TEXTURE_OFF);
	Landscape::instance()->getShadowMap().addCircle(
		tank_->getPhysics().getTankPosition()[0], 
		tank_->getPhysics().getTankPosition()[1], 
		4.0f * modelSize);

	// Draw the tank model
	model_->draw(currentTank, 
		tank_->getPhysics().getAngle(),
		tank_->getPhysics().getTankPosition(), 
		fireOffSet_, 
		tank_->getPhysics().getRotationGunXY(), 
		tank_->getPhysics().getRotationGunYZ(),
		false, modelSize);

	// Draw the tank sight
	if (currentTank &&
		OptionsDisplay::instance()->getDrawPlayerSight() &&
		!OptionsDisplay::instance()->getOldSightPosition())
	{
		drawSight();
	}

	// Draw the life bars
	drawLife();
}

void TankModelRenderer::drawSecond()
{
	if (!canSeeTank_) return;

	// Draw the arrow
	drawArrow();

	// Draw the parachutes (if any)
	std::map<unsigned int, TankFalling *>::iterator findItor = 
		TankFalling::fallingTanks.find(tank_->getPlayerId());
	if (findItor != TankFalling::fallingTanks.end())
	{
		if ((*findItor).second->getParachutes())
		{
			drawParachute();
		}
	}

	// Draw the current shield (if any)
	if (tank_->getAccessories().getShields().getCurrentShield())
	{
		drawShield();
	}

	Vector &position = tank_->getPhysics().getTankPosition();
	float height = position[2];
	float groundHeight = ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().
		getHeight((int) position[0], (int) position[1]);
	if (height < groundHeight)
	{
		height = groundHeight;
	}

	// Draw the names above all the tanks
	if (OptionsDisplay::instance()->getDrawPlayerNames())
	{
		Vector &bilX = GLCameraFrustum::instance()->getBilboardVectorX(); 
		bilX *= 0.5f * (float) GLWFont::instance()->getSmallPtFont()->getWidth(
			1, tank_->getName());

		glDepthMask(GL_FALSE);
		GLWFont::instance()->getSmallPtFont()->drawBilboard(
			tank_->getColor(), 1.0f, 1,
			(float) tank_->getPhysics().getTankPosition()[0] - bilX[0], 
			(float) tank_->getPhysics().getTankPosition()[1] - bilX[1], 
			(float) height + 8.0f,
			tank_->getName());
		glDepthMask(GL_TRUE);
	}

	if (OptionsDisplay::instance()->getDrawPlayerIcons())
	{
		Vector &bilX = GLCameraFrustum::instance()->getBilboardVectorX(); 
		if (OptionsDisplay::instance()->getDrawPlayerNames())
		{
			bilX *= 0.5f * (float) GLWFont::instance()->getSmallPtFont()->getWidth(
				1, tank_->getName());
			bilX += bilX.Normalize() * 1.0f;
		}
		else
		{
			bilX *= 0.0f;
		}

		Vector position = 
			tank_->getPhysics().getTankPosition() - bilX;
		position[2] = height + 8.7f;

		tank_->getAvatar().getTexture()->draw();
		static Vector white(1.0f, 1.0f, 1.0f);
		GLCameraFrustum::instance()->drawBilboard(
			position, white, 0.8f, 0.6f, 0.5f, false, 0);
	}
}

void TankModelRenderer::drawSight()
{
	GLState currentState(GLState::BLEND_OFF | GLState::TEXTURE_OFF);
	glPushMatrix();
		glTranslatef(
			tank_->getPhysics().getTankGunPosition()[0],
			tank_->getPhysics().getTankGunPosition()[1],
			tank_->getPhysics().getTankGunPosition()[2]);

		glRotatef(tank_->getPhysics().getRotationGunXY(), 
			0.0f, 0.0f, 1.0f);
		glRotatef(tank_->getPhysics().getRotationGunYZ(), 
			1.0f, 0.0f, 0.0f);

		TankMesh::drawSight();
	glPopMatrix();
}

void TankModelRenderer::drawShield()
{
	// Create the shield textures
	static GLTexture *shieldtexture = 0;
	static GLTexture *texture = 0;
	static GLTexture *texture2 = 0;
	static GLTexture *magtexture = 0;
	static GLUquadric *obj = 0;
	if (!texture)
	{
		std::string file1 = getDataFile("data/textures/bordershield/grid2.bmp");
		GLBitmap map(file1.c_str(), file1.c_str(), false);
		texture = new GLTexture;
		texture->create(map, GL_RGBA, true);

		std::string file2 = getDataFile("data/textures/bordershield/grid22.bmp");
		GLBitmap map2(file2.c_str(), file2.c_str(), false);
		texture2 = new GLTexture;
		texture2->create(map2, GL_RGBA, true);

		std::string file3 = getDataFile("data/textures/shield2.bmp");
		GLBitmap map3(file3.c_str(), file3.c_str(), false);
		magtexture = new GLTexture;
		magtexture->create(map3, GL_RGBA, true);

		std::string file4 = getDataFile("data/textures/shield.bmp");
		std::string file5 = getDataFile("data/textures/shielda.bmp");
		GLBitmap map4(file4.c_str(), file5.c_str(), false);
		map4.alphaMult(4.0f);
		shieldtexture = new GLTexture;
		shieldtexture->create(map4, GL_RGBA, true);

		obj = gluNewQuadric();
		gluQuadricTexture(obj, GL_TRUE);
	}

	// Create the shield objects
	static unsigned int smallListNo = 0;
	static unsigned int largeListNo = 0;
	static unsigned int smallHalfListNo = 0;
	static unsigned int largeHalfListNo = 0;
	static unsigned int spiralListNo = 0;
	GLTexture magTexture;
	if (!smallListNo)
	{
		glNewList(smallListNo = glGenLists(1), GL_COMPILE);
			gluSphere(obj, 3.0f, 8, 8);
		glEndList();
		glNewList(largeListNo = glGenLists(1), GL_COMPILE);
			gluSphere(obj, 6.0f, 8, 8);
		glEndList();
		glNewList(smallHalfListNo = glGenLists(1), GL_COMPILE);
			Hemisphere::draw(3.0f, 3.0f, 10, 10, 6, 0, true);
			Hemisphere::draw(3.0f, 3.0f, 10, 10, 6, 0, false);
		glEndList();
		glNewList(largeHalfListNo = glGenLists(1), GL_COMPILE);
			Hemisphere::draw(6.0f, 6.0f, 10, 10, 6, 0, true);
			Hemisphere::draw(6.0f, 6.0f, 10, 10, 6, 0, false);
		glEndList();
		glNewList(spiralListNo = glGenLists(1), GL_COMPILE);
			float height = 0.0f;
			float width = 0.0f;
			float totalA = 5.0f * PI;
			float aInc = PI / 6.0f;
			glBegin(GL_QUAD_STRIP);
			for (float a=0.0f; a<totalA; a+=aInc)
			{
				height += 0.05f;
				width += 0.05f;
				float x = getFastSin(a) * width;
				float y = getFastCos(a) * width;
				float z = height;
				glTexCoord2f(a / totalA, 0.0f);
				glVertex3f(x, y, z);
				glTexCoord2f(a / totalA, 1.0f);
				glVertex3f(x, y, z - 0.4f);
			}
			glEnd();

			height = 0.0f;
			width = 0.0f;
			glBegin(GL_QUAD_STRIP);
			for (float a=0.0f; a<5.0f * PI; a+=PI/6.0f)
			{
				height += 0.05f;
				width += 0.05f;
				float x = getFastSin(a) * width;
				float y = getFastCos(a) * width;
				float z = height;
				glTexCoord2f(a / totalA, 0.0f);
				glVertex3f(x, y, z - 0.4f);
				glTexCoord2f(a / totalA, 1.0f);
				glVertex3f(x, y, z);
			}
			glEnd();
		glEndList();
	}

	// Draw the actual shield
	Accessory *accessory = tank_->getAccessories().getShields().getCurrentShield();
	if (!accessory) return;
	Shield *shield = (Shield *) accessory->getAction();

	GLState state(GLState::BLEND_ON | GLState::TEXTURE_ON); 
	Vector &position = tank_->getPhysics().getTankPosition();
	Vector &color = shield->getColor();
	if (shield->getShieldType() == Shield::ShieldTypeMag)
	{
		magtexture->draw();

		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glColor4f(color[0], color[1], color[2], 0.4f);
		glPushMatrix();
			glTranslatef(position[0], position[1], position[2] + 1.0f);
			if (shield->getRadius() != Shield::ShieldSizeSmall) glScalef(2.0f, 2.0f, 2.0f);

			glRotatef(totalTime_ * 800.0f, 0.0f, 0.0f, 1.0f);
			glCallList(spiralListNo);
			glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
			glCallList(spiralListNo);
			glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
			glCallList(spiralListNo);
		glPopMatrix();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_TRUE);
	}
	else if (shield->getHalfShield())
	{
		texture->draw();
		glPushMatrix();
			glColor4f(color[0], color[1], color[2], 0.5f + shieldHit_);
			glTranslatef(position[0], position[1], position[2]);
			if (shield->getRadius() == Shield::ShieldSizeSmall) glCallList(smallHalfListNo);
			else glCallList(largeHalfListNo);
		glPopMatrix();
	}
	else
	{
		texture->draw();
		glPushMatrix();
			glColor4f(color[0], color[1], color[2], 0.5f + shieldHit_);
			glTranslatef(position[0], position[1], position[2]);
			if (shield->getRadius() == Shield::ShieldSizeSmall) glCallList(smallListNo);
			else glCallList(largeListNo);
		glPopMatrix();

		float aspect = float(GLViewPort::getHeight()) / float(GLViewPort::getWidth());
		shieldtexture->draw();
		GLCameraFrustum::instance()->drawBilboard(
			position,
			color,
			1.0f - shieldHit_,
			shield->getActualRadius(), 
			shield->getActualRadius() * aspect,
			true, // Additive
			0); // texcoord
	}
}

void TankModelRenderer::drawParachute()
{
	static GLuint listNo = 0;
	if (!listNo)
	{
		float a;
		glNewList(listNo = glGenLists(1), GL_COMPILE);
			glColor3f(1.0f, 1.0f, 1.0f);
			glBegin(GL_LINES);
				for (a=0.0f; a< 3.14f*2.0f; a+=3.14f / 4.0f)
				{
					glVertex3f(0.0f, 0.0f, 0.0f);
					glVertex3f(sinf(a) * 2.0f, cosf(a) * 2.0f, 2.0f);
				}
			glEnd();
			glBegin(GL_TRIANGLE_FAN);
				glVertex3f(0.0f, 0.0f, 3.0f);

				glColor3f(0.5f, 0.5f, 0.5f);
				for (a=3.14f*2.0f; a> 0.0f; a-=3.14f / 4.0f)
				{
					glVertex3f(sinf(a) * 2.0f, cosf(a) * 2.0f, 2.0f);
				}				
			glEnd();
		glEndList();
	}

	Vector &position = tank_->getPhysics().getTankPosition();
	GLState state(GLState::TEXTURE_OFF);
	glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);
		glCallList(listNo);
	glPopMatrix();
}

void TankModelRenderer::fired()
{
	fireOffSet_ = -0.25f;
}

void TankModelRenderer::shieldHit()
{
	shieldHit_ = 0.25f;
}

void TankModelRenderer::simulate(float frameTime)
{
	totalTime_ += frameTime;

	if (fireOffSet_ < 0.0f)
	{
		fireOffSet_ += frameTime / 25.0f;
		if (fireOffSet_ > 0.0f) fireOffSet_ = 0.0f;
	}
	if (shieldHit_ > 0.0f)
	{
		shieldHit_ -= frameTime / 25.0f;
		if (shieldHit_ < 0.0f) shieldHit_ = 0.0f;
	}
	if (tank_->getState().getLife() < 100)
	{
		smokeTime_ += frameTime;
		if (smokeTime_ >= smokeWaitForTime_)
		{
			const float randOff = 1.0f;
			const float randOffDiv = 0.5f;
			float randX = RAND * randOff - randOffDiv; 
			float randY = RAND * randOff - randOffDiv; 
			Landscape::instance()->getSmoke().addSmoke(
				tank_->getPhysics().getTankTurretPosition()[0] + randX, 
				tank_->getPhysics().getTankTurretPosition()[1] + randY, 
				tank_->getPhysics().getTankTurretPosition()[2]);

			smokeWaitForTime_ = (
				(RAND * float(tank_->getState().getLife()) * 10.0f) + 250.0f)
				/ 3000.0f;;
			smokeTime_ = 0.0f;
		}
	}
}

void TankModelRenderer::drawArrow()
{
	if (!OptionsDisplay::instance()->getDrawPlayerColor())
	{
		return;
	}

	GLState currentState(GLState::TEXTURE_OFF | GLState::BLEND_OFF);
	Vector &bilX = GLCameraFrustum::instance()->getBilboardVectorX();
	bilX /= 2.0f;

	Vector &position = tank_->getPhysics().getTankPosition();
	float height = position[2];
	float groundHeight = ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().
		getHeight((int) position[0], (int) position[1]);
	if (height < groundHeight)
	{
		height = groundHeight;
	}

	// Arrow over tank	
	{
		static GLTexture arrowTexture;
		if (!arrowTexture.textureValid())
		{
			std::string file1 = getDataFile("data/windows/arrow.bmp");
			std::string file2 = getDataFile("data/windows/arrowi.bmp");
			GLBitmap bitmap(file1.c_str(), file2.c_str(), true);
			arrowTexture.create(bitmap, GL_RGBA);
		}

		GLState currentState(GLState::TEXTURE_ON | GLState::BLEND_ON);
		arrowTexture.draw(true);

		glDepthMask(GL_FALSE);
		glColor3fv(tank_->getColor());
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(position[0] - bilX[0], 
				position[1] - bilX[1], height + 4.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(position[0] + bilX[0], 
				position[1] + bilX[1], height + 4.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(position[0] + bilX[0], 
				position[1] + bilX[1], height + 7.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(position[0] - bilX[0], 
				position[1] - bilX[1], height + 7.0f);
		glEnd();
		glDepthMask(GL_TRUE);
	}

}

void TankModelRenderer::drawLife()
{
	if (!OptionsDisplay::instance()->getDrawPlayerHealth())
	{
		return;
	}

	GLState currentState(GLState::TEXTURE_OFF | GLState::BLEND_OFF);
	Vector &bilX = GLCameraFrustum::instance()->getBilboardVectorX();
	bilX /= 2.0f;

	Vector &position = tank_->getPhysics().getTankPosition();
	float height = position[2];
	float groundHeight = ScorchedClient::instance()->getLandscapeMaps().getGroundMaps().
		getHeight((int) position[0], (int) position[1]);
	if (height < groundHeight)
	{
		height = groundHeight;
	}
	{
		float shieldLife = 0.0f;
		Accessory *currentShield =
			tank_->getAccessories().getShields().getCurrentShield();
		if (currentShield) shieldLife = 
			tank_->getAccessories().getShields().getShieldPower();

		drawLifeBar(bilX, tank_->getState().getLife(), height, 3.3f);
		drawLifeBar(bilX, shieldLife, height, 3.7f);
	}
}

void TankModelRenderer::drawLifeBar(Vector &bilX, float value, 
									float height, float barheight)
{
	Vector &position = tank_->getPhysics().getTankPosition();
	glBegin(GL_QUADS);
		if (value == 100.0f || value == 0.0f)
		{
			if (value == 100.0f) glColor3f(0.0f, 1.0f, 0.0f);
			else glColor3f(0.0f, 0.0f, 0.0f);

			glVertex3f(position[0] + bilX[0], 
				position[1] + bilX[1], height + barheight);
			glVertex3f(position[0] + bilX[0], 
				position[1] + bilX[1], height + barheight + 0.2f);
			glVertex3f(position[0] - bilX[0], 
				position[1] - bilX[1], height + barheight + 0.2f);
			glVertex3f(position[0] - bilX[0], 
				position[1] - bilX[1], height + barheight);
		}
		else
		{
			float lifePer = value / 50.0f;
			static Vector newBilX;
			newBilX = -bilX;
			newBilX += bilX * lifePer;

			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(position[0] + newBilX[0], 
				position[1] + newBilX[1], height + barheight);
			glVertex3f(position[0] + newBilX[0], 
				position[1] + newBilX[1], height + barheight + 0.2f);
			glVertex3f(position[0] - bilX[0], 
				position[1] - bilX[1], height + barheight + 0.2f);
			glVertex3f(position[0] - bilX[0], 
				position[1] - bilX[1], height + barheight);

			glColor3f(1.0f, 0.0f, 0.0f);

			glVertex3f(position[0] + bilX[0], 
				position[1] + bilX[1], height + barheight);
			glVertex3f(position[0] + bilX[0], 
				position[1] + bilX[1], height + barheight + 0.2f);
			glVertex3f(position[0] + newBilX[0], 
				position[1] + newBilX[1], height + barheight + 0.2f);
			glVertex3f(position[0] + newBilX[0], 
				position[1] + newBilX[1], height + barheight);
		}
	glEnd();
}

void TankModelRenderer::storeTank2DPos()
{
	Vector &tankTurretPos = 
		tank_->getPhysics().getTankTurretPosition();
	Vector camDir = 
		MainCamera::instance()->getCamera().getLookAt() - 
		MainCamera::instance()->getCamera().getCurrentPos();
	Vector tankDir = tankTurretPos - 
		MainCamera::instance()->getCamera().getCurrentPos();

	if (camDir.dotP(tankDir) < 0.0f)
	{
		posX_ = - 1000.0;
	}
	else
	{
		static GLdouble modelMatrix[16];
		static GLdouble projMatrix[16];
		static GLint viewport[4];

		glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
		glGetIntegerv(GL_VIEWPORT, viewport);

		viewport[2] = GLViewPort::getWidth();
		viewport[3] = GLViewPort::getHeight();
		int result = gluProject(
			tankTurretPos[0], 
			tankTurretPos[1], 
			tankTurretPos[2],
			modelMatrix, projMatrix, viewport,
			&posX_, 
			&posY_,
			&posZ_);
	}
}

void TankModelRenderer::draw2d(bool currentTank)
{
	if (!canSeeTank_) return;

	// Add the tooltip that displays the tank info
	GLWToolTip::instance()->addToolTip(&tankTips_.tankTip,
		float(posX_) - 10.0f, float(posY_) - 10.0f, 20.0f, 20.0f);

	// Draw the hightlighted ring around the tank
	if (currentTank)
	{
		GLState firstState(GLState::DEPTH_OFF);
		if (TankModelRendererHUD::drawText())
		{
			Vector yellow(0.7f, 0.7f, 0.0f);
			GLWFont::instance()->getSmallPtFont()->draw(
				yellow, 10,
				(float) posX_ + 47.0f, (float) posY_ - 4.0f, (float) posZ_,
				TankModelRendererHUD::getTextA());
			GLWFont::instance()->getSmallPtFont()->draw(
				yellow, 10,
				(float) posX_ + 47.0f, (float) posY_ - 15.0f, (float) posZ_,
				TankModelRendererHUD::getTextB());

			if (TankModelRendererHUD::getPercentage() >= 0.0f)
			{
				float totalHeight = 40.0f;
				float halfHeight = totalHeight / 2.0f;
				float height = totalHeight * TankModelRendererHUD::getPercentage() / 100.0f;
				
				GLState state2(GLState::TEXTURE_OFF);
				glBegin(GL_QUADS);
					glColor3f(1.0f, 0.0f, 0.0f);
					glVertex2f((float) posX_ + 42.0f, (float) posY_ -halfHeight);
					glVertex2f((float) posX_ + 45.0f, (float) posY_ -halfHeight);
					glVertex2f((float) posX_ + 45.0f, (float) posY_ -halfHeight + height);
					glVertex2f((float) posX_ + 42.0f, (float) posY_ -halfHeight + height);

					glColor3f(0.0f, 0.0f, 0.0f);
					glVertex2f((float) posX_ + 42.0f, (float) posY_ -halfHeight + height);
					glVertex2f((float) posX_ + 45.0f, (float) posY_ -halfHeight + height);
					glVertex2f((float) posX_ + 45.0f, (float) posY_ -halfHeight + totalHeight);
					glVertex2f((float) posX_ + 42.0f, (float) posY_ -halfHeight + totalHeight);
				glEnd();
				glBegin(GL_LINES);
					glColor3f(1.0f, 0.0f, 0.0f);
					glVertex2f((float) posX_ + 40.0f, (float) posY_ -halfHeight);
					glVertex2f((float) posX_ + 47.0f, (float) posY_ -halfHeight);

					glVertex2f((float) posX_ + 40.0f, (float) posY_ +halfHeight);
					glVertex2f((float) posX_ + 47.0f, (float) posY_ +halfHeight);
				glEnd();
			}
		}

		GLState newState(GLState::TEXTURE_OFF | GLState::BLEND_ON);

		glColor4f(0.9f, 0.9f, 1.0f, 0.5f);
		static GLuint listNo = 0;
		if (!listNo)
		{
			glNewList(listNo = glGenLists(1), GL_COMPILE);
				glBegin(GL_QUADS);
					for (float a=0; a<6.25f ;a+=0.25f)
					{
						const float skip = 0.2f;
						glVertex2d(sin(a + skip) * 40.0, 
							cos(a + skip) * 40.0);
						glVertex2d(sin(a) * 40.0, 
							cos(a) * 40.0);
						glVertex2d(sin(a) * 35.0, 
							cos(a) * 35.0);			
						glVertex2d(sin(a + skip) * 35.0, 
							cos(a + skip) * 35.0);
					}
				glEnd();
			glEndList();
		}

		glPushMatrix();
			glTranslated(posX_, posY_, 0.0);
			glCallList(listNo);
		glPopMatrix();
	}
}
