////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2004
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

#include <engine/Particle.h>

Particle::Particle()
{
	setDead();
}

Particle::~Particle()
{
}

void Particle::setDead()
{
	life_ = -1.0f;
	renderer_ = 0;
}

void Particle::setParticle(
	float life, float mass, float friction,
    Vector &velocity, Vector &gravity,
	Vector &color, Vector &colorCounter,
	Vector &size, Vector &sizeCounter,
	float alpha, float alphaCounter)
{
	life_ = life; mass_ = mass; friction_ = friction;
	velocity_ = velocity; gravity_ = gravity;
	color_ = color; colorCounter_ = colorCounter;
	size_ = size; sizeCounter_ = sizeCounter;
	alpha_ = alpha; alphaCounter_ = alphaCounter;
}
