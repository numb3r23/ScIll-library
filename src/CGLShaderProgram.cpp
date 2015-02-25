/*
 * GLShaderProgram.cpp
 *
 *  Created on: Oct 25, 2011
 *      Author: numb3r23
 */

#include "CGLShaderProgram.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "CGLInfo.hpp"

SciIllLib::CGLShaderProgram::CGLShaderProgram()
: m_hndVertex(0),
  m_hndFragment(0),
  m_hndGeometry(0),
  m_hndProgram(0),
  m_qstrVertex(""),
  m_qstrFragment(""),
  m_qstrGeometry(""),
  m_map()
{
  // do nothing
}

SciIllLib::CGLShaderProgram::~CGLShaderProgram()
{
	removeAllShaders();
}

/**
 * Remove all shader - currently not working
 */
void SciIllLib::CGLShaderProgram::removeAllShaders()
{	
	if (m_hndProgram > 0)
	{
		glDeleteProgram(m_hndProgram);
	}
  
	glDeleteShader(m_hndGeometry);
	glDeleteShader(m_hndFragment);
	glDeleteShader(m_hndVertex);
}

bool SciIllLib::CGLShaderProgram::loadShader(const std::string& vertexFile, const std::string& fragmentFile, CGLShaderProgram* shader)
{
  std::cout << "Loading shaders (" << vertexFile << ".vert & " << fragmentFile << ".frag)" << std::endl;
  
  if (!shader->addShadersFromSourceFile("res/glsl/" + vertexFile + ".vert",
                                        "res/glsl/" + fragmentFile + ".frag"))
  {
    std::cout << "! reload failed due to compilation" << std::endl;
    return false;
  }
  
  if (!shader->link())
  {
    std::cout << "! reload failed due to linkage" << std::endl;
    return false;
  }
  
  SciIllLib::CGLInfo::CheckError("shader loaded");
  
  std::cout << " ...done as program-id:" << shader->getHandleProgram() << std::endl;
  return true;
}

/**
 * Set a Vertex-/FragmentShader pair from file
 * @param filenameVertex path to the vertexshader
 * @param filenameFragment path to the fragmentshader
 */
bool SciIllLib::CGLShaderProgram::addShadersFromSourceFile(const char* filenameVertex, const char* filenameFragment)
{
	return 
		addShaderFromSourceFile(GL_VERTEX_SHADER, filenameVertex)
		&&
		addShaderFromSourceFile(GL_FRAGMENT_SHADER, filenameFragment);
}

/**
 * Set a Vertex-/FragmentShader pair from file
 * @param filenameVertex path to the vertexshader
 * @param filenameFragment path to the fragmentshader
 */
bool SciIllLib::CGLShaderProgram::addShadersFromSourceFile(const std::string& filenameVertex, const std::string& filenameFragment)
{
	return
    addShaderFromSourceFile(GL_VERTEX_SHADER, filenameVertex.c_str())
    &&
    addShaderFromSourceFile(GL_FRAGMENT_SHADER, filenameFragment.c_str());
    
}

/**
 * Add a Vertex- / Fragment or a GeometryShader from file
 * @param type Type of shader - GL_VERTEX_SHADER, GL_FRAGMENT_SHADER or GL_GEOMETRY_SHADER
 * @param filename filename of the shader to be loaded
 */
bool SciIllLib::CGLShaderProgram::addShaderFromSourceFile(GLenum type, const std::string& filename)
{
	return addShaderFromSourceFile(type, filename.c_str());
}

/**
 * Add a Vertex- / Fragment or a GeometryShader from file
 * @param type Type of shader - GL_VERTEX_SHADER, GL_FRAGMENT_SHADER or GL_GEOMETRY_SHADER
 * @param filename filename of the shader to be loaded
 */
