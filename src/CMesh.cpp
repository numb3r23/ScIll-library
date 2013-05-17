/*
 * CHeightmesh.cpp
 *
 *  Created on: Mar 30, 2011
 *      Author: numb3r23
 */
#include "stdafx.h"

#include <iostream>
#include "CGLInfo.h"
#include "CMesh.h"

SciIllLib::CMesh::CMesh()
{
	m_arrVertex = NULL;
	m_arrNormal = NULL;
	m_arrTexCoord = NULL;
    m_arrTangent = NULL;
    m_arrBinormal = NULL;
	m_arrIdx = NULL;
    
    m_world = glm::mat4(1.0);
    
    m_fRadius = 1.0f;
    
    m_shader = new CGLShaderProgram();
}

/**
 * Delete all arrays (if not already done before....)
 */
SciIllLib::CMesh::~CMesh()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(3, &m_vbo[0]);
    glDeleteBuffers(1, &m_vboIdx);

    if (m_arrIdx != NULL)
        delete [] m_arrIdx;
    
    cleanupBuffers();
    
    delete m_shader;
}

void SciIllLib::CMesh::cleanupBuffers(){
	if (m_arrVertex != NULL)
	{
		delete [] m_arrVertex;
		m_arrVertex = NULL;
	}
	if (m_arrNormal != NULL)
	{
		delete [] m_arrNormal;
		m_arrNormal = NULL;
	}
	if (m_arrTexCoord != NULL)
	{
		delete [] m_arrTexCoord;
		m_arrTexCoord = NULL;
	}
	if (m_arrTangent != NULL)
	{
		delete [] m_arrTangent;
		m_arrTangent = NULL;
	}
	if (m_arrBinormal != NULL)
	{
		delete [] m_arrBinormal;
		m_arrBinormal = NULL;
	}
}

void SciIllLib::CMesh::Draw(){
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES,0, m_countElements);
}

SciIllLib::CGLShaderProgram* SciIllLib::CMesh::Shader(){
    return m_shader;
}

void SciIllLib::CMesh::SetMatWorldScale(float size){
    if (m_fRadius != 0.0f)
        m_world = glm::scale(glm::mat4(1.0), glm::vec3(size / m_fRadius));
}


void SciIllLib::CMesh::MatWorldMult(glm::mat4 mat){
    m_world *= mat;
}

void SciIllLib::CMesh::SetMatWorld(glm::mat4 mat){
    m_world = mat;
}

glm::mat4* SciIllLib::CMesh::GetMatWorld(){
    return &m_world;
}

float SciIllLib::CMesh::GetModelRadius(){
    return m_fRadius;
}

bool SciIllLib::CMesh::setupVertexAttrib(GLint loc, GLuint vbo, int dim, GLfloat* buff){
	if (loc <= 0)
        return false;
    else
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, m_countElements * dim * sizeof(GLfloat), buff, GL_STATIC_DRAW);
		glVertexAttribPointer(loc, dim, GL_FLOAT, GL_FALSE, 0, 0);
		CGLInfo::CheckError("VA initialized ?");
		glEnableVertexAttribArray(loc);
		CGLInfo::CheckError("VA initialized !");
	}
    return true;
}

GLfloat* SciIllLib::CMesh::GetBuffer(VBO){
    return NULL;
}

void SciIllLib::CMesh::setTanBin(int idx, GLfloat* u1, GLfloat* u2, GLfloat* v0, GLfloat* v1){
    float det = u1[0] * u2[1] - u2[0] * u1[1];
    
    m_arrTangent[idx]     = (v0[0] * u2[1] - v1[0] * u1[1]) / det;
    m_arrTangent[idx + 1] = (v0[1] * u2[1] - v1[1] * u1[1]) / det;
    m_arrTangent[idx + 2] = (v0[2] * u2[1] - v1[2] * u1[1]) / det;
    
    m_arrBinormal[idx]     = (-v0[0] * u2[0] + v1[0] * u1[0]) / det;
    m_arrBinormal[idx + 1] = (-v0[1] * u2[0] + v1[1] * u1[0]) / det;
    m_arrBinormal[idx + 2] = (-v0[2] * u2[0] + v1[2] * u1[0]) / det;
}

void SciIllLib::CMesh::calculateTB(GLfloat* arrVertex, GLfloat* arrTexCoord){
    m_arrTangent = new GLfloat[3 * m_countElements];
    m_arrBinormal = new GLfloat[3 * m_countElements];
    
    GLfloat* v0 = new GLfloat[3];
    GLfloat* v1 = new GLfloat[3];
    
    GLfloat* u1 = new GLfloat[2];
    GLfloat* u2 = new GLfloat[2];
    float det = 0.0f;
    for (int i=0; i < m_countElements; i+=3){
        for(int j = 0; j < 3; j++){
            v0[j] = arrVertex[i*3 + 3 + j] - arrVertex[i*3 + j];
            v1[j] = arrVertex[i*3 + 6 + j] - arrVertex[i*3 + j];
        }
        for(int j=0; j < 2; j++){
            u1[j] = arrTexCoord[i*2 + 2 + j] - arrTexCoord[i*2 + j];
            u2[j] = arrTexCoord[i*2 + 4 + j] - arrTexCoord[i*2 + j];
        }
        setTanBin(i * 3, u1, u2, v0, v1);
        
        for(int j = 0; j < 3; j++){
            v0[j] = arrVertex[i*3 + 6 + j] - arrVertex[i*3 + 3 + j];
            v1[j] = arrVertex[i*3 + j] - arrVertex[i*3 + 3 + j];
        }
        for(int j=0; j < 2; j++){
            u1[j] = arrTexCoord[i*2 + 4 + j] - arrTexCoord[i*2 + 2 + j];
            u2[j] = arrTexCoord[i*2 + j] - arrTexCoord[i*2 + 2 + j];
        }
        setTanBin(i * 3 + 3, u1, u2, v0, v1);
        
        for(int j = 0; j < 3; j++){
            v0[j] = arrVertex[i*3 + j] - arrVertex[i*3 + 6 + j];
            v1[j] = arrVertex[i*3 + 3 + j] - arrVertex[i*3 + 6 + j];
        }
        for(int j=0; j < 2; j++){
            u1[j] = arrTexCoord[i*2 + j] - arrTexCoord[i*2 + 4 + j];
            u2[j] = arrTexCoord[i*2 + 2 + j] - arrTexCoord[i*2 + 4 + j];
        }
        
        setTanBin(i * 3 + 6, u1, u2, v0, v1);
    }
}


