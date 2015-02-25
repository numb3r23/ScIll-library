#include "CFilter.hpp"

SciIllLib::CFilter::CFilter()
{
}

bool SciIllLib::CFilter::LoadFragmentShader(const std::string& filename)
{
	if (m_shader.addShadersFromSourceFile("res/glsl/filter/base.vert", filename))
  {
		return postLoadAction();
  }
	return false;
}

bool SciIllLib::CFilter::LoadShader(const std::string& vertexFileName, const std::string& fragmentFileName)
{
  if (m_shader.addShadersFromSourceFile(vertexFileName, fragmentFileName))
  {
    return postLoadAction();
  }
  return false;
}

