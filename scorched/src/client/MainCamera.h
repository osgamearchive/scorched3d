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


// MainCamera.h: interface for the MainCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINCAMERA_H__97593EBB_5901_4D81_BAEB_8ADC76CFB627__INCLUDED_)
#define AFX_MAINCAMERA_H__97593EBB_5901_4D81_BAEB_8ADC76CFB627__INCLUDED_


#include <GLEXT/GLCamera.h>
#include <GLEXT/GLMenuI.h>
#include <engine/GameStateI.h>

class MainCamera : public GameStateI,
					public GLMenuI
{
public:
	static MainCamera *instance();
	enum CamType
	{
		CamTop = 0,
		CamBehind,
		CamTank,
		CamGun,
		CamLeft,
		CamRight,
		CamLeftFar,
		CamRightFar,
		CamSpectator,
		CamFree
	};

	GLCamera &getCamera() { return mainCam_; }
	void resetCam();

	// Inherited from GameStateI
	virtual void simulate(const unsigned state, 
		float frameTime);
	virtual void draw(const unsigned state);
	virtual void mouseWheel(const unsigned state, 
		short z, bool &skipRest);
	virtual void mouseDown(const unsigned state, 
		GameState::MouseButton button, int x, int y, bool &skipRest);
	virtual void mouseDrag(const unsigned state, 
		GameState::MouseButton button,
		int mx, int my, int x, int y, bool &skipRest);
	virtual void keyboardCheck(
		const unsigned state, float frameTime, 
		char *buffer, int bufCount,
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);

	// Inherited from GLMenuI
	virtual void menuSelection(const char* menuName, 
		const int position, const char *menuItem);
	virtual void getMenuItems(const char* menuName, 
		std::list<GLMenuItem> &result);

	static float heightFunc(int x, int y);

	// Class to save the screen shots
	class SaveScreen : public GameStateI
	{
	public:
		SaveScreen() : saveScreen_(false) {}
		virtual void draw(const unsigned state);

		bool saveScreen_;
	} saveScreen_;

protected:
	static MainCamera *instance_;
	GLCamera mainCam_;
	CamType cameraPos_;

	void moveCamera(float frameTime);

private:
	MainCamera();
	virtual ~MainCamera();

};

#endif // !defined(AFX_MAINCAMERA_H__97593EBB_5901_4D81_BAEB_8ADC76CFB627__INCLUDED_)
