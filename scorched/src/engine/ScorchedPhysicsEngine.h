#pragma once

#include <engine/PhysicsEngine.h>
#include <engine/ScorchedContext.h>

class HeightMapCollision;
class SkyRoofCollision;
class ScorchedCollisionHandler;
class ScorchedPhysicsEngine : public PhysicsEngine
{
public:
	ScorchedPhysicsEngine();
	virtual ~ScorchedPhysicsEngine();

	void setScorchedContext(ScorchedContext *context);
	void resetContext();
	void generate();

protected:
	ScorchedContext *context_;
	HeightMapCollision *hmcol_;
	SkyRoofCollision *srcol_;
	ScorchedCollisionHandler *sccol_;

	void setWind(Vector &wind);
};
