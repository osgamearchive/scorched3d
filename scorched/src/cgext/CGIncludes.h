#if !defined(__INCLUDE_CGIncludesh_INCLUDE__)
#define __INCLUDE_CGIncludesh_INCLUDE__

#ifdef HAVE_CG

#include <cg/cg.h>
#include <cg/cggl.h>

#else

enum CGenum
{
	CG_GL_FRAGMENT,
	CG_GL_MATRIX_IDENTITY,
	CG_GL_MODELVIEW_PROJECTION_MATRIX,
	CG_GL_MODELVIEW_MATRIX,
	CG_GL_VERTEX,
	CG_PROFILE_UNKNOWN,
	CG_SOURCE
};
typedef CGenum CGerror;

typedef unsigned CGprofile;
typedef unsigned CGprogram;
typedef unsigned CGcontext;
typedef unsigned CGparameter;

CGparameter cgGetNamedParameter(CGprogram, const char *name) 
	{ return CGparameter(); }

CGprofile cgGLGetLatestProfile(CGenum) { return 0; }
void cgGLEnableProfile(CGprofile) {}
void cgGLDisableProfile(CGprofile) {}
void cgGLSetOptimalOptions(CGprofile) {}
void cgGLBindProgram(CGprogram) {}
void cgGLLoadProgram(CGprogram) {}
void cgGLEnableTextureParameter(CGparameter) {}
void cgGLDisableTextureParameter(CGparameter) {}

void cgGLSetParameter3f(CGparameter, float, float, float) {}
void cgGLSetParameter4f(CGparameter, float, float, float, float) {}
void cgGLSetStateMatrixParameter(CGparameter, CGenum, CGenum) {}
void cgGLSetTextureParameter(CGparameter, unsigned int) {}

CGerror cgGetError() { return CG_SOURCE; }
const char *cgGetErrorString(CGerror) { return ""; }

CGcontext cgCreateContext() { return 0; }
void cgDestroyContext(CGcontext) {}

CGprogram cgCreateProgramFromFile(CGcontext context,
	CGenum programType,
	const char* program,
	CGprofile profile,
	const char* entry,
	int) { return 0; }


#endif
#endif // __INCLUDE_CGIncludesh_INCLUDE__
