/*
 * SFilterEngine.cpp
 *
 *  Created on: Feb 13, 2011
 *      Author: numb3r23
 */

#include "stdafx.h"

#include "SFilterEngine.h"
#include "CGLInfo.h"

int SciIllLib::SFilterEngine::m_width, SciIllLib::SFilterEngine::m_height;
int SciIllLib::SFilterEngine::m_widthOld, SciIllLib::SFilterEngine::m_heightOld;

GLuint SciIllLib::SFilterEngine::m_vao;
GLuint SciIllLib::SFilterEngine::m_vab[3];

GLuint SciIllLib::SFilterEngine::m_fbo, SciIllLib::SFilterEngine::m_depthTex;
SciIllLib::ATexture* SciIllLib::SFilterEngine::m_texDummy;

GLuint SciIllLib::SFilterEngine::m_rbo;

/**
 * Construct a new instance, initialize width and height ...
 */
SciIllLib::SFilterEngine::SFilterEngine() {
}

SciIllLib::SFilterEngine::~SFilterEngine() {
	glDeleteBuffers(3, &m_vab[0]);
    glDeleteVertexArrays(1, &m_vao);

	glDeleteRenderbuffers(1, &m_depthTex);
	glDeleteFramebuffers(1, &m_fbo);
    delete m_texDummy;
}

/**
 * Initialize VBO/VAO for the creenaligned quad and generate Frame- and Renderbuffer
 * @param width the current Viewport width
 * @param height the current Viewport height
 * @returns success?
 */

bool SciIllLib::SFilterEngine::Initialize(int width, int height)
{
    m_texDummy = new ATexture(0);

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

/**
 * Apply a filter sequence - as indicated by the missing parameters the functions is NOP
 */
void SciIllLib::SFilterEngine::ApplySequence()
{

}

/**
 * Apply a pure filter 
 * @deprecated Use another Apply function!
 * @param source the source TextureID
 * @param target the target TextureID
 */

void SciIllLib::SFilterEngine::Apply(GLuint source, GLuint target)
{
    ApplyPure(source, target);
}

void SciIllLib::SFilterEngine::Apply(SciIllLib::AFilter *shader, GLuint source, SciIllLib::ATexture *target, glm::vec4 clear){
	//CGLInfo::CheckError("PreFilter");
	PreRender(target);
    
    glClearColor(clear.r, clear.g, clear.b, clear.a);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //Clear(&clear);
    
    ToScreen(shader, source);
    /*
    shader->bind();
	shader->BindSampler(0, source);
	
	DrawQuad();
	
	shader->BindSampler(0, 0);
    shader->release();
    */
	PostRender();
	//CGLInfo::CheckError("PostFilter");
}

/*
void SciIllLib::SFilterEngine::Apply(SciIllLib::AFilter *shader, SciIllLib::ATexture *source, SciIllLib::ATexture *target){
    Apply(shader, source->GetID(), target);
}

void SciIllLib::SFilterEngine::Apply(SFilterCommon::Filter flt, ATexture* source, ATexture* target)
{
    Apply(SFilterCommon::Get(flt), source, target);
}
*/

/**
 * Prepare Multiple Rendering to Texture and set all Framebuffers and activate the Buffers
 * @param count number of framebuffers/RenderTargets
 * @param targets pointer to a textureID array
 * @param buffers pointer to a bufferID array
 */

void SciIllLib::SFilterEngine::PreRender(int count, GLuint* targets, GLenum* buffers)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	for (int i=0; i < count; i++)
		glFramebufferTexture(GL_FRAMEBUFFER,buffers[i],targets[i], 0);
    glDrawBuffers(count, buffers);
}

/**
 * Prepare single Rendering to Texture and set all Framebuffers and activate the Buffers
 * @param target the RenderTextures textureID
 */
void SciIllLib::SFilterEngine::PreRender(ATexture * target)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, target->GetID(), 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthTex);
	glViewport(0.0, 0.0, target->GetWidth(), target->GetHeight());
}

void SciIllLib::SFilterEngine::PreRender(GLuint target)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, target, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

/**
 * Unset the Framebuffer
 */