bool SciIllLib::CGLShaderProgram::addShaderFromSourceFile(GLenum type, const char* filename)
{
  if (m_hndProgram <= 0)
	{
		m_hndProgram = glCreateProgram();
	}

	std::fstream shaderFile(filename, std::ios::in);
  std::string sShader;
  const char * szShader;
  
  if (shaderFile.is_open())
  {
    std::stringstream buffer;
    buffer << shaderFile.rdbuf();
    sShader = buffer.str();
  } else {
    std::cout << "Failed to read file: " << filename << std::endl;
    return false;
  }
  shaderFile.close();

	//TODO cleanup shaders!
  GLuint hndShader = glCreateShader(type);
  
  szShader = sShader.c_str();
  glShaderSource(hndShader, 1, (const char**) &szShader, NULL);
  glCompileShader(hndShader);
  
  // check result
  int compiled = 0, length = 0, laux = 0;
  glGetShaderiv(hndShader, GL_COMPILE_STATUS, &compiled);
  if (compiled == GL_TRUE)
  {
    if (type == GL_VERTEX_SHADER) {
      if (m_hndVertex > 0)
      {
        glDetachShader(m_hndProgram, m_hndVertex);
        glDeleteShader(m_hndVertex);
      }
      m_qstrVertex = filename;
      m_hndVertex = hndShader;
      glAttachShader(m_hndProgram, m_hndVertex);
    }
    else
    {
      if (type == GL_FRAGMENT_SHADER)
      {
        if (m_hndFragment > 0)
        {
          glDetachShader(m_hndProgram, m_hndFragment);
          glDeleteShader(m_hndFragment);
        }
        m_qstrFragment = filename;
        m_hndFragment = hndShader;
        glAttachShader(m_hndProgram, m_hndFragment);
      }
      else
      {
        if (type == GL_GEOMETRY_SHADER)
        {
          if (m_hndGeometry > 0)
          {
            glDetachShader(m_hndProgram, m_hndGeometry);
            glDeleteShader(m_hndGeometry);
          }
          m_qstrGeometry = filename;
          m_hndGeometry = hndShader;
          glAttachShader(m_hndProgram, m_hndGeometry);
        }
      }
    }
    return true;
  }
  else
  {
    glGetShaderiv(hndShader, GL_INFO_LOG_LENGTH, &length);
    char *logString = new char[length];
    glGetShaderInfoLog(hndShader, length, &laux, logString);

    std::cout << "Couldn't compile shader - " << filename << std::endl;
    std::cout << logString << std::endl;

    return false;
  }
  return false;
}

/**
 * Link the shaders that are loaded - note: a valid Vertex- and FragmentShader must be loaded
 */
bool SciIllLib::CGLShaderProgram::link()
{
  // Link and check errors
  glLinkProgram(m_hndProgram);
  int result = 0, length = 0, laux = 0;
  glGetProgramiv(m_hndProgram, GL_LINK_STATUS, &result);
  if (result == GL_TRUE)
  {
    fillLocationMap();
    
    if (m_hndVertex > 0)
    {
      glDetachShader(m_hndProgram, m_hndVertex);
    }
    if (m_hndGeometry > 0)
    {
      glDetachShader(m_hndProgram, m_hndGeometry);
    }
    if (m_hndFragment > 0)
    {
      glDetachShader(m_hndProgram, m_hndFragment);
    }
    
    return true;
  }
  else
  {
    glGetProgramiv(m_hndProgram, GL_INFO_LOG_LENGTH, &length);
    char *infoLog = (char *)malloc(length * sizeof(char));
    glGetProgramInfoLog(m_hndProgram,length,&laux,infoLog);

    std::cout << "Couldn't link shader:" << std::endl;
    std::cout << infoLog << std::endl;
    
    delete infoLog;

    return false;
  }

  glDetachShader(m_hndProgram, m_hndVertex);
  glDetachShader(m_hndProgram, m_hndFragment);
  
  return true;
}

bool SciIllLib::CGLShaderProgram::reload()
{
  if ((m_qstrFragment.compare("") == 0) && (m_qstrVertex.compare("") == 0))
  {
    return false;
  }
  
  removeAllShaders();
  
  if (!addShadersFromSourceFile(m_qstrFragment, m_qstrVertex))
  {
    return false;
  }
  
  if (!link())
  {
    return false;
  }
  
  return true;
}

void SciIllLib::CGLShaderProgram::fillLocationMap()
{
  int count = 0;
  glGetProgramiv(m_hndProgram, GL_ACTIVE_UNIFORMS, &count);
  if (count == 0)
  {
    return;
  }
  
  GLsizei bufSize = 256;
  GLint size;
  GLsizei length;
  GLenum type;
  char* name = new char[bufSize];
  
  for (int idx=0; idx < count; idx++)
  {
    glGetActiveUniform(m_hndProgram, idx, bufSize, &length, &size, &type, name);
    m_map[name] = glGetUniformLocation(m_hndProgram, name);
  }
}

void SciIllLib::CGLShaderProgram::bind() const
{
  glUseProgram(m_hndProgram);
}

void SciIllLib::CGLShaderProgram::release()
{
  glUseProgram(0);
}

/**
 * Setup an attribute buffer by index. Wrapper for glVertexAttribPointer with rearranged parameters.
 * @param location index of the vertex attribute 
 * @param type Specifies the data type of each component in the array (usually GL_FLOAT)
 * @param offset Specifies a pointer to the first component of the first generic vertex attribute in the array. The initial value is 0.
 * @param tupleSize Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, or 4. The initial value is 4.
 * @param stride Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
 */
