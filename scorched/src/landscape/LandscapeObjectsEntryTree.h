#if !defined(__INCLUDE_LandscapeObjectsEntryTreeh_INCLUDE__)
#define __INCLUDE_LandscapeObjectsEntryTreeh_INCLUDE__

#include <landscape/LandscapeObjectsEntry.h>
#include <GLEXT/GLState.h>
#include <GLEXT/GLTexture.h>

class LandscapeObjectsEntryTree : public LandscapeObjectsEntry
{
public:
	static GLuint treePine, treePineBurnt, treePineSnow;
	static GLuint treePineSmall, treePineBurntSmall, treePineSnowSmall;
	static GLuint treePalm, treePalmBurnt;
	static GLuint treePalmSmall, treePalmBurntSmall;
	static GLTexture texture_;

	bool pine;
	bool snow;

	virtual void render(float distance);
};

#endif // __INCLUDE_LandscapeObjectsEntryTreeh_INCLUDE__
