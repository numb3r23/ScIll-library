/*
 * CGBuffers.h
 *
 *  Created on: Feb 16, 2011
 *      Author: schaefer
 */

#ifndef CGBUFFERS_H_
#define CGBUFFERS_H_

//#include "myOpenGL.h"
#include "stdafx.h"
#include "CGLShaderProgram.h"
#include "SFilterEngine.h"

#define RENDER_TARGETS 3

namespace SciIllLib {
    
    /**
     * The GBufferRenderer renders a scene into 3 G-Buffers (texcoord-, normal- and depth-map) using 
     * MultipleRenderTargets in a single pass. The class holds and manages the RenderTextures, the shader is not provided.
     */
    class CGBuffers{
    public:
        CGBuffers();
        virtual ~CGBuffers();
        
        void Initialize(int width, int height);
        
        void SetSize(int width, int height);
        
        void Activate();
        void Deactivate();
        
        GLuint* GetRenderTargets();
        GLuint GetRenderTarget(int i);
        
        
    private:
        int m_width, m_height;

        GLuint m_rt[RENDER_TARGETS];
        GLenum m_mrtTargets[RENDER_TARGETS];
    };
    
}
#endif /* CGBUFFERS_H_ */
