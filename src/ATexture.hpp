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
        ATexture(const GLuint id);
        ATexture(const GLuint id, const int width, const int height);
        virtual ~ATexture();

        inline operator GLuint(){return m_tex;}
        GLuint operator~ ();
        
        const GLuint GetID() const;
        
        const int GetWidth() const;
        const int GetHeight() const;
    protected:
        int m_width;
        int m_height;

        GLuint m_tex;
    };
}
#endif /* ATEXTURE_H_ */