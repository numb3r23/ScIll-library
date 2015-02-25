#include "AFilter.hpp"

#include "CGLInfo.hpp"


SciIllLib::AFilter::AFilter()
: m_shader()
{
}

SciIllLib::AFilter::~AFilter()
{
}

void SciIllLib::AFilter::bind() const
{
	m_shader.bind();
}

void SciIllLib::AFilter::release() const
{
	m_shader.release();
}

void SciIllLib::AFilter::BindBaseSampler(const GLint unit, const GLuint id) const
{
	BindSampler("image", unit, id);
}

void SciIllLib::AFilter::BindSampler(const GLint unit, const GLuint id) const
{
	BindSampler("image", unit, id);
}

void SciIllLib::AFilter::BindSampler(const char* name, const GLint unit, const GLuint id) const
{
	m_shader.setUniformValue(name, unit);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, id);
}

SciIllLib::CGLShaderProgram& SciIllLib::AFilter::shader()
{
  return m_shader;
}

bool SciIllLib::AFilter::postLoadAction()
{
	bool trouble = false;
	trouble |= CGLInfo::CheckError("PostLoadAction");
	m_shader.bindAttribLocation(0, "vertex");
	m_shader.bindAttribLocation(1, "texCoord");
	trouble |= CGLInfo::CheckError("Attribs bound");
	m_shader.link();
	trouble |= CGLInfo::CheckError("program linked");
	return !trouble;
}

