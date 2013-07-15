#ifndef CGLINFO_H
#define CGLINFO_H

#include "stdafx.h"
#include <iostream>

namespace SciIllLib {
    
    /**
     * The GLInfo class consists of static functions that retrieve various information of OpenGL and prints it to the std::cout.
     */
    class CGLInfo
    {
    public:
        static void Version();
        static bool CheckError();
        static bool CheckError(const char* msg);
        static bool CheckErrorFBO();
    };
    
}
#endif // CGLINFO_H
