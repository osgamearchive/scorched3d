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


// WaterMapModifier.h: interface for the WaterMapModifier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WATERMAPMODIFIER_H__AD3CF4DB_FB59_4D0F_89B5_A4536C7E7DFC__INCLUDED_)
#define AFX_WATERMAPMODIFIER_H__AD3CF4DB_FB59_4D0F_89B5_A4536C7E7DFC__INCLUDED_


#include <landscape/HeightMap.h>
#include <landscape/WaterMap.h>

namespace WaterMapModifier  
{
	void addWaterVisibility(HeightMap &hMap, WaterMap &wMap, ProgressCounter *counter = 0);
};

#endif // !defined(AFX_WATERMAPMODIFIER_H__AD3CF4DB_FB59_4D0F_89B5_A4536C7E7DFC__INCLUDED_)
