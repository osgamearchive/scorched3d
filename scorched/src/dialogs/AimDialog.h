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

#if !defined(AFX_AimDialog_H__083C6C16_1CD9_469C_AE4B_B44D159FB53C__INCLUDED_)
#define AFX_AimDialog_H__083C6C16_1CD9_469C_AE4B_B44D159FB53C__INCLUDED_

#include <GLW/GLWWindow.h>
#include <GLW/GLWSlider.h>
#include <GLW/GLWTracker.h>

class AimDialog : public GLWWindow,
	public GLWSliderI, public GLWTrackerI
{
public:
	static AimDialog *instance();

	virtual void draw();

	// Inherited from GLWSliderI
	virtual void currentChanged(unsigned int id, float value);

	// Inherited from GLWTrackerI
	virtual void currentChanged(unsigned int id, float valueX, float valueY);

protected:
	static AimDialog *instance_;
	GLWSlider *pwrSlider_;
	GLWTracker *tracker_;

private:
	AimDialog();
	virtual ~AimDialog();

};

#endif // !defined(AFX_AimDialog_H__083C6C16_1CD9_469C_AE4B_B44D159FB53C__INCLUDED_)
