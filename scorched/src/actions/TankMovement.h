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

#if !defined(__INCLUDE_TankMovementh_INCLUDE__)
#define __INCLUDE_TankMovementh_INCLUDE__

#include <engine/ActionMeta.h>
#include <engine/ViewPoints.h>
#include <common/Vector.h>
#include <list>
#include <map>

class VirtualSoundSource;
class Tank;
class TankMovement : public ActionMeta
{
public:
	static std::map<unsigned int, TankMovement*> movingTanks;

	TankMovement();
	TankMovement(unsigned int playerId,
		int positionX, int positionY);
	virtual ~TankMovement();

	virtual void init();
	virtual void simulate(float frameTime, bool &remove);
	virtual bool writeAction(NetBuffer &buffer);
	virtual bool readAction(NetBufferReader &reader);

	void remove();

	REGISTER_ACTION_HEADER(TankMovement);

protected:
	// A list containing smooth positions
	struct PositionEntry
	{
		PositionEntry(float newx, float newy, float newa, bool useF,
			float *ha, float *hb) :
		x(newx), y(newy), ang(newa), useFuel(useF), heighta(ha), heightb(hb)
		{}

		float x, y;
		float ang;
		bool useFuel;
		float *heighta, *heightb;
	};
	std::list<PositionEntry> expandedPositions_;
	float timePassed_;
	ViewPoints::ViewPoint *vPoint_;
	VirtualSoundSource *moveSoundSource_;

	unsigned int playerId_;
	int positionX_, positionY_;
	bool remove_, moving_;

	void moveTank(Tank *tank);
	void simulationMove(float frameTime);

};

#endif
