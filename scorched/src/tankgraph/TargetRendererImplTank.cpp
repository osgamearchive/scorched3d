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

#include <tankgraph/TargetRendererImplTank.h>
#include <tank/TankLib.h>
#include <tank/TankContainer.h>
#include <tank/TankModelStore.h>
#include <tank/TankModelContainer.h>
#include <tank/TankState.h>
#include <tank/TankPosition.h>
#include <tank/TankAvatar.h>
#include <target/TargetLife.h>
#include <target/TargetShield.h>
#include <target/TargetState.h>
#include <tankgraph/TankMesh.h>
#include <tankgraph/TankMeshStore.h>
#include <landscape/Landscape.h>
#include <landscapemap/LandscapeMaps.h>
#include <landscape/ShadowMap.h>
#include <landscape/Smoke.h>
#include <landscape/Hemisphere.h>
#include <actions/TankFalling.h>
#include <engine/ActionController.h>
#include <client/ScorchedClient.h>
#include <client/ClientState.h>
#include <graph/OptionsDisplay.h>
#include <graph/ModelRenderer.h>
#include <common/Defines.h>
#include <graph/ModelRendererStore.h>
#include <graph/ModelRendererSimulator.h>
#include <weapons/Shield.h>
#include <weapons/Accessory.h>
#include <dialogs/TutorialDialog.h>
#include <GLEXT/GLCameraFrustum.h>
#include <GLEXT/GLCamera.h>
#include <GLEXT/GLImageFactory.h>
#include <GLEXT/GLTexture.h>
#include <GLEXT/GLViewPort.h>
#include <GLW/GLWFont.h>

float TargetRendererImplTankHUD::timeLeft_ = -1.0f;
float TargetRendererImplTankHUD::percentage_ = -1.0f;
std::string TargetRendererImplTankHUD::textA_ = "";
std::string TargetRendererImplTankHUD::textB_ = "";

Vector TargetRendererImplTankAIM::aimPosition_;
float TargetRendererImplTankAIM::timeLeft_ = -1.0f;

ModelRendererSimulator *TargetRendererImplTankAIM::getAutoAimModel()
{
	static ModelRendererSimulator *modelRenderer = 0;
	if (!modelRenderer)
	{
		ModelID id;
		id.initFromString("ase", "data/meshes/autoaim.ase", "none");
		modelRenderer = new ModelRendererSimulator(
			ModelRendererStore::instance()->loadModel(id));
	}
	return modelRenderer;
}

TargetRendererImplTank::TargetRendererImplTank(Tank *tank) :
	tank_(tank), tankTips_(tank),
	model_(0), mesh_(0), canSeeTank_(false),
	smokeTime_(0.0f), smokeWaitForTime_(0.0f),
	fireOffSet_(0.0f), shieldHit_(0.0f),
	posX_(0.0f), posY_(0.0f), posZ_(0.0f), 
	totalTime_(0.0f)
{
	frame_ = (float) rand();
}

TargetRendererImplTank::~TargetRendererImplTank()
{
}

TankModel *TargetRendererImplTank::getModel()
{
	if (!model_)
	{
		// Try the user's specified tank 1st
		model_ = ScorchedClient::instance()->getTankModels().getModelByName(
			tank_->getModelContainer().getTankOriginalModelName(), 
			tank_->getTeam(),
			tank_->isTemp());
		if (0 != strcmp(model_->getName(), 
			tank_->getModelContainer().getTankOriginalModelName()))
		{
			// If this fails use the one the server chose
			model_ = ScorchedClient::instance()->getTankModels().getModelByName(
				tank_->getModelContainer().getTankModelName(), 
				tank_->getTeam(),
				tank_->isTemp());
		}
	}
	return model_;
}

TankMesh *TargetRendererImplTank::getMesh()
{
	if (!mesh_)
	{
		mesh_ = TankMeshStore::instance()->getMesh(getModel()->getTankModelID());
	}
	return mesh_;
}

