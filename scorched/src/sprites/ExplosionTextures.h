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


// ExplosionTextures.h: interface for the ExplosionTextures class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPLOSIONTEXTURES_H__F2BC42E7_B2FB_4C0D_ACF0_0B91D8DC84D3__INCLUDED_)
#define AFX_EXPLOSIONTEXTURES_H__F2BC42E7_B2FB_4C0D_ACF0_0B91D8DC84D3__INCLUDED_

#include <GLEXT/GLTextureSet.h>
#include <common/ProgressCounter.h>
#include <map>

class ExplosionTextures  
{
public:
	static ExplosionTextures *instance();
	void createTextures(ProgressCounter *counter = 0);

	GLTextureSet exp00;
	GLTextureSet exp01;
	GLTextureSet exp02;
	GLTextureSet exp03;
	GLTextureSet exp04;
	GLTextureSet exp05;

	GLTextureSet flames;

	GLTexture smokeTexture;
	GLTexture smokeTexture2;

	std::map<const char *, GLTextureSet*> textureSets;

protected:
	static ExplosionTextures *instance_;

	void addTextureToSet(GLTextureSet &set,
						const char *texPath);
	void createTextureSet(GLTextureSet &set,
						int number, 
						const char *baseName,
						bool zeros = false);
private:
	ExplosionTextures();
	virtual ~ExplosionTextures();

};

#endif // !defined(AFX_EXPLOSIONTEXTURES_H__F2BC42E7_B2FB_4C0D_ACF0_0B91D8DC84D3__INCLUDED_)
