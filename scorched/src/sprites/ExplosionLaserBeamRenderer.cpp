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

#include <sprites/ExplosionLaserBeamRenderer.h>
#include <common/Defines.h>
#include <GLEXT/GLConsole.h>
#include <GLEXT/GLBitmap.h>


ExplosionLaserBeamRenderer::~ExplosionLaserBeamRenderer()
{
}

ExplosionLaserBeamRenderer::ExplosionLaserBeamRenderer(Vector &position, float size):
totalTime_(0), time_(0), size_(size), position_(position), angle_(0)
{
	for (int j=0;j<layers;j++){
		for(int i=0;i<sides;i++){
			points[j][i]=Vector((float)(360/sides)*i,(double)((size_/(layers+1))*(j+1)));
		}
	}
	_texture = 0;

	std::string file1 = getDataFile("data/textures/bordershield/grid22.bmp");
	std::string file2 = getDataFile("data/textures/bordershield/grid.bmp");
	
	GLBitmap map(file1.c_str(), file2.c_str(), true);
	_texture = new GLTexture;
	_texture->create(map, GL_RGBA, true);
	//GLConsole::instance()->addLine(false, "Size=%f", size_);




}

void ExplosionLaserBeamRenderer::draw(Action *action)
{
	GLState currentState(GLState::TEXTURE_ON | GLState::BLEND_ON);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glPushMatrix();	
	glTranslatef(position_[0],position_[1],0.0f);
	glScalef(time_*0.05f,time_*0.05f,1.0f);
	
	for (int j=0;j<layers;j++){
		glRotatef(angle_,0.0f,0.0f,1.0f);
		glBegin(GL_TRIANGLE_STRIP);
		int tempheight=(int)(time_*time_*time_);
		if (tempheight>100) tempheight=100;
		Vector height(0,0,tempheight);


		for (int i=0;i<(sides+1);i++){
			
			_texture->draw();
			glColor4f(1.0f,1.0f,1.8f, 0.5f);
			glNormal3fv ((float*)(points[j][i%sides]));
			if (i%2){
				glTexCoord2f(0.0f, 0.0f+time_);
			}else{
				glTexCoord2f(1.0f, 0.0f+time_);
			}
			glVertex3fv((float*)(points[j][i%sides]+height));
			glNormal3fv ((float*)(points[j][i%sides]));
			if (i%2){
				glTexCoord2f(0.0f, (float)(tempheight/10)+time_);
			}else{
				glTexCoord2f(1.0f, (float)(tempheight/10)+time_);
			}
			glVertex3fv((float*)(points[j][i%sides]));
		}
		for (int i=0;i<(sides+1);i++){
			
		_texture->draw();
		//GLState currentState(GLState::TEXTURE_ON);

			glColor4f(0.0f,0.5f,1.0f, 0.5f);
			
			glNormal3fv ((float*)(points[j][i%sides]));
			if (i%2){
				glTexCoord2f(0.0f, 0.0f-time_);
			}else{
				glTexCoord2f(1.0f, 0.0f-time_);
			}
			glVertex3fv((float*)(points[j][i%sides]));
			glNormal3fv ((float*)(points[j][i%sides]));
			if (i%2){
				glTexCoord2f(0.0f, (float)(tempheight/10)-time_);
			}else{
				glTexCoord2f(1.0f, (float)(tempheight/10)-time_);
			}
			glVertex3fv((float*)(points[j][i%sides]+height));
		}
		
		glEnd();
	}
	glPopMatrix();
	
}
void ExplosionLaserBeamRenderer::simulate(Action *action, float frameTime, bool &remove)
{
	totalTime_ += frameTime;
	time_ += frameTime;
	angle_=(angle_+3.0f);
	if (angle_>360.0f){
		angle_=0.0f;
	}
	if ((time_)>size_){
		remove=true;
	}else{
		remove=false;
	}

		
}

