#include <boids/ScorchedBoidsObstacle.h>
#include <client/ScorchedClient.h>
#include <common/Line.h>
#include <landscape/LandscapeMaps.h>
#include <landscape/HeightMap.h>

ScorchedBoidsObstacle::ScorchedBoidsObstacle()
{
}

ScorchedBoidsObstacle::~ScorchedBoidsObstacle()
{
}

ISectData ScorchedBoidsObstacle::IntersectionWithRay(const BoidVector & raydirection,
		const BoidVector &rayorigin) const
{
	ISectData result;
	result.intersectionflag = 0;

	Vector direction(
		(float) raydirection.x, 
		(float) raydirection.z, 
		(float) raydirection.y);
	direction.StoreNormalize();
	direction *= 2.0f;
	Vector position(
		(float) rayorigin.x, 
		(float) rayorigin.z, 
		(float) rayorigin.y);

	for (int i = 0; i < 12; i++)
	{
		position += direction;
		if (position[0] < 0.0f || position[0] > 256.0 ||
			position[1] < 0.0f || position[1] > 256.0) break;
	
		if (ScorchedClient::instance()->getLandscapeMaps().getHMap().
			getHeight((int) position[0], (int) position[1]) >
			position[2] - 3.0f)
		{
			position -= direction;
			Vector &normal = ScorchedClient::instance()->getLandscapeMaps().getHMap().
				getNormal((int) position[0], (int) position[1]);

			result.intersectionflag = 1;
			result.point.x = position[0];
			result.point.y = position[2];
			result.point.z = position[1];
			result.normal.x = normal[0];
			result.normal.y = normal[2];
			result.normal.z = normal[1];
			break;
		}
	}

	return result;
}
