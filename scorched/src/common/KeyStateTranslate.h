#include <SDL/SDL_keysym.h>

struct KeyStateTranslation
{
	const char *keyStateName;
	unsigned int keyStateSym;
} KeyStateTranslationTable [] =
{
    "NONE", KMOD_NONE,
	"LCTRL", KMOD_LCTRL,
	"RCTRL", KMOD_RCTRL,
	"RSHIFT", KMOD_RSHIFT,
	"LSHIFT", KMOD_LSHIFT,
	"RALT", KMOD_RALT,
	"LALT", KMOD_LALT
};
