/*
 * SFilterEngine.h
 *
 *  Created on: Feb 13, 2011
 *      Author: numb3r23
 */

#ifndef SFilterEngine_H_
#define SFilterEngine_H_

#include "stdafx.h"


#include "CGLShaderProgram.h"
#include "AFilter.h"
#include "ATexture.h"

namespace SciIllLib {
    
    /**
     * The FilterEngine filters images with a CFilter to the screen or into a RenderTarget. 
     * It also (re)-generates RenderTextures with the currently set size.  
     */
    class SFilterEngine{
    public:
        static bool Initialize(int, int);
        
        static void ApplySequence();
        static void Apply(GLuint, GLuint);
        static void ApplyPure(GLuint, GLuint);
        
        static void SetSize(int, int);
        static void SetSizeOld();
        
        static void ReCreateDepthBuffer(GLuint*, int, int);
        
        static void ReGenerateTexture(GLuint*, GLenum);
        static void ReGenerateTexture(GLuint*, GLenum, GLenum);
        
        static void ReGenerateTextureSize(GLuint*, GLenum, int, int);
        static void ReGenerateTextureSize(GLuint*, GLenum, GLenum, int, int);

		static void DeleteTexture(GLuint*);
        
        static void ToScreen(AFilter*, GLuint);
        static void ToScreen(AFilter*);

		static void Apply(AFilter*, GLuint, ATexture*, glm::vec4 = glm::vec4(1,1,1,0));

        static void Filter(GLuint);
        static void Filter(GLuint, GLfloat*);
        
        static void DrawQuad();
        
        static void PreRender(int, GLuint*, GLenum*);
        static void PreRender(ATexture*);
        static void PreRender(GLuint);
        static void PostRender();
        
        static int GetWidth();
        static int GetHeight();

        static ATexture* DummyTexture();
        
    private:
        SFilterEngine();
        ~SFilterEngine();
        
        static int m_width, m_height;
        static int m_widthOld, m_heightOld;

        static GLuint m_vao;
        static GLuint m_vab[3];
        
        static GLuint m_fbo, m_depthTex;
        static ATexture* m_texDummy;
        
        static GLuint m_rbo;
    };
    
}
#endif /* SFilterEngine_H_ */
