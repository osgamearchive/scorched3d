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


// TankAIComputerDHTest.h: interface for the TankAIComputerTosser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TankAIComputerDHTest_H__2D4FA3B6_4C3B_48ED_9C04_0D8EC257C32D__INCLUDED_)
#define AFX_TankAIComputerDHTest_H__2D4FA3B6_4C3B_48ED_9C04_0D8EC257C32D__INCLUDED_

#include <tankai/TankAIComputerPShark.h>

class TankAIComputerDHTest : public TankAIComputerPShark
{
public:
	TankAIComputerDHTest(Tank *tank);
	virtual ~TankAIComputerDHTest();

TANKAI_DEFINE(DHTest, TankAIComputerDHTest);

protected:
	void selectWeapons();
};

#endif // !defined(AFX_TankAIComputerDHTest_H__2D4FA3B6_4C3B_48ED_9C04_0D8EC257C32D__INCLUDED_)