void SciIllLib::CGLShaderProgram::setAttributeBuffer(int location, GLenum type, int offset, int tupleSize, int stride) const
{
	  glVertexAttribPointer(location, tupleSize, type, GL_TRUE, stride,reinterpret_cast<const void *>(offset));
}

/**
 * Setup an attribute buffer by Name. 
 * @param name this name is used for the location index look-up
 * @param type Specifies the data type of each component in the array (usually GL_FLOAT)
 * @param offset Specifies a pointer to the first component of the first generic vertex attribute in the array. The initial value is 0.
 * @param tupleSize Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, or 4. The initial value is 4.
 * @param stride Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
 */
void SciIllLib::CGLShaderProgram::setAttributeBuffer(const char *name, GLenum type, int offset, int tupleSize, int stride) const
{
	setAttributeBuffer(attribLocation(name), type, offset, tupleSize, stride);
}

/**
 * Enable a vertex attribute by index. direct Wrapper for glEnableVertexAttribArray.
 * @param location index of the vertex attribute 
 */
void SciIllLib::CGLShaderProgram::enableAttributeArray(int location) const
{
	glEnableVertexAttribArray(location);
}

/**
 * Enable a vertex attribute by name. 
 * @param name this name is used for the location index look-up
 */
void SciIllLib::CGLShaderProgram::enableAttributeArray(const char *name) const
{
	enableAttributeArray(attribLocation(name));
}

/**
 * Bind an attribute location for this program. Wrapper to glBindAttribLocation.
 * @param index index of the vertex attribute 
 * @param name the null terminated string containing the name of the VertexShader attribute variable
 */
void SciIllLib::CGLShaderProgram::bindAttribLocation(GLuint index, const char* name) const
{
    glBindAttribLocation(m_hndProgram, index, name);
}

/**
 * Specify values to record in transform fewedback buffers. Wrapper to glTransformFeedbackVaryings with rearranged parameters and bufferMode set to GL_SEPERATE_ATTRIBS.
 * @param varyings An array of count zero-terminated strings specifying the names of the varying variables to use for transform feedback.
 * @param count The number of varying variables used for transform feedback.
 */
void SciIllLib::CGLShaderProgram::setTransformFeedbackVaryings(const char** varyings, int count) const
{    
    glTransformFeedbackVaryings(m_hndProgram, count, varyings, GL_SEPARATE_ATTRIBS);    
}

/**
 * Show information of attribs and uniforms for this program
 */
void SciIllLib::CGLShaderProgram::showInformation() const
{
	std::cout << "IDs " << m_hndProgram << std::endl;
	showAttribInformation();
  showUniformInformation();
}

/**
 * Show detailed information for all attribs of this program
 */
void SciIllLib::CGLShaderProgram::showAttribInformation() const
{
  std::cout << "Getting ShaderProgram attributes information:" << std::endl;
  int count = 0;
  glGetProgramiv(m_hndProgram, GL_ACTIVE_ATTRIBUTES, &count);
  if (count == 0)
  {
    std::cout << "--> No attributes defined!" << std::endl;
    return;
  }
  
  GLsizei bufSize = 256;
  GLint size;
  GLsizei length;
  GLenum type;
  char* name = new char[bufSize];
  
  for (int idx=0; idx < count; idx++)
  {
    glGetActiveAttrib(m_hndProgram, idx, bufSize, &length, &size, &type, name);
    std::cout << "- " << idx << ":" << name << ", ";
    printTypeInfo(type);
    std::cout << std::endl;
  }
}

/**
 * Show detailed information for all uniforms of this program
 */
void SciIllLib::CGLShaderProgram::showUniformInformation() const
{
  std::cout << "Getting ShaderProgram uniform information: " << std::endl;
  int count = 0;
  glGetProgramiv(m_hndProgram, GL_ACTIVE_UNIFORMS, &count);
  if (count == 0)
  {
    std::cout << "--> No uniforms defined!" << std::endl;
    return;
  }
  
  GLsizei bufSize = 256;
  GLint size;
  GLsizei length;
  GLenum type;
  char* name = new char[bufSize];
  
  for (int idx=0; idx < count; idx++)
  {
    glGetActiveUniform(m_hndProgram, idx, bufSize, &length, &size, &type, name);
    std::cout << "- " << idx << ":" << name << ", ";
    printTypeInfo(type);
    std::cout << std::endl;
  }
}

/**
 * Show type information in human readable form
 * @param type the openGL Type that's associated with an OpenGL-type-enum
 */
