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

#if !defined(AFX_ScoreDialog_H__1E583C82_CF52_4F2A_9AF5_7E2AC893EFE9__INCLUDED_)
#define AFX_ScoreDialog_H__1E583C82_CF52_4F2A_9AF5_7E2AC893EFE9__INCLUDED_

#include <GLW/GLWWindow.h>

class Tank;
class ScoreDialog : public GLWWindow
{
public:
	static ScoreDialog *instance();
	static ScoreDialog *instance2();

	// Inherited from GLWWindow
	virtual void display();
	virtual void windowInit(const unsigned state);
	virtual void draw();

protected:
	static ScoreDialog *instance_;
	static ScoreDialog *instance2_;
	std::list<unsigned int> sortedTanks_;
	int lastScoreValue_;
	int lastMoneyValue_;
	int lastNoPlayers_;

	void calculateScores();
	void addLine(Tank *current, float y, char *rank, bool finished);
	void addScoreLine(float y, Vector &color, int score);

private:
	ScoreDialog();
	virtual ~ScoreDialog();

};

#endif // !defined(AFX_ScoreDialog_H__1E583C82_CF52_4F2A_9AF5_7E2AC893EFE9__INCLUDED_)
