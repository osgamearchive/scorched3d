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


// SmokeChain.cpp: implementation of the SmokeChain class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#include <GLEXT/GLState.h>
#include <common/OptionsTransient.h>
#include <landscape/GlobalHMap.h>
#include <landscape/SmokeChain.h>
#include <landscape/Landscape.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void SmokeChainEntry::set(Vector &position, Vector &velocity, float ms, float mt)
{
	position_ = position;
	velocity_ = velocity;
	maxSize = ms;
	maxTime = mt;
	a = 0; t = 0;
	texCoordType = (int) (RAND * 4.0f);
}

void SmokeChainEntry::draw(Vector &bilX, Vector &bilY)
{
	if (t < maxTime &&
		position_[0] < GlobalHMap::instance()->getHMap().getWidth() &&
		position_[0] > 0.0f &&
		position_[1] < GlobalHMap::instance()->getHMap().getWidth() &&
		position_[1] > 0.0f)
	{
	float f = a * 5.0f;
	if (a > maxSize) f = maxSize * 5.0f;
	float alpha = (1.0f - a) * 0.1f;
	float aboveGround =
		position_[2] - GlobalHMap::instance()->getHMap().getHeight((int) position_[0], (int) position_[1]);

	float smokeAlpha = alpha + .2f; if (smokeAlpha > 1.0f) smokeAlpha = 1.0f;
	Landscape::instance()->getShadowMap().
		addCircle(position_[0], position_[1], (f * aboveGround) / 10.0f, smokeAlpha);

		glPushMatrix();
			glTranslatef(position_[0], position_[1], position_[2]);
			glColor4f(1.0f, 1.0f, 1.0f, alpha);

			glBegin(GL_QUADS);
				
				switch(texCoordType)
				{
				default:
					glTexCoord2d(1.0f, 1.0f);
					break;
				case 1:
					glTexCoord2d(1.0f, 0.0f);
					break;
				case 2:
					glTexCoord2d(0.0f, 0.0f);
					break;
				case 3:
					glTexCoord2d(0.0f, 1.0f);
					break;
				}
				glVertex3fv(( bilX - bilY) * f);

				switch(texCoordType)
				{
				default:
					glTexCoord2d(1.0f, 0.0f);
					break;
				case 1:
					glTexCoord2d(0.0f, 0.0f);
					break;
				case 2:
					glTexCoord2d(0.0f, 1.0f);
					break;
				case 3:
					glTexCoord2d(1.0f, 1.0f);
					break;
				}
				glVertex3fv(( bilX + bilY) * f);

				switch(texCoordType)
				{
				default:
					glTexCoord2d(0.0f, 0.0f);
					break;
				case 1:
					glTexCoord2d(0.0f, 1.0f);
					break;
				case 2:
					glTexCoord2d(1.0f, 1.0f);
					break;
				case 3:
					glTexCoord2d(1.0f, 0.0f);
					break;
				}
				glVertex3fv((-bilX + bilY) * f);

			switch(texCoordType)
				{
				default:
					glTexCoord2d(0.0f, 1.0f);
					break;
				case 1:
					glTexCoord2d(1.0f, 1.0f);
					break;
				case 2:
					glTexCoord2d(1.0f, 0.0f);
					break;
				case 3:
					glTexCoord2d(0.0f, 0.0f);
					break;
				}
				glVertex3fv((-bilX - bilY) * f);

			glEnd();
		glPopMatrix();		

	}
}

bool SmokeChainEntry::move(float frameTime)
{
	position_[2] += 5.0f * frameTime;
	position_ += velocity_ * frameTime;
	velocity_ += OptionsTransient::instance()->getWindDirection() * 
		OptionsTransient::instance()->getWindSpeed() / 100.0f;
	velocity_ *= (float ) pow(0.9, frameTime);
	
	a += frameTime / 5.0f;
	t += frameTime;

	return (a < 1.0f);
}

SmokeChain::SmokeChain(int noEntries) : 
	noEntries_(0), maxNoEntries_(noEntries), iter_(0)
{
	firstEntry_ = nextEntry_ = entries_ = new SmokeChainEntry[noEntries];
	lastEntry_ = &entries_[noEntries - 1];
	memset(entries_, 0, sizeof(SmokeChainEntry) * noEntries);
}

SmokeChain::~SmokeChain()
{
	delete [] entries_;
}

SmokeChainEntry *SmokeChain::getNextEntry()
{
	SmokeChainEntry * result = 0;
	if (noEntries_ < maxNoEntries_ - 1)
	{
		noEntries_++;
		result = nextEntry_++;
		if (nextEntry_ > lastEntry_) nextEntry_ = entries_;
	}

	return result;
}

void SmokeChain::removeEntry()
{
	if (noEntries_ == 0) abort();
	noEntries_--;
	firstEntry_++;
	if (firstEntry_ > lastEntry_) firstEntry_ = entries_;
}

SmokeChainEntry *SmokeChain::getFirst()
{
	iter_ = firstEntry_;
	if (iter_ == nextEntry_) 
	{
		iter_ = 0;
	}

	return iter_;
}

SmokeChainEntry *SmokeChain::getNext()
{
	iter_++;
	if (iter_ > lastEntry_) iter_ = entries_;
	if (iter_ == nextEntry_) 
	{
		iter_ = 0;
	}

	return iter_;
}