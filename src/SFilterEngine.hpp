/*
 * SFilterEngine.h
 *
 *  Created on: Feb 13, 2011
 *      Author: numb3r23
 */

#ifndef SFilterEngine_H_
#define SFilterEngine_H_

#include "stdafx.h"


#include "CGLShaderProgram.hpp"
#include "AFilter.hpp"
#include "ATexture.hpp"

namespace SciIllLib
{
  /**
   * The FilterEngine filters images with a CFilter to the screen or into a RenderTarget.
   * It also (re)-generates RenderTextures with the currently set size.
   */
  class SFilterEngine
  {
  public:
    static glm::vec4 kWhiteAlphaZero;
    
    /**
     * Initialize VBO/VAO for the creenaligned quad and generate Frame- and Renderbuffer
     * @param width the current Viewport width
     * @param height the current Viewport height
     * @returns success?
     */
    static bool Initialize(const int width, const int height);
    
    /**
     * Apply a pure filter
     * @deprecated Use another Apply function!
     * @param source the source TextureID
     * @param target the target TextureID
     */
    static void Apply(const GLuint source, const GLuint target);
    static void Apply(AFilter& shader, const GLuint source, ATexture& target, const glm::vec4& = SFilterEngine::kWhiteAlphaZero);
    /**
     * Apply a pure filter
     * @deprecated Use another Apply function!
     * @param source the source TextureID
     * @param target the target TextureID
     */
    static void ApplyPure(const GLuint source, const GLuint target);
    
    
    /**
     * Resize the Viewport means resize all reandertargets
     * @param width the viewport width
     * @param height the viewport height
     */
    static void SetSize(const int width, const int height);
    static void SetSizeOld();
    
    static void ReCreateDepthBuffer(GLuint* rboID, const int width, const int height);
    
    /**
     * (Re)Generate a Texture (possible use: RenderTarget) with the specified type, if oldTex is !+= 0 the texture is deleted first
     * @param oldTex a pointer to the old & new TextureID
     * @param type Specifies the data type of the pixel data. If set to GL_FLOAT, use GL_RGBA32F for internal format, otherwise GL_RGBA8
     */
    static void ReGenerateTexture(GLuint* oldTex, const GLenum type);
    /**
     * Generate a Texture (possible use: RenderTarget) with the specified type, if oldTex is !+= 0 the texture is deleted first
     * @param oldTex a pointer to the old & new TextureID
     * @param format Specifies the number of color components in the texture.
     * @param type Specifies the data type of the pixel data.
     */
    static void ReGenerateTexture(GLuint* oldTex, const GLenum format, const GLenum type);
    
    /**
     * (Re)Generate a Texture (possible use: RenderTarget) with the specified type, if oldTex is !+= 0 the texture is deleted first.
     * Use the specified size!
     * @param oldTex a pointer to the old & new TextureID
     * @param type Specifies the data type of the pixel data. If set to GL_FLOAT, use GL_RGBA32F for internal format, otherwise GL_RGBA8
     * @param width width of the texture
     * @param height height of the texture
     */
    static void ReGenerateTextureSize(GLuint* oldTex, const GLenum type, const int width, const int height);
    /**
     * Generate a Texture (possible use: RenderTarget) with the specified type, if oldTex is !+= 0 the texture is deleted first
     * Use the specified size!
     * @param oldTex a pointer to the old & new TextureID
     * @param format Specifies the number of color components in the texture.
     * @param type Specifies the data type of the pixel data.
     * @param width width of the texture
     * @param height height of the texture
     */
    static void ReGenerateTextureSize(GLuint* oldTex, const GLenum format, const GLenum type, const int width, const int height);
    
    /**
     * Delete the texture if the ID is > 0
     * @param oldTex a pointer to the old TextureID
     */
    static void DeleteTexture(GLuint* oldTex);
    
    /**
     * Bind the shaders from the Filter and filter the source texture to screen
     * @param shader the filter that should be used
     * @param texture the TextureID of the base image that is filtered
     */
    static void ToScreen(const AFilter& source, const GLuint texture);
    /**
     * Bind the shaders from the Filter and filter to screen.
     * The Filter should have the textures already assigned!
     * @param shader the filter that should be used
     */
    static void ToScreen(const AFilter& shader);
    
    /**
     * Set the Rendertarget, clear with color (0, 0, 0, 0) and render a screen-aligned-quad
     * @param target the TextureID of the target image
     */
    static void Filter(const GLuint target);
    /**
     * Set the Rendertarget, clear with the assigned color and render a screen-aligned-quad
     * @param target the TextureID of the target image
     * @param color use this color for clearing before rendering
     */
    static void Filter(const GLuint target, GLfloat* colorBackground);
    
    /**
     * Draw a screen-aligned-quad (using a trinagle fan if anyone cares)
     */
    static void DrawQuad();
    
    /**
     * Prepare Multiple Rendering to Texture and set all Framebuffers and activate the Buffers
     * @param count number of framebuffers/RenderTargets
     * @param targets pointer to a textureID array
     * @param buffers pointer to a bufferID array
     */
    static void PreRender(const int countBuffers, GLuint* targets, GLenum* buffers);
    /**
     * Prepare single Rendering to Texture and set all Framebuffers and activate the Buffers
     * @param target the RenderTextures textureID
     */
    static void PreRender(const ATexture& target);
    static void PreRender(const GLuint target);
    /**
     * Unset the Framebuffer
     */
    static void PostRender();
    
    static int GetWidth();
    static int GetHeight();
    
    static ATexture& DummyTexture();
    
  private:
    SFilterEngine();
    ~SFilterEngine();
    
    static int m_width, m_height;
    static int m_widthOld, m_heightOld;
    
    static GLuint m_vao;
    static GLuint m_vab[3];
    
    static GLuint m_fbo, m_depthTex;
    static ATexture m_texDummy;
    
    static GLuint m_rbo;
  };
}
#endif /* SFilterEngine_H_ */
