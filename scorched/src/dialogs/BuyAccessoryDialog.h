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


// BuyAccessoryDialog.h: interface for the BuyAccessoryDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BuyAccessoryDialog_H__4B5E93CF_1DE2_4979_A629_AEBD725ABE65__INCLUDED_)
#define AFX_BuyAccessoryDialog_H__4B5E93CF_1DE2_4979_A629_AEBD725ABE65__INCLUDED_

#pragma warning(disable: 4786)

#include <map>
#include <vector>
#include <weapons/Weapon.h>
#include <GLW/GLWWindow.h>
#include <GLW/GLWButton.h>
#include <GLW/GLWVisiblePanel.h>
#include <GLW/GLWTab.h>

class BuyAccessoryDialog : public GLWWindow,
						public GLWButtonI
{
public:
	BuyAccessoryDialog();
	virtual ~BuyAccessoryDialog();

	// Inherited from GLWButtonI
	virtual void buttonDown(unsigned int id);
	virtual void windowInit(const unsigned state);

protected:
	unsigned int okId_;
	GLWTab *buyWeaponTab_;
	GLWTab *buyOtherTab_;
	GLWTab *sellTab_;
	GLWVisiblePanel *topPanel_;
	int currentPlayer_;
	std::map<unsigned int, Accessory *> sellMap_;
	std::map<unsigned int, Accessory *> buyMap_;

	void finished();
	void nextPlayer();

	void setupWindow();
	void playerRefresh();
	void addPlayerName();
	void addPlayerWeapons();
	void addPlayerWeaponsSell();
	void addPlayerWeaponsBuy(GLWTab *tab, bool weapons);
	void sort(std::vector<Accessory *> &accVector);

};

#endif // !defined(AFX_BuyAccessoryDialog_H__4B5E93CF_1DE2_4979_A629_AEBD725ABE65__INCLUDED_)
