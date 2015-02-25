/*
 * STextureProcessor
 *
 *  Created on: Mar 2, 2011
 *      Author: numb3r23
 */

#ifndef STEXTURELOADER_H_
#define STEXTURELOADER_H_

#include "stdafx.h"

namespace RenderLib {
    
    class STextureLoader{
    public:
        static GLuint FTXTLoader(std::string);
        static GLuint CreateTexture(GLint, void*, int, int);
    protected:
        STextureLoader();
        virtual ~STextureLoader();

        static float* LoadFTXTData(std::string, int*, int*);
    };
    
}
#endif /* CTEXTURELOADER_H_ */
