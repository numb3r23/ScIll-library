#ifndef AFILTER_H
#define AFILTER_H

#include "stdafx.h"
#include <iostream>

#include "CGLShaderProgram.hpp"

namespace SciIllLib {
    
  /**
   * An image Filter consist of a generic vertex-shader and specialized fragment shader. 
   * This base class handles binding, releasing of the shader and  setting the image that should be filtered.
   */
  class AFilter
  {
  public:
    AFilter();
    virtual ~AFilter();
    
    void bind() const;
    void release() const;
    
    /**
     * Bind a Sampler to the samplername "image"
     * Note: the shader has to be bound before!
     * @param unit the textureunit that should be used
     * @param id the TextureID
     */
    void BindBaseSampler(const GLint unit, const GLuint id) const;
    
    /**
     * Bind a Sampler to the samplername "image"
     * Note: the shader has to be bound before!
     * @deprecated use the CFilter::BindBaseSampler method!
     * @param unit the textureunit that should be used
     * @param id the TextureID
     */
    void BindSampler(GLint unit, GLuint id) const;

    /**
     * Bind a Sampler
     * Note: the shader has to be bound before!
     * @param name the samplername the texture should be bound to
     * @param unit the textureunit that should be used
     * @param id the TextureID
     */
    void BindSampler(const char* name, GLint unit, GLuint id) const;
    
    virtual CGLShaderProgram& shader();
      
  protected:
    CGLShaderProgram m_shader;

    bool postLoadAction();
  };
}
#endif // AFILTER_H
