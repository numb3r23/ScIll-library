/*
 * CRendererGBuff.cpp
 *
 *  Created on: Feb 16, 2011
 *      Author: schaefer
 */

#include "stdafx.h"

#include "CGBuffers.hpp"

/**
 * Create a new instance: create space, reserve memory, set values to defaults
 * @param filterEngine a pointer to a SFilterEngine instace since the GBuffer needs RenderTextures
 */
SciIllLib::CGBuffers::CGBuffers()
{
	// nothing to do here
	m_width = -1;
	m_height = -1;

  m_mrtTargets = new GLuint[RENDERTARGETS];
  m_rt = new GLuint[RENDERTARGETS];
	for (int idx = 0; idx < RENDERTARGETS; idx++)
	{
		m_mrtTargets[idx] = GL_COLOR_ATTACHMENT0 + idx;
		m_rt[idx] = 0;
	}
}

/**
 * Delete the textures
 */
SciIllLib::CGBuffers::~CGBuffers()
{
	for (int idx = 0; idx < RENDERTARGETS; idx++)
		SFilterEngine::DeleteTexture(&m_rt[idx]);
}

/**
 * Initialize the instance - mainly create Textures (by calling SetSize)
 * @param width viewport width
 * @param height viewport height
 */
void SciIllLib::CGBuffers::Initialize(const int width, const int height)
{
	SetSize(width, height);
}

/**
 * New viewportsize = recreate Textures
 * @param width viewport width
 * @param height viewport height
 */
void SciIllLib::CGBuffers::SetSize(const int width, const int height)
{
	if ((width == m_width) && (height == m_height))
  {
		return;
  }
  
	m_width = width;
	m_height = height;

	for (int idx = 0; idx < RENDERTARGETS; idx++)
  {
		SFilterEngine::ReGenerateTexture(&m_rt[idx], GL_FLOAT);
  }
}

/**
 * Activate GBuffer-Rendering: setup MRT
 */
void SciIllLib::CGBuffers::Activate() const
{
  SFilterEngine::PreRender(RENDERTARGETS, m_rt, m_mrtTargets);
}

/**
 * Deactivate GBuffer-Rendering: unset MRT
 */
void SciIllLib::CGBuffers::Deactivate() const
{
	SFilterEngine::PostRender();
}

/**
 * Return a pointer to the TextureID array
 */
const GLuint* SciIllLib::CGBuffers::GetRenderTargets() const
{
	return m_rt;
}

/**
 * Return the selected RenderTarget TextureID
 */
GLuint SciIllLib::CGBuffers::GetRenderTarget(const int idx) const
{
  return m_rt[idx];
}

