#ifndef __SNPRINTF_SAMBA__
#define __SNPRINTF_SAMBA__

#if !defined(HAVE_SNPRINTF)
#include <stdio.h>
#include <stdarg.h>

#define snprintf smb_snprintf
#define vsnprintf smb_vsnprintf

#ifdef __cplusplus
extern "C" {
#endif

extern int snprintf(char *str, size_t size, const char *format, ...);
extern int vsnprintf(char *str, size_t size, const char *format, va_list ap);

#ifdef __cplusplus
}
#endif

#endif // HAVE_SNPRINTF

/*
#define _STD_USING
#ifdef sprintf
#undef sprintf
#endif 
#define sprintf __ERROR__XX__NEVER_USE_SPRINTF__;

#ifdef vsprintf
#undef vsprintf
#endif 
#define vsprintf __ERROR__XX__NEVER_USE_VSPRINTF__;
*/

extern const char *formatString(const char *format, ...);
extern const char *formatStringList(const char *format, va_list ap); 
extern char *s3d_stristr(const char *x, const char *y);

#endif // __SNPRINTF_SAMBA__
