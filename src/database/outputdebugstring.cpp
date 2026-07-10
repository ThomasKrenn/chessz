/****************************************************************************
** (C) Copyright 2016-20 Timitoo Systems GmbH. All Rights Reserved.
** Unauthorized copying of this file, via any medium is strictly prohibited
** Proprietary and confidential
**
** Written by Thomas Krenn <thomas.krenn@kabsi.at>, 2020
**
****************************************************************************/

#include <QtCore/QtGlobal>


#ifdef Q_OS_WIN        


#ifndef WINVER                  // Minimum platform is Windows 7
#define WINVER 0x0601
#endif

#ifndef _WIN32_WINNT            // Minimum platform is Windows 7
#define _WIN32_WINNT 0x0601
#endif

#ifndef _WIN32_WINDOWS          // Minimum platform is Windows 7
#define _WIN32_WINDOWS 0x0601
#endif

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <cstdarg>
#include "outputdebugstring.h"

void winDebugPrintFunc(const char * file, int line, const char *func) {

    char buf[1024] = { 0 };
    snprintf(&buf[0], sizeof(buf),"%s [%s:%d]\n", func, file, line);
    ::OutputDebugStringA(&buf[0]);
}

void winDebugPrintf(const char * format, ...) {

    char buf[1024*2] = { 0 };
    va_list args;
    va_start(args, format);
    vsnprintf( &buf[0], sizeof(buf), format, args);
    va_end(args);

    ::OutputDebugStringA( &buf[0] );
}


#endif