void TargetRendererImplTank::draw(float distance)
{
	if (tank_->getState().getState() != TankState::sNormal) return;

	if (TargetRendererImplTankAIM::drawAim())
	{
		GLState texState(GLState::TEXTURE_OFF);
		Vector &pos = TargetRendererImplTankAIM::getAimPosition();

		glPushMatrix();
			glTranslatef(pos[0], pos[1], pos[2]);
			TargetRendererImplTankAIM::getAutoAimModel()->draw();
		glPopMatrix();
	}

	// Check we can see the tank
	canSeeTank_ = true;
	if (!GLCameraFrustum::instance()->
		sphereInFrustum(tank_->getPosition().getTankPosition(),
		tank_->getLife().getSize().Max() / 2.0f,
		GLCameraFrustum::FrustrumRed) ||
		(tank_->getState().getState() != TankState::sNormal))
	{
		canSeeTank_ = false;
		return;
	}

	createParticle(tank_);

	// Store the position in which we should draw the players names
	storeTank2DPos();

	// Get the model size
	float modelSize = float(OptionsDisplay::instance()->getTankModelSize()) / 100.0f;

	// Figure out the drawing distance
	float drawDistance = OptionsDisplay::instance()->getDrawDistance() * modelSize * 2.0f;
	float drawDistanceFade =  OptionsDisplay::instance()->getDrawDistanceFade();
	float drawDistanceFadeStart = drawDistance - drawDistanceFade;
	if (distance > drawDistance) return;
	float fade = 1.0f;
	if (distance > drawDistanceFadeStart)
	{
		fade = 1.0f - ((distance - drawDistanceFadeStart) / drawDistanceFade);
	}

	// Add the tank shadow
	GLState currentState(GLState::TEXTURE_OFF);
	if (tank_->getTargetState().getDisplayShadow())
	{
		Landscape::instance()->getShadowMap().addCircle(
			tank_->getPosition().getTankPosition()[0], 
			tank_->getPosition().getTankPosition()[1], 
			(tank_->getLife().getSize().Max() + 2.0f) * modelSize, 
			fade);
	}

	// Draw the tank model
	bool currentTank = 
		(tank_ == ScorchedClient::instance()->getTankContainer().getCurrentTank() &&
		ScorchedClient::instance()->getGameState().getState() == ClientState::StatePlaying);

	TankMesh *mesh = getMesh();
	if (mesh)
	{
		mesh->draw(frame_,
			currentTank, 
			tank_->getLife().getQuaternion(),
			tank_->getPosition().getTankPosition(), 
			fireOffSet_, 
			tank_->getPosition().getRotationGunXY(), 
			tank_->getPosition().getRotationGunYZ(),
			false, modelSize, fade);
	}

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

void TargetRendererImplTank::drawSecond(float distance)
{
	if (!canSeeTank_ ||
		tank_->getState().getState() != TankState::sNormal) return;

	drawParachute(tank_);
	drawShield(tank_, shieldHit_, totalTime_);
	
	if (!tank_->isTemp()) drawInfo();
}

void TargetRendererImplTank::drawInfo()
{
	// Draw the arrow
	drawArrow();

	Vector &position = tank_->getPosition().getTankPosition();
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
		bilX *= 0.5f * GLWFont::instance()->getSmallPtFont()->getWidth(
			1, tank_->getName());

		glDepthMask(GL_FALSE);
		GLWFont::instance()->getSmallPtFont()->drawBilboard(
			tank_->getColor(), 1.0f, 1,
			(float) tank_->getPosition().getTankPosition()[0] - bilX[0], 
			(float) tank_->getPosition().getTankPosition()[1] - bilX[1], 
			(float) height + 8.0f,
			tank_->getName());
		glDepthMask(GL_TRUE);
	}

	if (OptionsDisplay::instance()->getDrawPlayerIcons())
	{
		Vector &bilX = GLCameraFrustum::instance()->getBilboardVectorX(); 
		if (OptionsDisplay::instance()->getDrawPlayerNames())
		{
			bilX *= 0.5f * GLWFont::instance()->getSmallPtFont()->getWidth(
				1, tank_->getName());
			bilX += bilX.Normalize() * 1.0f;
		}
		else
		{
			bilX *= 0.0f;
		}

		Vector position = 
			tank_->getPosition().getTankPosition() - bilX;
		position[2] = height + 8.5f;

		tank_->getAvatar().getTexture()->draw();
		static Vector white(1.0f, 1.0f, 1.0f);
		GLCameraFrustum::instance()->drawBilboard(
			position, white, 0.8f, 0.5f, 0.5f, false, 0);
	}
}

void TargetRendererImplTank::drawSight()
{
	GLState currentState(GLState::BLEND_OFF | GLState::TEXTURE_OFF);
	glPushMatrix();
		glTranslatef(
			tank_->getPosition().getTankGunPosition()[0],
			tank_->getPosition().getTankGunPosition()[1],
			tank_->getPosition().getTankGunPosition()[2]);

		glRotatef(tank_->getPosition().getRotationGunXY(), 
			0.0f, 0.0f, 1.0f);
		glRotatef(tank_->getPosition().getRotationGunYZ(), 
			1.0f, 0.0f, 0.0f);

		TankMesh::drawSight();
	glPopMatrix();
}

void TargetRendererImplTank::fired()
{
	fireOffSet_ = -0.25f;
}

void TargetRendererImplTank::shieldHit()
{
	shieldHit_ = 0.25f;
}

