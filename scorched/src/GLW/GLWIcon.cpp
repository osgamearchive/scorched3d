#include <GLW/GLWIcon.h>
#include <GLEXT/GLState.h>

GLWIcon::GLWIcon(float x, float y, float w, float h, GLTexture *texture) : 
	GLWVisibleWidget(x, y, w, h),
	texture_(texture)
{
}

GLWIcon::~GLWIcon()
{
}

void GLWIcon::draw()
{
	if (texture_)
	{
		GLState state(GLState::TEXTURE_ON);
		glColor3f(1.0f, 1.0f, 1.0f);
		texture_->draw();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(x_, y_);
			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(x_ + w_, y_);
			glTexCoord2f(1.0f, 1.0f);
			glVertex2f(x_ + w_, y_ + h_);
			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(x_, y_ + h_);
		glEnd();
	}

	GLWVisibleWidget::draw();
}
