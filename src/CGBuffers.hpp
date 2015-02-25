/*
 * CGBuffers.h
 *
 *  Created on: Feb 16, 2011
 *      Author: schaefer
 */

#ifndef CGBUFFERS_H_
#define CGBUFFERS_H_

#include "stdafx.h"

#include "CGLShaderProgram.hpp"
#include "SFilterEngine.hpp"

namespace SciIllLib
{
  /**
   * The GBufferRenderer renders a scene into 3 G-Buffers (texcoord-, normal- and depth-map) using 
   * MultipleRenderTargets in a single pass. The class holds and manages the RenderTextures, the shader is not provided.
   */
  class CGBuffers{
  public:
    const int RENDERTARGETS = 3;
    CGBuffers();
    virtual ~CGBuffers();
    
    void Initialize(const int width, const int height);
    
    void SetSize(const int width, const int height);
    
    void Activate() const;
    void Deactivate() const;
    
    const GLuint* GetRenderTargets() const;
    GLuint GetRenderTarget(const int i) const;
  private:
    
      int m_width, m_height;

      GLuint* m_rt;
      GLenum* m_mrtTargets;
  };
}
#endif /* CGBUFFERS_H_ */
