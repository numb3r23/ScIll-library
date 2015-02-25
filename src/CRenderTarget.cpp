/*
 * CRenderTarget.cpp
 *
 *  Created on: May 20, 2012
 *      Author: numb3r23
 */

#include "stdafx.h"

#include "CRenderTarget.hpp"

SciIllLib::CRenderTarget::CRenderTarget(GLuint tex, int width, int height)
: ATexture(tex, width, height)
{
  m_bScreenSize = true;
}

SciIllLib::CRenderTarget::CRenderTarget(GLuint tex, int width, int height, bool screenSize)
: ATexture(tex, width, height)
{
  m_bScreenSize = screenSize;
}

SciIllLib::CRenderTarget::~CRenderTarget()
{
	
}

bool SciIllLib::CRenderTarget::IsScreenSize() const
{
  return m_bScreenSize;
}