void SciIllLib::SFilterEngine::PostRender()
{
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 0, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * Return the current width
 */
int SciIllLib::SFilterEngine::GetWidth()
{
	return m_width;
}

/**
 * Return the current height
 */
int SciIllLib::SFilterEngine::GetHeight()
{
	return m_height;
}

/**
 * Apply a pure filter 
 * @deprecated Use another Apply function!
 * @param source the source TextureID
 * @param target the target TextureID
 */
void SciIllLib::SFilterEngine::ApplyPure(GLuint source, GLuint target)
{
	PreRender(target);
	DrawQuad();
    PostRender();
}

/**
 * Bind the shaders from the Filter and filter the source texture to screen
 * @param shader the filter that should be used
 * @param texture the TextureID of the base image that is filtered
 */
void SciIllLib::SFilterEngine::ToScreen(AFilter* shader, GLuint texture)
{
	shader->bind();
	shader->BindSampler(0, texture);
	DrawQuad();
	shader->BindSampler(0, 0);
    shader->release();
}

/**
 * Bind the shaders from the Filter and filter the source texture to screen
 * @param shader the filter that should be used
 * @param texture the TextureID of the base image that is filtered
void SciIllLib::SFilterEngine::ToScreen(SFilterCommon::Filter flt, ATexture* texture)
{
	ToScreen(SFilterCommon::Get(flt), texture->GetID());
}
 */

/**
 * Bind the shaders from the Filter and filter the source texture to screen
 * @param shader the filter that should be used
 * @param texture the TextureID of the base image that is filtered
void SciIllLib::SFilterEngine::ToScreen(AFilter* shader, ATexture* texture)
{
	ToScreen(shader, texture->GetID());
}
 */

/**
 * Bind the shaders from the Filter and filter to screen.
 * The Filter should have the textures already assigned!
 * @param shader the filter that should be used
 */
void SciIllLib::SFilterEngine::ToScreen(AFilter* shader)
{
	shader->bind();
	DrawQuad();
    shader->release();
}

/**
 * Apply a filter and filter the source into the target. Clear with color (0, 0, 0, 0)! 
 * @param shader the filter that should be used
 * @param source the TextureID of the base image
 * @param target the TextureID of the target image
void SciIllLib::SFilterEngine::Apply(AFilter* shader, GLuint source, GLuint target)
{
    PreRender(target);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	ToScreen(shader, source);
    PostRender();
}
 */

/**
 * Apply a filter and filter the source into the target, Use the assigned clearcolor
 * @param shader the filter that should be used
 * @param source the TextureID of the base image
 * @param target the TextureID of the target image
 * @param color use this color for clearing before rendering
void SciIllLib::SFilterEngine::Apply(AFilter* shader, GLuint source, GLuint target, glm::vec4 clear)
{
    PreRender(target);
    glClearColor(clear.r, clear.g, clear.b, clear.a);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	ToScreen(shader, source);
    PostRender();
}
 */

/**
 * Set the Rendertarget, clear with color (0, 0, 0, 0) and render a screen-aligned-quad
 * @param target the TextureID of the target image
 */
void SciIllLib::SFilterEngine::Filter(GLuint target)
{
	//used by APostProcessor
	//draws a ScreenAlignedQuad in a texture; clears the FB first
	PreRender(target);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawQuad();
    PostRender();
}

/**
 * Set the Rendertarget, clear with the assigned color and render a screen-aligned-quad
 * @param target the TextureID of the target image
 * @param color use this color for clearing before rendering
 */
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

/**
 * Draw a screen-aligned-quad (using a trinagle fan if anyone cares)
 */
void SciIllLib::SFilterEngine::DrawQuad()
{
    glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

/**
 * Resize the Viewport means resize all reandertargets
 * @param width the viewport width
 * @param height the viewport height
 */
void SciIllLib::SFilterEngine::SetSize(int width, int height)
{
	if ((width == m_width) && (height == m_height))
		return;
    
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


void SciIllLib::SFilterEngine::ReCreateDepthBuffer(GLuint* rboId, int width, int height){
    if (*rboId > 0)
        glDeleteRenderbuffers(1, rboId);
	*rboId = 0;

    glGenRenderbuffers(1, rboId);
    glBindRenderbuffer(GL_RENDERBUFFER, *rboId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

/**
 * (Re)Generate a Texture (possible use: RenderTarget) with the specified type, if oldTex is !+= 0 the texture is deleted first
 * @param oldTex a pointer to the old & new TextureID
 * @param type Specifies the data type of the pixel data. If set to GL_FLOAT, use GL_RGBA32F for internal format, otherwise GL_RGBA8
 */
void SciIllLib::SFilterEngine::ReGenerateTexture(GLuint*oldTex, GLenum type)
{
    GLint intFormat = GL_RGBA8;
	if (type == GL_FLOAT)
		intFormat = GL_RGBA32F;
	ReGenerateTexture(oldTex, intFormat, type);

}

/**
 * Generate a Texture (possible use: RenderTarget) with the specified type, if oldTex is !+= 0 the texture is deleted first
 * @param oldTex a pointer to the old & new TextureID
 * @param format Specifies the number of color components in the texture.
 * @param type Specifies the data type of the pixel data.
 */
void SciIllLib::SFilterEngine::ReGenerateTexture(GLuint*oldTex, GLenum format, GLenum type)
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

/**
 * (Re)Generate a Texture (possible use: RenderTarget) with the specified type, if oldTex is !+= 0 the texture is deleted first.
 * Use the specified size!
 * @param oldTex a pointer to the old & new TextureID
 * @param type Specifies the data type of the pixel data. If set to GL_FLOAT, use GL_RGBA32F for internal format, otherwise GL_RGBA8
 * @param width width of the texture
 * @param height height of the texture
 */
void SciIllLib::SFilterEngine::ReGenerateTextureSize(GLuint*oldTex, GLenum type, int width, int height)
{
    GLint intFormat = GL_RGBA8;
	if (type == GL_FLOAT)
		intFormat = GL_RGBA32F;
	ReGenerateTextureSize(oldTex, intFormat, type, width, height);

}

/**
 * Generate a Texture (possible use: RenderTarget) with the specified type, if oldTex is !+= 0 the texture is deleted first
 * Use the specified size!
 * @param oldTex a pointer to the old & new TextureID
 * @param format Specifies the number of color components in the texture.
 * @param type Specifies the data type of the pixel data.
 * @param width width of the texture
 * @param height height of the texture
 */
void SciIllLib::SFilterEngine::ReGenerateTextureSize(GLuint*oldTex, GLenum format, GLenum type, int width, int height)
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

/**
 * Delete the texture if the ID is > 0
 * @param oldTex a pointer to the old TextureID
 */
void SciIllLib::SFilterEngine::DeleteTexture(GLuint* oldTex)
{
    if (*oldTex > 0)
        glDeleteTextures(1, oldTex);
	*oldTex = 0;
}

SciIllLib::ATexture* SciIllLib::SFilterEngine::DummyTexture(){
    return m_texDummy;
}

