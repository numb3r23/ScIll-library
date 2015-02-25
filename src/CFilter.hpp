#ifndef CFILTER_H
#define CFILTER_H

#include "stdafx.h"
#include <iostream>

#include "CGLShaderProgram.hpp"
#include "AFilter.hpp"

namespace SciIllLib
{
  /**
   * An image Filter consist of a generic vertex-shader and specialized fragment shader. 
   * This base class handles binding, releasing of the shader and  setting the image that should be filtered.
   */
  class CFilter : public AFilter
  {
  public:
    CFilter();
      
    /**
     * Load a Fragment shader and use "res/glsl/filter/base.vert" as vertex shader
     * @param filename full (absolute or relative) path to fragment shader
     * @returns success?
     */
    bool LoadFragmentShader(const std::string& filename);
    
    /**
     * Load a Fragment shader and use "res/glsl/filter/base.vert" as vertex shader
     * @param filename full (absolute or relative) path to fragment shader
     * @returns success?
     */
    bool LoadShader(const std::string& vertexFileName, const std::string& fragmentFileName);
  };
}
#endif // CFILTER_H
