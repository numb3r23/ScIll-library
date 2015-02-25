/*
 * ATexture.cpp
 *
 *  Created on: May 16, 2012
 *      Author: numb3r23
 */

#include "stdafx.h"

#include "ATexture.hpp"

/**
 * Construct a new instance, initialize width and height ...
 */
SciIllLib::ATexture::ATexture()
{
  m_width = 1;
  m_height = 1;
  m_tex = 0;
}

SciIllLib::ATexture::ATexture(const GLuint id)
{
  m_width = 1;
  m_height = 1;
	m_tex = id;
}

SciIllLib::ATexture::ATexture(const GLuint id, const int width, const int height)
{
	m_tex = id;
	m_width = width;
  m_height = height;
}

SciIllLib::ATexture::~ATexture()
{
	//TODO freigeben...
}

const GLuint SciIllLib::ATexture::GetID() const
{
	return m_tex;
}

const int SciIllLib::ATexture::GetWidth() const
{
	return m_width;
}

const int SciIllLib::ATexture::GetHeight() const
{
	return m_height;
}

GLuint SciIllLib::ATexture::operator~(){
    return m_tex;
}
