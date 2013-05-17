#include "CFilter.h"

#include "CGLInfo.h"


SciIllLib::CFilter::CFilter()
{
	m_shader = new CGLShaderProgram();
}

/**
 * Load a Fragment shader and use "res/glsl/filter/base.vert" as vertex shader
 * @param filename full (absolute or relative) path to fragment shader
 * @returns success?
 */
bool SciIllLib::CFilter::LoadFragmentShader(std::string filename)
{
	if (m_shader->addShadersFromSourceFile("res/glsl/filter/base.vert", filename))
		return postLoadAction();
	return false;
}