/**
 * Loada heightmap and create the mesh.
 * @param filename filename that contains the heightmap
 * @param scale that is applied to the height (compared to max(width,height))
 * @param locAttrVertex location/index of the vertexattribute vertex-position
 * @param locAttrNormal location/index of the vertexattribute vertex-normal
 * @param locAttrTexCoord location/index of the vertexattribute vertex-texture coordinate
 */
bool SciIllLib::CMesh::InitializeGLBuffer()
{
    CGLInfo::CheckError("IntializeGLBuffer");
	GLint locAttrVertex = m_shader->attribLocation("vertex");
	GLint locAttrNormal = m_shader->attribLocation("normal");
	GLint locAttrTexCoord = m_shader->attribLocation("texCoord");
	GLint locAttrTangent = m_shader->attribLocation("tangent");
	GLint locAttrBinormal = m_shader->attribLocation("binormal");

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    
    
	glGenBuffers(5, &m_vbo[0]);
    
    bool found = false;
    found |= setupVertexAttrib(locAttrVertex,    m_vbo[VBO_VERTEX],      4, m_arrVertex);
    found |= setupVertexAttrib(locAttrNormal,    m_vbo[VBO_NORMAL],      4, m_arrNormal);
    found |= setupVertexAttrib(locAttrTexCoord,  m_vbo[VBO_TEXCOORD],    2, m_arrTexCoord);
    
    if (locAttrTangent * locAttrBinormal > 0){
        calculateTB(m_arrVertex, m_arrTexCoord);
        found |= setupVertexAttrib(locAttrTangent,   m_vbo[VBO_TANGENT],     3, m_arrTangent);
        found |= setupVertexAttrib(locAttrBinormal,  m_vbo[VBO_BINORMAL],    3, m_arrBinormal);
    }
    //found |= setupVertexAttrib(locAttrTangent,   m_vbo[VBO_TANGENT],     3, m_arrTangent);
    //found |= setupVertexAttrib(locAttrBinormal,  m_vbo[VBO_BINORMAL],    3, m_arrBinormal);
    /*
	int idx = 0;
	if (locAttrVertex >= 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VBO_VERTEX]);
		glBufferData(GL_ARRAY_BUFFER, m_countElements * 4 * sizeof(GLfloat), m_arrVertex, GL_STATIC_DRAW);
		glVertexAttribPointer(locAttrVertex, 4, GL_FLOAT, GL_FALSE, 0, 0);
		CGLInfo::CheckError("VA1 initialized ?");
		glEnableVertexAttribArray(locAttrVertex);
		CGLInfo::CheckError("VA1 initialized !");
	}

	if (locAttrNormal >= 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VBO_NORMAL]);
		glBufferData(GL_ARRAY_BUFFER, m_countElements * 4 * sizeof(GLfloat), m_arrNormal, GL_STATIC_DRAW);
		glVertexAttribPointer(locAttrNormal, 4, GL_FLOAT, GL_FALSE, 0, 0);
		CGLInfo::CheckError("VA2 initialized ?");
		glEnableVertexAttribArray(locAttrNormal);
		CGLInfo::CheckError("VA2 initialized");
	}

    if (locAttrTexCoord >= 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VBO_TEXCOORD]);
		glBufferData(GL_ARRAY_BUFFER, m_countElements * 2 * sizeof(GLfloat), m_arrTexCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(locAttrTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		CGLInfo::CheckError("VA3 initialized ?");
		glEnableVertexAttribArray(locAttrTexCoord);
		CGLInfo::CheckError("VA3 initialized");
	}

    if (locAttrTangent >= 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VBO_TANGENT]);
		glBufferData(GL_ARRAY_BUFFER, m_countElements * 2 * sizeof(GLfloat), m_arrTexCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(locAttrTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		CGLInfo::CheckError("VA3 initialized ?");
		glEnableVertexAttribArray(locAttrTexCoord);
		CGLInfo::CheckError("VA3 initialized");
	}
    
    if (locAttrTexCoord >= 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[idx++]);
		glBufferData(GL_ARRAY_BUFFER, m_countElements * 2 * sizeof(GLfloat), m_arrTexCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(locAttrTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		CGLInfo::CheckError("VA3 initialized ?");
		glEnableVertexAttribArray(locAttrTexCoord);
		CGLInfo::CheckError("VA3 initialized");
	}
    */
    if (found)
    {
        glGenBuffers(1, &m_vboIdx);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIdx);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_countIdx * sizeof(GLuint), m_arrIdx, GL_STATIC_DRAW);
    }

    cleanupBuffers();
    
    glBindVertexArray(0);

	return true;
}

GLuint SciIllLib::CMesh::GetVAO(){
    return m_vao;
}

GLuint SciIllLib::CMesh::GetVBO(VBO vbo){
    return m_vbo[vbo];
}

int SciIllLib::CMesh::GetElementCount(){
    return m_countElements;
}
