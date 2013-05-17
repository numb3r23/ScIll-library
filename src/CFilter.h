#ifndef CFILTER_H
#define CFILTER_H

#include "stdafx.h"
#include <iostream>

#include "CGLShaderProgram.h"
#include "AFilter.h"

namespace SciIllLib {
    
    /**
     * An image Filter consist of a generic vertex-shader and specialized fragment shader. 
     * This base class handles binding, releasing of the shader and  setting the image that should be filtered.
     */
    class CFilter : public AFilter
    {
    public:
        CFilter();
        
        bool LoadFragmentShader(std::string);
        
    };
    
}
#endif // CFILTER_H
