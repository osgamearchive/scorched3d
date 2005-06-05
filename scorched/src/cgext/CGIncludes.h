#if !defined(__INCLUDE_CGIncludesh_INCLUDE__)
#define __INCLUDE_CGIncludesh_INCLUDE__

#ifdef HAVE_CG

#include <cg/cg.h>
#include <cg/cggl.h>

#else

// Not sure if this is a good way of doing this, time will tell...

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

static CGparameter cgGetNamedParameter(CGprogram, const char *name) 
	{ return CGparameter(); }

static CGprofile cgGLGetLatestProfile(CGenum) { return 0; }
static void cgGLEnableProfile(CGprofile) {}
static void cgGLDisableProfile(CGprofile) {}
static void cgGLSetOptimalOptions(CGprofile) {}
static void cgGLBindProgram(CGprogram) {}
static void cgGLLoadProgram(CGprogram) {}
static void cgGLEnableTextureParameter(CGparameter) {}
static void cgGLDisableTextureParameter(CGparameter) {}

static void cgGLSetParameter3f(CGparameter, float, float, float) {}
static void cgGLSetParameter4f(CGparameter, float, float, float, float) {}
static void cgGLSetStateMatrixParameter(CGparameter, CGenum, CGenum) {}
static void cgGLSetTextureParameter(CGparameter, unsigned int) {}

static CGerror cgGetError() { return CG_SOURCE; }
static const char *cgGetErrorString(CGerror) { return ""; }

static CGcontext cgCreateContext() { return 0; }
static void cgDestroyContext(CGcontext) {}

static CGprogram cgCreateProgramFromFile(CGcontext context,
	CGenum programType,
	const char* program,
	CGprofile profile,
	const char* entry,
	int) { return 0; }


#endif
#endif // __INCLUDE_CGIncludesh_INCLUDE__
