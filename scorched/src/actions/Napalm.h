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


#if !defined(__INCLUDE_Napalmh_INCLUDE__)
#define __INCLUDE_Napalmh_INCLUDE__

#include <engine/ActionMeta.h>
#include <GLEXT/GLBilboardRenderer.h>
#include <list>

class Napalm : public ActionMeta
{
public:
	struct NapalmEntry 
	{
		NapalmEntry(int x, int y, int o) : offset(o), posX(x), posY(y)
		{
			renderEntry1.posX = float(posX) + 0.5f;
			renderEntry2.posX = float(posX) - 0.5f;
			renderEntry3.posX = float(posX);
			renderEntry1.posY = float(posY) - 0.2f;
			renderEntry2.posY = float(posY) - 0.2f;
			renderEntry3.posY = float(posY) + 0.5f;
			renderEntry1.width = renderEntry2.width = renderEntry3.width = 1.0f;
			renderEntry1.height = renderEntry2.height = renderEntry3.height = 2.0f;
		}

		int offset;
		int posX, posY;

		GLBilboardRenderer::Entry renderEntry1;
		GLBilboardRenderer::Entry renderEntry2;
		GLBilboardRenderer::Entry renderEntry3;
	};

	Napalm();
	Napalm(int x, int y, float napalmTime, bool hot,
		unsigned int playerId);
	virtual ~Napalm();

	virtual void init();
	virtual void simulate(float frameTime, bool &remove);
	virtual bool writeAction(NetBuffer &buffer);
	virtual bool readAction(NetBufferReader &reader);

	std::list<NapalmEntry *> &getPoints() { return napalmPoints_; }

REGISTER_ACTION_HEADER(Napalm);

protected:
	int x_, y_;
	bool hot_;
	float napalmTime_;
	unsigned int playerId_;
	float totalTime_, hurtTime_;

	// Not sent bu wire
	bool hitWater_;
	std::list<NapalmEntry *> napalmPoints_;

	float getHeight(int x, int y);
	void simulateAddStep();
	void simulateRmStep();
	void simulateDamage();

};


#endif
