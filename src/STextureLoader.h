/*
 * STextureProcessor
 *
 *  Created on: Mar 2, 2011
 *      Author: numb3r23
 */

#ifndef STEXTURELOADER_H_
#define STEXTURELOADER_H_

#include "stdafx.h"

namespace SciIllLib {
    
    class STextureLoader{
    public:
        static GLuint FTXTLoader(std::string fname);
        static GLuint CreateTexture(GLint type, void* data, int width, int height);
    protected:
        STextureLoader();
        virtual ~STextureLoader();

        static float* LoadFTXTData(std::string fname, int* width, int *height);
    };
    
}
#endif /* CTEXTURELOADER_H_ */
