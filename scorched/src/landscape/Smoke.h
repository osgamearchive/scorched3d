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


// Smoke.h: interface for the Smoke class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMOKE_H__6265A8BD_0FB0_4CB0_BDDC_6C706B2A2A07__INCLUDED_)
#define AFX_SMOKE_H__6265A8BD_0FB0_4CB0_BDDC_6C706B2A2A07__INCLUDED_

#include <GLEXT/GLTexture.h>
#include <landscape/SmokeChain.h>

class SmokeCounter 
{
public:
	SmokeCounter(float minTime, float timeDiff);

	bool nextDraw(float frameTime);

protected:
	float minTime_, timeDiff_;
	float currentTime_;
	float nextTime_;

	void genNextTime();
};

class Smoke
{ 
public:
	Smoke();
	virtual ~Smoke();

	void addSmoke(float x, float y, float z,
			float dx = 0.0f, float dy = 0.0f, float dz = 0.0f,
			float maxSize = 0.5f, float maxTime = 5.0f);

	bool noSmoke();
	void removeAllSmokes();

	virtual void draw();
	virtual void simulate(float frameTime);

protected:
	static Smoke *instance_;
	SmokeChain chain_;

};

#endif // !defined(AFX_SMOKE_H__6265A8BD_0FB0_4CB0_BDDC_6C706B2A2A07__INCLUDED_)
