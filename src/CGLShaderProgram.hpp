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
    
    static bool loadShader(const std::string& vertexFile, const std::string& fragmentFile, CGLShaderProgram* shader);

    bool addShaderFromSourceFile(GLenum type, const char* filename);
    bool addShaderFromSourceFile(GLenum type, const std::string& filename);
    
    bool addShadersFromSourceFile(const char* filenameVertex, const char* filenameFragment);
    bool addShadersFromSourceFile(const std::string& filenameVertex, const std::string& filenameFragment);
    
    bool link();
    
    bool reload();
    
    void bind() const;
    static void release();
    
    void setAttributeBuffer(int location, GLenum type, int offset, int tupleSize, int stride = 0) const;
    void setAttributeBuffer(const char *name, GLenum type, int offset, int tupleSize, int stride = 0) const;
    
    void enableAttributeArray(int location) const;
    void enableAttributeArray(const char *name) const;
    
    void setTransformFeedbackVaryings(const char** varyings, int count) const;
    
    void bindAttribLocation(GLuint index, const char* name) const;
    
    void setUniformValue(const std::string& name, const glm::vec2& value) const;
    void setUniformValue(const std::string& name, const glm::vec3& value) const;
    void setUniformValue(const std::string& name, const glm::vec4& value) const;
    void setUniformValue(const std::string& name, GLfloat value) const;
    void setUniformValue(const std::string& name, GLfloat x, GLfloat y) const;
    void setUniformValue(const std::string& name, GLfloat x, GLfloat y, GLfloat z) const;
    void setUniformValue(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;
    void setUniformValue(const std::string& name, GLint value) const;
    void setUniformValue(const std::string& name, GLint x, GLint y) const;
    void setUniformValue(const std::string& name, GLint x, GLint y, GLint z) const;
    void setUniformValue(const std::string& name, GLint x, GLint y, GLint z, GLint w) const;
    void setUniformValue(const std::string& name, const glm::mat4& value) const;
    
    void setUniformValue(GLint position, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;

    void BindSampler2D(const std::string& name, GLint unit, GLuint id) const;
    void BindSamplerCube(const std::string& name, GLint unit, GLuint id) const;
    
    GLint attribLocation(const char* name) const;
    GLuint getHandleProgram() const;
    
    GLint uniformLocationMap(const std::string& name) const;
    
    
    void showInformation() const;
    void showAttribInformation() const;
    void showUniformInformation() const;
    
  protected:
    GLuint m_hndVertex;
    GLuint m_hndFragment;
    GLuint m_hndGeometry;
    
    GLuint m_hndProgram;

    std::string m_qstrVertex;
    std::string m_qstrFragment;
    std::string m_qstrGeometry;
    
    std::map<std::string, GLint> m_map;
    
    void printTypeInfo(GLenum type)  const;
    void fillLocationMap();

    GLint uniformLocation(const char* name) const;
  };
}
#endif /* CGLShaderProgram_H_ */
