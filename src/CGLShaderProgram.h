/*
 * GLShaderProgram.h
 *
 *  Created on: Oct 25, 2011
 *      Author: numb3r23
 */

#ifndef CGLShaderProgram_H_
#define CGLShaderProgram_H_

#include "stdafx.h"
#include <map>

namespace SciIllLib {
    
    /**
     * A ShaderProgram consists of a Vertex- and a FragmentShader. 
     * The class handles loading and setting of various parameters.
     * To set all possible paramteres the opengl functions must be used - this is not a full wrapper!
     */
    class CGLShaderProgram
    {
    public:
        CGLShaderProgram();
        ~CGLShaderProgram();
        
        void removeAllShaders();
        bool addShaderFromSourceFile(GLenum type, const char* filename);
        bool addShaderFromSourceFile(GLenum type, std::string filename);
        
        bool addShadersFromSourceFile(const char* filenameVertex, const char* filenameFragment);
        bool addShadersFromSourceFile(std::string filenameVertex, std::string filenameFragment);
        
        bool link();
        
        void bind();
        static void release();
        
        void setAttributeBuffer(int location, GLenum type, int offset, int tupleSize, int stride = 0);
        void setAttributeBuffer(const char *name, GLenum type, int offset, int tupleSize, int stride = 0);
        
        void enableAttributeArray(int location);
        void enableAttributeArray(const char *name);
        
        void bindAttribLocation(GLuint index, const char* name);
        
		void setUniformValue(std::string name, const glm::vec2 value);
		void setUniformValue(std::string name, const glm::vec3 value);
		void setUniformValue(std::string name, const glm::vec4 value);

		void setUniformValue(std::string name, GLfloat value);
        void setUniformValue(std::string name, GLfloat x, GLfloat y);
        void setUniformValue(std::string name, GLfloat x, GLfloat y, GLfloat z);
        void setUniformValue(std::string name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
        void setUniformValue(std::string name, GLint value);
        void setUniformValue(std::string name, GLint x, GLint y);
        void setUniformValue(std::string name, GLint x, GLint y, GLint z);
        void setUniformValue(std::string name, GLint x, GLint y, GLint z, GLint w);
        
        void setUniformValue(std::string name, const glm::mat4 value);
        
        void setUniformValue(GLint position, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
        
        void setTransformFeedbackVaryings(const char** varyings, int count);
        
        GLint attribLocation(const char* name);
        GLuint getHandleProgram();
        
        GLint uniformLocationMap(std::string name);
        
        void BindSampler2D(std::string name, GLint unit, GLuint id);
        void BindSamplerCube(std::string name, GLint unit, GLuint id);
        //void BindSampler(const char* name, GLint unit, ATexture tex);
        //void BindSampler(const char* name, GLint unit, ATexture *tex);
        
        void showInformation();
        void showAttribInformation();
        void showUniformInformation();
        
    protected:
        GLuint m_hndVertex;
        GLuint m_hndFragment;
        GLuint m_hndGeometry;
        
        GLuint m_hndProgram;

		std::string m_qstrVertex;
		std::string m_qstrFragment;
		std::string m_qstrGeometry;
        
        std::map<std::string, GLint> m_map;
        
        void printTypeInfo(GLenum type);
        void fillLocationMap();

        GLint uniformLocation(const char* name);
        
    };
    
}
#endif /* CGLShaderProgram_H_ */
