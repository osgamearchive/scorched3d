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

#if !defined(AFX_TARGETCAMERA_H__97593EBB_5901_4D81_BAEB_8ADC76CFB627__INCLUDED_)
#define AFX_TARGETCAMERA_H__97593EBB_5901_4D81_BAEB_8ADC76CFB627__INCLUDED_

#include <GLEXT/GLCamera.h>
#include <engine/GameState.h>
#include <common/Keyboard.h>

class TargetCamera 
{
public:
	enum CamType
	{
		CamTop = 0,
		CamBehind,
		CamTank,
		CamGun,
		CamAction,
		CamLeft,
		CamRight,
		CamLeftFar,
		CamRightFar,
		CamSpectator,
		CamFree
	};

	TargetCamera();
	virtual ~TargetCamera();

	GLCamera &getCamera() { return mainCam_; }
	CamType getCameraType() { return cameraPos_; }
	void setCameraType(CamType type) { cameraPos_ = type; }
	void resetCam();

	void simulate(float frameTime);
	void draw();
	void mouseWheel(short z, bool &skipRest);
	void mouseDown(GameState::MouseButton button, 
		int x, int y, bool &skipRest);
	void mouseDrag(GameState::MouseButton button,
		int mx, int my, int x, int y, bool &skipRest);
	void keyboardCheck(
		float frameTime, 
		char *buffer, unsigned int keyState,
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);

	static const char **getCameraNames();
	static int getNoCameraNames();
	static float heightFunc(int x, int y, void *heightData);

protected:
	GLCamera mainCam_;
	CamType cameraPos_;

	bool moveCamera(float frameTime);

};

#endif // !defined(AFX_TARGETCAMERA_H__97593EBB_5901_4D81_BAEB_8ADC76CFB627__INCLUDED_)