void SciIllLib::CGLShaderProgram::printTypeInfo(GLenum type) const
{
  switch (type)
  {
    case GL_FLOAT:				std::cout <<"GL_FLOAT"; break;
    case GL_FLOAT_VEC2:			std::cout <<"GL_FLOAT_VEC2"; break;
    case GL_FLOAT_VEC3:			std::cout <<"GL_FLOAT_VEC3"; break;
    case GL_FLOAT_VEC4:			std::cout <<"GL_FLOAT_VEC4"; break;
    case GL_INT:				std::cout <<"GL_INT"; break;
    case GL_INT_VEC2:			std::cout <<"GL_INT_VEC2"; break;
    case GL_INT_VEC3:			std::cout <<"GL_INT_VEC3"; break;
    case GL_INT_VEC4:			std::cout <<"GL_INT_VEC4"; break;
    case GL_BOOL:				std::cout <<"GL_BOOL"; break;
    case GL_BOOL_VEC2:			std::cout <<"GL_BOOL_VEC2"; break;
    case GL_BOOL_VEC3:			std::cout <<"GL_BOOL_VEC3"; break;
    case GL_BOOL_VEC4:			std::cout <<"GL_BOOL_VEC4"; break;
    case GL_FLOAT_MAT2:			std::cout <<"GL_FLOAT_MAT2"; break;
    case GL_FLOAT_MAT3:			std::cout <<"GL_FLOAT_MAT3"; break;
    case GL_FLOAT_MAT4:			std::cout <<"GL_FLOAT_MAT4"; break;
    case GL_FLOAT_MAT2x3:		std::cout <<"GL_FLOAT_MAT2x3"; break;
    case GL_FLOAT_MAT2x4:		std::cout <<"GL_FLOAT_MAT2x4"; break;
    case GL_FLOAT_MAT3x2:		std::cout <<"GL_FLOAT_MAT3x2"; break;
    case GL_FLOAT_MAT3x4:		std::cout <<"GL_FLOAT_MAT3x4"; break;
    case GL_FLOAT_MAT4x2:		std::cout <<"GL_FLOAT_MAT4x2"; break;
    case GL_FLOAT_MAT4x3:		std::cout <<"GL_FLOAT_MAT4x3"; break;
    case GL_SAMPLER_1D:			std::cout <<"GL_SAMPLER_1D"; break;
    case GL_SAMPLER_2D:			std::cout <<"GL_SAMPLER_2D"; break;
    case GL_SAMPLER_3D:			std::cout <<"GL_SAMPLER_3D"; break;
    case GL_SAMPLER_CUBE:		std::cout <<"GL_SAMPLER_CUBE"; break;
    case GL_SAMPLER_1D_SHADOW:	std::cout <<"GL_SAMPLER_1D_SHADOW"; break;
    case GL_SAMPLER_2D_SHADOW:	std::cout <<"GL_SAMPLER_2D_SHADOW"; break;
  default:
    std::cout <<"Unknown!"; break;
  }
}

/**
 * @returns the programs handle
 */
GLuint SciIllLib::CGLShaderProgram::getHandleProgram()  const
{
  return m_hndProgram;
}

/**
 * Bind a sampler. note: you must bind the program first! The texture unit is also activated.
 * @param name the name of the sampler in the program
 * @param unit the TextureUnit that should be used
 * @param id the TextureID that should be used
 */
void SciIllLib::CGLShaderProgram::BindSampler2D(const std::string& name, GLint unit, GLuint id) const
{
	setUniformValue(name, unit);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, id);
}

/**
 * Bind a sampler. note: you must bind the program first! The texture unit is also activated.
 * @param name the name of the sampler in the program
 * @param unit the TextureUnit that should be used
 * @param id the TextureID that should be used
 */
