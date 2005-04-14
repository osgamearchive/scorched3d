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

#include <tankgraph/TracerStore.h>
#include <tank/TankContainer.h>
#include <client/ScorchedClient.h>

TracerStore::TracerStore() : current_(0), listNo_(0)
{
	obj_ = gluNewQuadric();
}

TracerStore::~TracerStore()
{
	gluDeleteQuadric(obj_);
}

void TracerStore::clearTracers()
{
	if (current_) 
	{
		current_->lines.clear();
		current_->points.clear();
	}
}

void TracerStore::draw(const unsigned state)
{
	Tank *current = ScorchedClient::instance()->getTankContainer().getCurrentTank();
	if (!current) return;

	if (!current_ || 
		current_->tank != current->getPlayerId())
	{
		std::map<unsigned int, TraceEntry>::iterator itor = 
			traceEntries_.find(current->getPlayerId());
		if (itor == traceEntries_.end())
		{
			TraceEntry entry(current->getPlayerId());
			traceEntries_[current->getPlayerId()] = entry;
			current_ = &traceEntries_[current->getPlayerId()];
		}
		else
		{
			current_ = &(*itor).second;
		}
	}
	if (current_->points.empty()) return;

	glColor3fv(current->getColor());
	
	std::list<Vector>::iterator itor = current_->points.begin();
	std::list<Vector>::iterator itorend = current_->points.end();
	for (;itor != itorend; itor++)
	{
		drawTracerEnd(*itor);
	}

	std::list<std::list<Vector> >::iterator itor2 = current_->lines.begin();
	std::list<std::list<Vector> >::iterator itorend2 = current_->lines.end();
	for (;itor2 != itorend2; itor2++)
	{
		drawSmokeTracer(*itor2);
	}
}

void TracerStore::drawTracerEnd(Vector &position)
{
	GLState currentState(GLState::TEXTURE_OFF | GLState::BLEND_OFF);

	glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);
		if (!listNo_)
		{
			glNewList(listNo_ = glGenLists(1), GL_COMPILE_AND_EXECUTE);
				gluSphere(obj_, 0.5f, 4, 2);
			glEndList();
		}
		else
		{
			glCallList(listNo_);
		}
	glPopMatrix();
}

void TracerStore::drawSmokeTracer(std::list<Vector> &positions)
{
	GLState currentState(GLState::TEXTURE_OFF | GLState::BLEND_OFF);
	
	glBegin(GL_LINES);
	std::list<Vector>::iterator itor = positions.begin();
	std::list<Vector>::iterator itorend = positions.end();
	for (;itor != itorend; itor++)
	{
		Vector &startPos = *itor;
		itor++;
		if (itor != itorend)
		{
			Vector &endPos = *itor;
				if (fabs(startPos[0] - endPos[0]) < 100.0f &&
				fabs(startPos[1] - endPos[1]) < 100.0f)
			{
				glVertex3fv(startPos);
				glVertex3fv(endPos);
			}
		}
		else break;
	}
	glEnd();
}

void TracerStore::newGame()
{
	traceEntries_.clear();
	current_ = 0;
}

void TracerStore::addTracer(unsigned int tank, Vector &position)
{
	std::map<unsigned int, TraceEntry>::iterator itor = 
		traceEntries_.find(tank);
	if (itor == traceEntries_.end())
	{
		TraceEntry entry(tank);
		entry.points.push_back(position);
		traceEntries_[tank] = entry;
	}
	else
	{
		(*itor).second.points.push_back(position);
	}
}

void TracerStore::addSmokeTracer(unsigned int tank, Vector &position, std::list<Vector> &positions)
{
	std::map<unsigned int, TraceEntry>::iterator itor = 
		traceEntries_.find(tank);
	if (itor == traceEntries_.end())
	{
		TraceEntry entry(tank);
		entry.lines.push_back(positions);
		entry.points.push_back(position);
		traceEntries_[tank] = entry;
	}
	else
	{
		(*itor).second.lines.push_back(positions);
		(*itor).second.points.push_back(position);
	}
}
