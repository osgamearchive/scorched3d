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


// PlayersDialog.h: interface for the PlayersDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PlayersDialog_H__DC09E024_0A12_40EA_8517_1A035CE741CA__INCLUDED_)
#define AFX_PlayersDialog_H__DC09E024_0A12_40EA_8517_1A035CE741CA__INCLUDED_

#include <GLW/GLWWindow.h>
#include <GLW/GLWDropDown.h>
#include <GLW/GLWButton.h>
#include <GLW/GLWTextBox.h>
#include <GLW/GLWFlag.h>
#include <common/FileLines.h>
#include <tankgraph/GLWTankViewer.h>

class PlayersDialog : public GLWWindow,
						  public GLWButtonI
{
public:
	static PlayersDialog *instance();

	// Inherited from GLWWindow
	virtual void windowInit(const unsigned state);

	// Inherited from GLWButtonI
	virtual void buttonDown(unsigned int id);

protected:
	static PlayersDialog *instance_;
	GLWTankViewer *viewer_;
	GLWTextBox *playerName_;
	FileLines playerNames_;
	GLWFlag *playerFlag_;
	GLWDropDown *dropDown_;
	unsigned int okId_;
	int currentPlayer_;

	void nextPlayer();

private:
	PlayersDialog();
	virtual ~PlayersDialog();

};

#endif // !defined(AFX_PlayersDialog_H__DC09E024_0A12_40EA_8517_1A035CE741CA__INCLUDED_)