void SciIllLib::CGLShaderProgram::BindSamplerCube(const std::string& name, GLint unit, GLuint id) const
{
	setUniformValue(name, unit);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void SciIllLib::CGLShaderProgram::setUniformValue(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(uniformLocationMap(name), 1, glm::value_ptr(value));
}

void SciIllLib::CGLShaderProgram::setUniformValue(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(uniformLocationMap(name), 1, glm::value_ptr(value));
}

void SciIllLib::CGLShaderProgram::setUniformValue(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(uniformLocationMap(name), 1, glm::value_ptr(value));
}

/**
 * Set a uniform 1d value. Wrapper to glUniform1f
 * @param name this name is used for the location index look-up
 * @param value the (x) value that is set
 */
void SciIllLib::CGLShaderProgram::setUniformValue(const std::string& name, GLfloat value) const
{
  GLint location = uniformLocationMap(name);
	glUniform1f(location, value);
}

/**
 * Set a uniform 2d value. Wrapper to glUniform2f
 * @param name this name is used for the location index look-up
 * @param x the x value that is set
 * @param y the y value that is set
 */
void SciIllLib::CGLShaderProgram::setUniformValue(const std::string& name, GLfloat x, GLfloat y) const
{
	glUniform2f(uniformLocationMap(name), x, y);
}

/**
 * Set a uniform 3d value. Wrapper to glUniform3f
 * @param name this name is used for the location index look-up
 * @param x the x value that is set
 * @param y the y value that is set
 * @param z the z value that is set
 */
void SciIllLib::CGLShaderProgram::setUniformValue(const std::string& name, GLfloat x, GLfloat y, GLfloat z) const
{
	glUniform3f(uniformLocationMap(name), x, y, z);
}

/**
 * Set a uniform 4d value. Wrapper to glUniform4f
 * @param name this name is used for the location index look-up
 * @param x the x value that is set
 * @param y the y value that is set
 * @param z the z value that is set
 * @param w the w value that is set
 */
void SciIllLib::CGLShaderProgram::setUniformValue(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
{
	GLint loc = uniformLocationMap(name);
	glUniform4f(loc, x, y, z, w);
}

/**
 * Set a uniform 1d value. Wrapper to glUniform1i
 * @param name this name is used for the location index look-up
 * @param value the value that is set
 */
void SciIllLib::CGLShaderProgram::setUniformValue(const std::string& name, GLint value) const
{
	glUniform1i(uniformLocationMap(name), value);
}

/**
 * Set a uniform 2d value. Wrapper to glUniform2i
 * @param name this name is used for the location index look-up
 * @param value the value that is set
 */
void SciIllLib::CGLShaderProgram::setUniformValue(const std::string& name, GLint x, GLint y) const
{
	glUniform2i(uniformLocationMap(name), x, y);
}

/**
 * Set a uniform 3d value. Wrapper to glUniform3i
 * @param name this name is used for the location index look-up
 * @param value the value that is set
 */
void SciIllLib::CGLShaderProgram::setUniformValue(const std::string& name, GLint x, GLint y, GLint z) const
{
	glUniform3i(uniformLocationMap(name), x, y, y);
}

/**
 * Set a uniform 4d value. Wrapper to glUniform4i
 * @param name this name is used for the location index look-up
 * @param value the value that is set
 */
void SciIllLib::CGLShaderProgram::setUniformValue(const std::string& name, GLint x, GLint y, GLint z, GLint w) const
{
	glUniform4i(uniformLocationMap(name), x, y, z, w);
}

/**
 * Set a mat4 matrix. Wrapper to glUniformMatrix4fv with fixed values: count=1, transpose=GL_FALSE
 * @param name this name is used for the location index look-up
 * @param value the matrix that is set
 */
void SciIllLib::CGLShaderProgram::setUniformValue(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(uniformLocationMap(name), 1, GL_FALSE, glm::value_ptr(value));
}

/**
 * Set a uniform 4d value. Wrapper to glUniform4f
 * @param position the index of the uniform parameter
 * @param x the x value that is set
 * @param y the y value that is set
 * @param z the z value that is set
 * @param w the w value that is set
 */
void SciIllLib::CGLShaderProgram::setUniformValue(GLint position, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
{
	glUniform4f(position, x, y, z, w);
}

/**
 * Wrapper for glGetUniformLocation for this program
 * @param name the name that should be looked up
 * @return the location index
 */
GLint SciIllLib::CGLShaderProgram::uniformLocation(const char* name) const
{
#ifdef DEBUG
	GLint loc = glGetUniformLocation(m_hndProgram, name);
	if (loc < 0)
  {
		std::cout << "couldnt find uniform '" << name << "' in:"<<std::endl;
		std::cout << m_qstrVertex << ", " << m_qstrFragment << ", " << m_qstrGeometry <<std::endl;
		this->showUniformInformation();
		std::cout << std::endl;
	}
#endif
	return glGetUniformLocation(m_hndProgram, name);
}

GLint SciIllLib::CGLShaderProgram::uniformLocationMap(const std::string& name) const
{
  //return uniformLocation(name.c_str());
  auto search = m_map.find(name);
  if (search != m_map.end())
  {
    return search->second;
  }
  //std::cout << "Could not find: " << name << std::endl;
  return -1;
}

/**
 * Wrapper for glGetAttribLocation for this program
 * @param name the name that should be looked up
 * @return the location index
 */
GLint SciIllLib::CGLShaderProgram::attribLocation(const char* name) const
{
	return glGetAttribLocation(m_hndProgram, name);
}
