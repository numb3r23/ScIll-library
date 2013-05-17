/*
 * CRendererGBuff.cpp
 *
 *  Created on: Feb 16, 2011
 *      Author: schaefer
 */

#include "stdafx.h"

#include "CGBuffers.h"

/**
 * Create a new instance: create space, reserve memory, set values to defaults
 * @param filterEngine a pointer to a SFilterEngine instace since the GBuffer needs RenderTextures
 */
SciIllLib::CGBuffers::CGBuffers() {
	// nothing to do here
	m_width = -1;
	m_height = -1;

	for (int idx = 0; idx < RENDER_TARGETS; idx++)
	{
		m_mrtTargets[idx] = GL_COLOR_ATTACHMENT0 + idx;
		m_rt[idx] = 0;
	}
}

/**
 * Delete the textures
 */
SciIllLib::CGBuffers::~CGBuffers() {
	for (int idx = 0; idx < RENDER_TARGETS; idx++)
		SFilterEngine::DeleteTexture(&m_rt[idx]);
}

/**
 * Initialize the instance - mainly create Textures (by calling SetSize)
 * @param width viewport width
 * @param height viewport height
 */
void SciIllLib::CGBuffers::Initialize(int width, int height) {
	SetSize(width, height);
}

/**
 * New viewportsize = recreate Textures
 * @param width viewport width
 * @param height viewport height
 */
void SciIllLib::CGBuffers::SetSize(int width, int height) {
	if ((width == m_width) && (height == m_height))
		return;

	m_width = width;
	m_height = height;

	for (int idx = 0; idx < RENDER_TARGETS; idx++)
		SFilterEngine::ReGenerateTexture(&m_rt[idx], GL_FLOAT);
}

/**
 * Activate GBuffer-Rendering: setup MRT
 */
void SciIllLib::CGBuffers::Activate() {
	SFilterEngine::PreRender(RENDER_TARGETS, m_rt, m_mrtTargets);
}

/**
 * Deactivate GBuffer-Rendering: unset MRT
 */
void SciIllLib::CGBuffers::Deactivate() {
	SFilterEngine::PostRender();
}

/**
 * Return a pointer to the TextureID array
 */
GLuint* SciIllLib::CGBuffers::GetRenderTargets() {
	return m_rt;
}

/**
 * Return the selected RenderTarget TextureID
 */
GLuint SciIllLib::CGBuffers::GetRenderTarget(int idx) {
	return m_rt[idx];
}

