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


// TankAIHuman.h: interface for the TankAIHuman class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TANKAIHUMAN_H__F71C91A5_B74D_4109_8F7B_ACD471025C28__INCLUDED_)
#define AFX_TANKAIHUMAN_H__F71C91A5_B74D_4109_8F7B_ACD471025C28__INCLUDED_

#include <tank/Tank.h>

class TankAIHuman : public TankAI
{
public:
	TankAIHuman(Tank *tank);
	virtual ~TankAIHuman();

	virtual void playMove(const unsigned state, 
		float frameTime, char *buffer);

protected:
	Tank *currentTank_;

	void movePower(char *buffer, float mult);
	void moveUpDown(char *buffer, float mult);
	void moveLeftRight(char *buffer, float mult);
	void fireShot();

};

#endif // !defined(AFX_TANKAIHUMAN_H__F71C91A5_B74D_4109_8F7B_ACD471025C28__INCLUDED_)
