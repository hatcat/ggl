#if !defined PLATFORM_H
#define PLATFORM_H

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#if defined UNICODE
#define _U(a) L##a
#else
#define _U(a) a
#endif

#endif // PLATFORM_H