#include "AFilter.h"

#include "CGLInfo.h"


SciIllLib::AFilter::AFilter()
{
	m_shader = new CGLShaderProgram();
}

SciIllLib::AFilter::~AFilter(){
    delete m_shader;
}

/**
 * Bind the shader program
 */
void SciIllLib::AFilter::bind()
{
	m_shader->bind();
}

/**
 * Release the shader program
 */
void SciIllLib::AFilter::release()
{
	m_shader->release();
}

/**
 * Bind a Sampler to the samplername "image"
 * Note: the shader has to be bound before!
 * @param unit the textureunit that should be used
 * @param id the TextureID
 */
void SciIllLib::AFilter::BindBaseSampler(GLint unit, GLuint id)
{
	BindSampler("image", unit, id);
}

/**
 * Bind a Sampler to the samplername "image"
 * Note: the shader has to be bound before!
 * @deprecated use the CFilter::BindBaseSampler method!
 * @param unit the textureunit that should be used
 * @param id the TextureID
 */
void SciIllLib::AFilter::BindSampler(GLint unit, GLuint id) //--> Base!
{
	BindSampler("image", unit, id);
}

/**
 * Bind a Sampler 
 * Note: the shader has to be bound before!
 * @param name the samplername the texture should be bound to
 * @param unit the textureunit that should be used
 * @param id the TextureID
 */
void SciIllLib::AFilter::BindSampler(const char* name, GLint unit, GLuint id)
{
	m_shader->setUniformValue(name, unit);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, id);
}

/**
 * get the CGLShaderProgram instance that is used (for setting further parameters)
 * @return the instance of CGLShaderProgram
 */
SciIllLib::CGLShaderProgram* SciIllLib::AFilter::shader()
{
    return m_shader;
}


void SciIllLib::AFilter::setShader(SciIllLib::CGLShaderProgram* shader){
    m_shader = shader;
}

/**
 * Check for errors and set attrib Locations, the link the program and recheck for errors
 */
bool SciIllLib::AFilter::postLoadAction()
{
	bool trouble = false;
	trouble |= CGLInfo::CheckError("PostLoadAction");
	m_shader->bindAttribLocation(0, "vertex");
	m_shader->bindAttribLocation(1, "texCoord");
	trouble |= CGLInfo::CheckError("Attribs bound");
	m_shader->link();
	trouble |= CGLInfo::CheckError("program linked");
	return !trouble;
}

