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

#if !defined(AFX_ASEMODEL_H__5164D8CF_5506_4E58_BB87_A89EDC315B0B__INCLUDED_)
#define AFX_ASEMODEL_H__5164D8CF_5506_4E58_BB87_A89EDC315B0B__INCLUDED_

#pragma warning(disable: 4786)

#include <3dsparse/Model.h>

class ASEModel : public Model
{
public:
	ASEModel(char *name, char *texture);
	virtual ~ASEModel();

	// Used by parser
	void setTmRow(Vector &row, int index);
	virtual void setFaceNormal(Vector &normal, int face, int index);
	
protected:
	Vector tmRow_[3];

};

#endif // !defined(AFX_ASEMODEL_H__5164D8CF_5506_4E58_BB87_A89EDC315B0B__INCLUDED_)
