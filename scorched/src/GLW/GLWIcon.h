#pragma once

#include <GLEXT/GLTexture.h>
#include <GLW/GLWVisibleWidget.h>

class GLWIcon : public GLWVisibleWidget
{
public:
	GLWIcon(float x, float y, float w, float h, GLTexture *texture);
	virtual ~GLWIcon();

	virtual void draw();

METACLASSID
protected:
	GLTexture *texture_;

};
