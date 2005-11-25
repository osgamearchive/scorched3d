#include <engine/ScorchedPhysicsEngine.h>
#include <engine/ScorchedCollisionHandler.h>
#include <landscape/HeightMapCollision.h>
#include <landscape/SkyRoofCollision.h>
#include <common/OptionsGame.h>
#include <common/OptionsTransient.h>

ScorchedPhysicsEngine::ScorchedPhysicsEngine() :
	hmcol_(0), srcol_(0), sccol_(0), context_(0)
{
}

ScorchedPhysicsEngine::~ScorchedPhysicsEngine()
{
}

void ScorchedPhysicsEngine::setScorchedContext(ScorchedContext *context)
{
	context_ = context;

	hmcol_ = new HeightMapCollision(context);
	srcol_ = new SkyRoofCollision(context);
	sccol_ = new ScorchedCollisionHandler(context);
	setCollisionHandler(sccol_);

	hmcol_->setContext(context_);
	srcol_->setContext(context_);
}

void ScorchedPhysicsEngine::resetContext()
{
	HeightMapCollision::setContext(context_);
 	SkyRoofCollision::setContext(context_);
}

void ScorchedPhysicsEngine::setWind(Vector &wind)
{
	DIALOG_ASSERT(context_);
	float gravity = (float) context_->optionsGame->getGravity();
	Vector gravityVec = wind;
	gravityVec[2] += gravity;

	setGravity(gravityVec);
}

void ScorchedPhysicsEngine::generate()
{
	// Set the wind for the next shot
	Vector wind;
	if (context_->optionsTransient->getWindOn())
	{
		wind = context_->optionsTransient->getWindDirection();
		wind *= context_->optionsTransient->getWindSpeed() / 2.0f;
	}
	setWind(wind);

	// Set the walls
	hmcol_->generate();
}
