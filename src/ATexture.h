/*
 * ATexture.h
 *
 *  Created on: May 16, 2012
 *      Author: numb3r23
 */

#ifndef ATEXTURE_H_
#define ATEXTURE_H_

#include "stdafx.h"

namespace SciIllLib {
    
    class ATexture{
    public:
        ATexture();
        ATexture(GLuint id);
        ATexture(GLuint id, int width, int height);
        virtual ~ATexture();

		inline operator GLuint(){return m_tex;}
        GLuint operator~ ();
        
        GLuint GetID();
        
		int GetWidth();
		int GetHeight();
    protected:
		int m_width;
        int m_height;

        GLuint m_tex;
    };
}
#endif /* ATEXTURE_H_ */