/*
 * ATexture.cpp
 *
 *  Created on: May 16, 2012
 *      Author: numb3r23
 */

#include "stdafx.h"

#include "ATexture.h"

/**
 * Construct a new instance, initialize width and height ...
 */
SciIllLib::ATexture::ATexture(){
    m_width = 1;
    m_height = 1;
    m_tex = 0;
}

SciIllLib::ATexture::ATexture(GLuint id){
    m_width = 1;
    m_height = 1;
	m_tex = id;
}

SciIllLib::ATexture::ATexture(GLuint id, int width, int height){
	m_tex = id;
	m_width = width;
    m_height = height;
}

SciIllLib::ATexture::~ATexture(){
	//TODO freigeben...
}

GLuint SciIllLib::ATexture::GetID(){
	return m_tex;
}

int SciIllLib::ATexture::GetWidth(){
	return m_width;
}
int SciIllLib::ATexture::GetHeight(){
	return m_height;
}

GLuint SciIllLib::ATexture::operator~(){
    return m_tex;
}
