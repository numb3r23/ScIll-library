#ifndef AFILTER_H
#define AFILTER_H

#include "stdafx.h"
#include <iostream>

#include "CGLShaderProgram.h"

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
        
        void bind();
        void release();
        
        void BindBaseSampler(GLint unit, GLuint id);
        void BindSampler(GLint unit, GLuint id); //--> Base!
        void BindSampler(const char* name, GLint unit, GLuint id);
        
        virtual CGLShaderProgram* shader();
        virtual void setShader(CGLShaderProgram* shader);
        
    protected:
        CGLShaderProgram* m_shader;

        bool postLoadAction();
    };
    
}
#endif // AFILTER_H
