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
        static bool Initialize(int width, int height);
        
        static void ApplySequence();
        static void Apply(GLuint source, GLuint target);
		static void Apply(AFilter* shader, GLuint source, ATexture* target, glm::vec4 = glm::vec4(1,1,1,0));
        static void ApplyPure(GLuint source, GLuint target);

        
        static void SetSize(int width, int height);
        static void SetSizeOld();
        
        static void ReCreateDepthBuffer(GLuint* rboID, int width, int height);
        
        static void ReGenerateTexture(GLuint* oldTex, GLenum type);
        static void ReGenerateTexture(GLuint* oldTex, GLenum format, GLenum type);
        
        static void ReGenerateTextureSize(GLuint* oldTex, GLenum type, int width, int height);
        static void ReGenerateTextureSize(GLuint* oldTex, GLenum format, GLenum type, int width, int height);

		static void DeleteTexture(GLuint* oldTex);
        
        static void ToScreen(AFilter* source, GLuint texture);
        static void ToScreen(AFilter* shader);

        static void Filter(GLuint target);
        static void Filter(GLuint target, GLfloat* colorBackground);
        
        static void DrawQuad();
        
        static void PreRender(int countBuffers, GLuint* targets, GLenum* buffers);
        static void PreRender(ATexture* target);
        static void PreRender(GLuint target);
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
