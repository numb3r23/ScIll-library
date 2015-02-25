/*
 * SFilterEngine.cpp
 *
 *  Created on: Feb 13, 2011
 *      Author: numb3r23
 */

#include "stdafx.h"

#include <cassert>

#include "SFilterEngine.hpp"
#include "CGLInfo.hpp"

glm::vec4 SciIllLib::SFilterEngine::kWhiteAlphaZero = glm::vec4(1,1,1,0);

int SciIllLib::SFilterEngine::m_width, SciIllLib::SFilterEngine::m_height;
int SciIllLib::SFilterEngine::m_widthOld, SciIllLib::SFilterEngine::m_heightOld;

GLuint SciIllLib::SFilterEngine::m_vao;
GLuint SciIllLib::SFilterEngine::m_vab[3];

GLuint SciIllLib::SFilterEngine::m_fbo, SciIllLib::SFilterEngine::m_depthTex;
SciIllLib::ATexture SciIllLib::SFilterEngine::m_texDummy;

GLuint SciIllLib::SFilterEngine::m_rbo;

/**
 * Construct a new instance, initialize width and height ...
 */
SciIllLib::SFilterEngine::SFilterEngine()
{
}

SciIllLib::SFilterEngine::~SFilterEngine()
{
	glDeleteBuffers(3, &m_vab[0]);
  glDeleteVertexArrays(1, &m_vao);

	glDeleteRenderbuffers(1, &m_depthTex);
	glDeleteFramebuffers(1, &m_fbo);
}

bool SciIllLib::SFilterEngine::Initialize(const int width, const int height)
{
  m_texDummy = ATexture(0);

	m_width = -1;
	m_height = -1;
	m_widthOld = width;
	m_heightOld = height;

	//Something to draw...
	float arrVertex[] = {
		-1.0f, -1.0f, 
		 1.0f, -1.0f, 
		 1.0f,  1.0f, 
		-1.0f,  1.0f
	};
	float arrTexCoord[] = 
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
    
	glGenBuffers(3, &m_vab[0]);

	int idx = 0;
	glBindBuffer(GL_ARRAY_BUFFER, m_vab[idx++]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLfloat), arrVertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vab[idx++]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLfloat), arrTexCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//somewhere to draw to
	glGenFramebuffers(1, &m_fbo);
	glGenRenderbuffers(1, &m_depthTex);

	SetSize(width, height);
	return true;
}

void SciIllLib::SFilterEngine::Apply(const GLuint source, const GLuint target)
{
    ApplyPure(source, target);
}

void SciIllLib::SFilterEngine::Apply(AFilter& shader, const GLuint source, ATexture& target, const glm::vec4& clear)
{
  assert(target.GetID() != source);
  
	PreRender(target);
  CGLInfo::CheckErrorFBO();
  CGLInfo::CheckError("PreFilter");
  
  glClearColor(clear.r, clear.g, clear.b, clear.a);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  ToScreen(shader, source);

  PostRender();
	
  CGLInfo::CheckError("PostFilter");
}

void SciIllLib::SFilterEngine::PreRender(const int count, GLuint* targets, GLenum* buffers)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	for (int i=0; i < count; i++)
  {
		glFramebufferTexture(GL_FRAMEBUFFER, buffers[i], targets[i], 0);
  }
  glDrawBuffers(count, buffers);
}

void SciIllLib::SFilterEngine::PreRender(const ATexture& target)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, target.GetID(), 0);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthTex);
	glViewport(0.0, 0.0, target.GetWidth(), target.GetHeight());
}

