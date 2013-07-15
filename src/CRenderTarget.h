/*
 * CRenderTarget.h
 *
 *  Created on: May 16, 2012
 *      Author: numb3r23
 */

#ifndef CRENDERTARGET_H_
#define CRENDERTARGET_H_

#include "stdafx.h"

#include "ATexture.h"


namespace SciIllLib {
    
    class CRenderTarget : public ATexture{
    public:
        CRenderTarget(GLuint tex, int width, int height);
        CRenderTarget(GLuint tex, int width, int height, bool isScreenSize);

		~CRenderTarget();
        
        bool IsScreenSize();
        
    protected:
        bool m_bScreenSize;
    };
    
}
#endif /* CRENDERTARGET_H_ */