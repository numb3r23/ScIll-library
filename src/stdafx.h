/*
 * stdafx.h
 *
 *  Created on: Feb 13, 2011
 *      Author: numb3r23
 */

#ifndef STDAFX_H_
#define STDAFX_H_

#include <iostream>

// thx :D
// http://stackoverflow.com/questions/3907818/opengl-headers-for-os-x-linux

#ifdef __APPLE__
    #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
    #include <OpenGL/gl3.h>
#else
    #ifdef _WIN32
        #define NOMINMAX
        #include <windows.h>
		#include <GL/glew.h>
    #endif
    #include <GL/glu.h>
    #include <GL/gl.h>
#endif

// thx :D
// http://msinttypes.googlecode.com/svn/trunk/stdint.h
#if (_MSC_VER < 1300)
   typedef signed char       int8_t;
   typedef signed short      int16_t;
   typedef signed int        int32_t;
   typedef unsigned char     uint8_t;
   typedef unsigned short    uint16_t;
   typedef unsigned int      uint32_t;
#else
   typedef signed __int8     int8_t;
   typedef signed __int16    int16_t;
   typedef signed __int32    int32_t;
   typedef unsigned __int8   uint8_t;
   typedef unsigned __int16  uint16_t;
   typedef unsigned __int32  uint32_t;
#endif
#ifdef __APPLE__
#define __int64 long long
#endif

typedef signed __int64       int64_t;
typedef unsigned __int64     uint64_t;

typedef uint32_t 	Uint32;

#include <math.h>

#if defined(_WIN32) || defined(_WIN64)
#define fmax std::max
#define fmin std::min
#pragma warning (disable:4996)
#define snprintf sprintf_s
#endif

#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtc/matrix_transform.hpp>  

#endif /* STDAFX_H_ */

