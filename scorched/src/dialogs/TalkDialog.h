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

#if !defined(__INCLUDE_TalkDialogh_INCLUDE__)
#define __INCLUDE_TalkDialogh_INCLUDE__

#include <GLW/GLWWindow.h>
#include <GLW/GLWTextButton.h>
#include <GLW/GLWTextBox.h>
#include <GLW/GLWCheckBox.h>

class TalkDialog : public GLWWindow,
						  public GLWButtonI
{
public:
	static TalkDialog *instance();

	// Inherited from GLWButtonI
	virtual void draw();
	virtual void buttonDown(unsigned int id);
	virtual void windowDisplay();

	void setTeamTalk();

	// Inherited from GLWWindow
	virtual void keyDown(char *buffer, unsigned int keyState, 
		KeyboardHistory::HistoryElement *history, int hisCount, 
		bool &skipRest);

protected:
	static TalkDialog *instance_;
	GLWTextBox *talkText_;
	GLWTextButton *ok_, *cancel_, *mute_;
	GLWCheckBox *teamBox_;

private:
	TalkDialog();
	virtual ~TalkDialog();

};

class TeamTalkDialog : public GLWWindow
{
public:
	static TeamTalkDialog *instance();

	virtual void windowDisplay();

protected:
	static TeamTalkDialog *instance_;

private:
	TeamTalkDialog();
	virtual ~TeamTalkDialog();

};

#endif
