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

#if !defined(AFX_DEFORMCIRCULAR_H__7E191509_3CD0_4EA5_A6B2_7C96C081C1AD__INCLUDED_)
#define AFX_DEFORMCIRCULAR_H__7E191509_3CD0_4EA5_A6B2_7C96C081C1AD__INCLUDED_

class Vector;
class ScorchedContext;

namespace DeformLandscape
{
	struct DeformPoints
	{
		float map[100][100];
	};

	bool deformLandscape(
		ScorchedContext &context,
		Vector &pos, float radius, 
		bool down, DeformPoints &map);

};

#endif // !defined(AFX_DEFORMCIRCULAR_H__7E191509_3CD0_4EA5_A6B2_7C96C081C1AD__INCLUDED_)