void SciIllLib::SFilterEngine::PreRender(const GLuint target)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, 0);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void SciIllLib::SFilterEngine::PostRender()
{
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 0, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int SciIllLib::SFilterEngine::GetWidth()
{
	return m_width;
}

int SciIllLib::SFilterEngine::GetHeight()
{
	return m_height;
}

void SciIllLib::SFilterEngine::ApplyPure(const GLuint source, const GLuint target)
{
  assert(source != target);
  
	PreRender(target);
	DrawQuad();
  PostRender();
}

void SciIllLib::SFilterEngine::ToScreen(const AFilter& shader, const GLuint texture)
{
	shader.bind();
	shader.BindSampler(0, texture);
	
  DrawQuad();
	
  shader.BindSampler(0, 0);
  shader.release();
}

void SciIllLib::SFilterEngine::ToScreen(const AFilter& shader)
{
	shader.bind();
	DrawQuad();
  shader.release();
}

void SciIllLib::SFilterEngine::Filter(const GLuint target)
{
	//used by APostProcessor
	//draws a ScreenAlignedQuad in a texture; clears the FB first
	PreRender(target);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawQuad();
  PostRender();
}

void SciIllLib::SFilterEngine::Filter(GLuint target, GLfloat* color)
{
	//used by APostProcessor
	//draws a ScreenAlignedQuad in a texture; clears the FB first
	PreRender(target);
  glClearColor(color[0], color[1], color[2], color[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawQuad();
  PostRender();
}

void SciIllLib::SFilterEngine::DrawQuad()
{
  glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void SciIllLib::SFilterEngine::SetSize(const int width, const int height)
{
	if ((width == m_width) && (height == m_height))
  {
		return;
  }
  
  m_widthOld = m_width;
  m_heightOld = m_height;

	m_width = width;
	m_height = height;

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glBindRenderbuffer(GL_RENDERBUFFER, m_depthTex);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F ,m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthTex);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SciIllLib::SFilterEngine::SetSizeOld()
{
  SetSize(m_widthOld, m_heightOld);
}


void SciIllLib::SFilterEngine::ReCreateDepthBuffer(GLuint* rboId, const int width, const int height)
{
  if (*rboId > 0)
  {
        glDeleteRenderbuffers(1, rboId);
  }
	*rboId = 0;

  glGenRenderbuffers(1, rboId);
  glBindRenderbuffer(GL_RENDERBUFFER, *rboId);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void SciIllLib::SFilterEngine::ReGenerateTexture(GLuint*oldTex, const GLenum type)
{
  GLint intFormat = GL_RGBA8;
	if (type == GL_FLOAT)
  {
		intFormat = GL_RGBA32F;
  }
	ReGenerateTexture(oldTex, intFormat, type);
}

void SciIllLib::SFilterEngine::ReGenerateTexture(GLuint*oldTex, const GLenum format, const GLenum type)
{
	DeleteTexture(oldTex);
    
	glGenTextures(1, oldTex);
    
 	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);									// switch to the new framebuffer
	// initialize color texture
	glBindTexture(GL_TEXTURE_2D, *oldTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0,GL_RGBA, type, NULL);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
    
	glBindFramebuffer(GL_FRAMEBUFFER, 0);									// Swithch back to normal framebuffer rendering
}

void SciIllLib::SFilterEngine::ReGenerateTextureSize(GLuint*oldTex, const GLenum type, const int width, const int height)
{
  GLint intFormat = GL_RGBA8;
	if (type == GL_FLOAT)
  {
		intFormat = GL_RGBA32F;
  }
	ReGenerateTextureSize(oldTex, intFormat, type, width, height);
}

void SciIllLib::SFilterEngine::ReGenerateTextureSize(GLuint*oldTex, const GLenum format, const GLenum type, const int width, const int height)
{
	DeleteTexture(oldTex);
    
	glGenTextures(1, oldTex);
    
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);									// switch to the new framebuffer
	// initialize color texture
	glBindTexture(GL_TEXTURE_2D, *oldTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,GL_RGBA, type, NULL);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
    
	glBindFramebuffer(GL_FRAMEBUFFER, 0);									// Swithch back to normal framebuffer rendering
}

void SciIllLib::SFilterEngine::DeleteTexture(GLuint* oldTex)
{
  if (*oldTex > 0)
  {
        glDeleteTextures(1, oldTex);
  }
	*oldTex = 0;
}

SciIllLib::ATexture& SciIllLib::SFilterEngine::DummyTexture(){
  return m_texDummy;
}

