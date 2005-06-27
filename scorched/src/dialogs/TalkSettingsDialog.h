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

#if !defined(AFX_TalkSettingsDialog_H__4B5E93CF_1DE2_4979_A629_AEBD725ABE65__INCLUDED_)
#define AFX_TalkSettingsDialog_H__4B5E93CF_1DE2_4979_A629_AEBD725ABE65__INCLUDED_

#include <map>
#include <GLW/GLWWindow.h>
#include <GLW/GLWButton.h>
#include <GLW/GLWCheckBoxText.h>
#include <GLW/GLWTab.h>

class TalkSettingsDialog : 
	public GLWWindow, public GLWButtonI
{
public:
	static TalkSettingsDialog *instance();

	// Inherited from GLWButtonI
	virtual void buttonDown(unsigned int id);

	// Inherited from GLWWindow
	virtual void windowDisplay();

protected:
	static TalkSettingsDialog *instance_;

	unsigned int okId_;
	unsigned int muteAllId_, muteNoneId_;
	GLWTab *muteTab_;
	std::map<GLWCheckBoxText *, unsigned int> muteMap_;

	void addPlayers();

private:
	TalkSettingsDialog();
	virtual ~TalkSettingsDialog();
};

#endif // !defined(AFX_TalkSettingsDialog_H__4B5E93CF_1DE2_4979_A629_AEBD725ABE65__INCLUDED_)


