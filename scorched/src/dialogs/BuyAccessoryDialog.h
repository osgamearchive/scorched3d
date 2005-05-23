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

#if !defined(AFX_BuyAccessoryDialog_H__4B5E93CF_1DE2_4979_A629_AEBD725ABE65__INCLUDED_)
#define AFX_BuyAccessoryDialog_H__4B5E93CF_1DE2_4979_A629_AEBD725ABE65__INCLUDED_

#include <map>
#include <set>
#include <vector>
#include <weapons/Weapon.h>
#include <GLW/GLWWindow.h>
#include <GLW/GLWButton.h>
#include <GLW/GLWPanel.h>
#include <GLW/GLWTab.h>
#include <GLW/GLWCheckBox.h>

class Tank;
class BuyAccessoryDialog : public GLWWindow,
						public GLWButtonI,
						public GLWCheckBoxI
{
public:
	BuyAccessoryDialog();
	virtual ~BuyAccessoryDialog();

	// Inherited from GLWButtonI
	virtual void draw();
	virtual void buttonDown(unsigned int id);
	virtual void windowInit(const unsigned state);

	// Inherited from GLWCheckBoxI
	virtual void stateChange(bool state, unsigned int id);

protected:
	unsigned int okId_;
	bool firstDrawTime_;
	GLWTab *buyWeaponTab_;
	GLWTab *buyOtherTab_;
	GLWTab *sellTab_;
	GLWTab *favouritesTab_;
	GLWPanel *topPanel_;
	GLWCheckBox *sortBox_;
	std::map<unsigned int, Accessory *> sellMap_;
	std::map<unsigned int, Accessory *> buyMap_;
	std::map<unsigned int, Accessory *> favMap_;
	std::set<std::string> favorites_;

	void loadFavorites();
	void saveFavorites();
	void playerRefresh();
	void playerRefreshKeepPos();
	void addPlayerName();
	void addPlayerWeapons();
	void addPlayerWeaponsSell();
	void addPlayerFavorites();
	void addPlayerWeaponsBuy(GLWTab *tab, bool weapons);
	void addAccessory(Tank *tank, GLWTab *tab, float height, Accessory *current);

};

#endif // !defined(AFX_BuyAccessoryDialog_H__4B5E93CF_1DE2_4979_A629_AEBD725ABE65__INCLUDED_)