void TargetRendererImplTank::simulate(float frameTime)
{
	if (tank_->getState().getState() != TankState::sNormal) return;

	frameTime *= ScorchedClient::instance()->getActionController().getFast();
	totalTime_ += frameTime;
	frame_ += frameTime * 20.0f;

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
	if (tank_->getLife().getLife() < tank_->getLife().getMaxLife())
	{
		smokeTime_ += frameTime;
		if (smokeTime_ >= smokeWaitForTime_)
		{
			const float randOff = 1.0f;
			const float randOffDiv = 0.5f;
			float randX = RAND * randOff - randOffDiv; 
			float randY = RAND * randOff - randOffDiv; 
			Landscape::instance()->getSmoke().addSmoke(
				tank_->getPosition().getTankTurretPosition()[0] + randX, 
				tank_->getPosition().getTankTurretPosition()[1] + randY, 
				tank_->getPosition().getTankTurretPosition()[2]);

			smokeWaitForTime_ = (
				(RAND * float(tank_->getLife().getLife()) * 10.0f) + 250.0f)
				/ 3000.0f;;
			smokeTime_ = 0.0f;
		}
	}
}

void TargetRendererImplTank::drawArrow()
{
	if (!OptionsDisplay::instance()->getDrawPlayerColor())
	{
		return;
	}

	GLState currentState(GLState::TEXTURE_OFF | GLState::BLEND_OFF);
	Vector &bilX = GLCameraFrustum::instance()->getBilboardVectorX();
	bilX /= 2.0f;

	Vector &position = tank_->getPosition().getTankPosition();
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
			GLImageHandle bitmap = 
				GLImageFactory::loadImageHandle(file1.c_str(), file2.c_str(), true);
			arrowTexture.create(bitmap);
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

void TargetRendererImplTank::drawLife()
{
	if (!OptionsDisplay::instance()->getDrawPlayerHealth())
	{
		return;
	}

	GLState currentState(GLState::TEXTURE_OFF | GLState::BLEND_OFF);
	Vector &bilX = GLCameraFrustum::instance()->getBilboardVectorX();
	bilX /= 2.0f;

	Vector &position = tank_->getPosition().getTankPosition();
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
			tank_->getShield().getCurrentShield();
		if (currentShield)
		{	
			Shield *shield =  (Shield *) 
				tank_->getShield().getCurrentShield()->getAction();
			shieldLife = tank_->getShield().getShieldPower() / 
				shield->getPower() * 100.0f;
		}

		drawLifeBar(bilX, 
			tank_->getLife().getLife() / tank_->getLife().getMaxLife() * 100.0f, 
			height, 3.3f);
		drawLifeBar(bilX, shieldLife, height, 3.7f);
	}
}

void TargetRendererImplTank::drawLifeBar(Vector &bilX, float value, 
									float height, float barheight)
{
	Vector &position = tank_->getPosition().getTankPosition();
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

void TargetRendererImplTank::storeTank2DPos()
{
	Vector &tankTurretPos = 
		tank_->getPosition().getTankTurretPosition();
	Vector camDir = 
		GLCamera::getCurrentCamera()->getLookAt() - 
		GLCamera::getCurrentCamera()->getCurrentPos();
	Vector tankDir = tankTurretPos - 
		GLCamera::getCurrentCamera()->getCurrentPos();

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

void TargetRendererImplTank::draw2d()
{
	if (!canSeeTank_) return;

	// Add the tooltip that displays the tank info
	GLWToolTip::instance()->addToolTip(&tankTips_.tankTip,
		float(posX_) - 10.0f, float(posY_) - 10.0f, 20.0f, 20.0f);

	// Draw the hightlighted ring around the tank
	bool currentTank = 
		(tank_ == ScorchedClient::instance()->getTankContainer().getCurrentTank() &&
		ScorchedClient::instance()->getGameState().getState() == ClientState::StatePlaying);
	if (currentTank)
	{
		GLState firstState(GLState::DEPTH_OFF);
		if (TargetRendererImplTankHUD::drawText())
		{
			Vector yellow(0.7f, 0.7f, 0.0f);
			GLWFont::instance()->getSmallPtFont()->draw(
				yellow, 10,
				(float) posX_ + 47.0f, (float) posY_ - 4.0f, (float) posZ_,
				TargetRendererImplTankHUD::getTextA());
			GLWFont::instance()->getSmallPtFont()->draw(
				yellow, 10,
				(float) posX_ + 47.0f, (float) posY_ - 15.0f, (float) posZ_,
				TargetRendererImplTankHUD::getTextB());

			if (TargetRendererImplTankHUD::getPercentage() >= 0.0f)
			{
				float totalHeight = 40.0f;
				float halfHeight = totalHeight / 2.0f;
				float height = totalHeight * TargetRendererImplTankHUD::getPercentage() / 100.0f;
				
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

	// Draw highlight around the tank
	if ((highlightType_ == ePlayerHighlight && tank_->getDestinationId()) ||
		(highlightType_ == eOtherHighlight && !tank_->getDestinationId() ))
	{
		TutorialDialog::instance()->drawHighlight(
			float(posX_) - 10.0f, float(posY_) - 10.0f, 20.0f, 20.0f);
	}
}
