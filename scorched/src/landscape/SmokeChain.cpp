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

#include <math.h>
#include <common/OptionsTransient.h>
#include <sprites/ExplosionTextures.h>
#include <client/ScorchedClient.h>
#include <landscape/SmokeChain.h>
#include <landscape/Landscape.h>

void SmokeChainEntry::set(Vector &position, Vector &velocity, float ms, float mt)
{
	posX = position[0];
	posY = position[1];
	posZ = position[2];
	velocity_ = velocity;
	maxSize = ms;
	maxTime = mt;
	a = 0; t = 0;
	texture = &ExplosionTextures::instance()->smokeTexture;
	textureCoord = (int) (RAND * 4.0f);
}

void SmokeChainEntry::draw()
{
	if (t < maxTime &&
		posX < ScorchedClient::instance()->getLandscapeMaps().getHMap().getWidth() &&
		posX > 0.0f &&
		posY < ScorchedClient::instance()->getLandscapeMaps().getHMap().getWidth() &&
		posY > 0.0f)
	{
		// Calculate the size of the smoke
		width = height = a * 5.0f;
		if (a > maxSize) width = height = maxSize * 5.0f;

		// Calculate the actual transparency of the smoke
		alpha = (1.0f - a) * 0.4f;

		// Add a shadow of the smoke on the ground
		if (posX > 0.0f && posY > 0.0f && 
			posX < 255.0f && posY < 255.0f)
		{
			float aboveGround =
				posZ - ScorchedClient::instance()->getLandscapeMaps().getHMap().getHeight(
				int (posX), int(posY));
			float smokeAlpha = alpha + .2f; if (smokeAlpha > 1.0f) smokeAlpha = 1.0f;
			Landscape::instance()->getShadowMap().
				addCircle(posX, posY, (height * aboveGround) / 10.0f, smokeAlpha);
		}
	}
}

bool SmokeChainEntry::move(float frameTime)
{
	posZ += 5.0f * frameTime;
	posX += velocity_[0] * frameTime;
	posY += velocity_[1] * frameTime;
	posZ += velocity_[2] * frameTime;
	velocity_ += ScorchedClient::instance()->getOptionsTransient().getWindDirection() * 
		ScorchedClient::instance()->getOptionsTransient().getWindSpeed() / 100.0f;
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

		GLBilboardRenderer::instance()->addEntry(result);
	}

	return result;
}

void SmokeChain::removeEntry()
{
	if (noEntries_ == 0) return;
	noEntries_--;

	GLBilboardRenderer::instance()->removeEntry(firstEntry_, false);
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
