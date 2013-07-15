/*
 * CHeightmesh.h
 *
 *  Created on: Mar 30, 2011
 *      Author: numb3r23
 */

#ifndef CMESH_H_
#define CMESH_H_

#include "stdafx.h"

#include "CGLShaderProgram.h"

namespace SciIllLib {
    
    /**
     * The HeightMesh maps a heightmap onto a grid (every pixel is mapped onto a vertex).
     * The height is read from the red channel, a mask (0/1) is stored in the blue channel.
     */
    class CMesh
    {
    public:
        enum VBO {VBO_VERTEX, VBO_TEXCOORD, VBO_NORMAL, VBO_TANGENT, VBO_BINORMAL};

        CMesh();
        virtual ~CMesh();
        
        virtual void Draw();
        
        GLuint GetVAO();
        GLuint GetVBO(VBO);
        
        virtual GLfloat* GetBuffer(VBO element);

        int GetElementCount();
        
        CGLShaderProgram* Shader();
        
        void SetMatWorldScale(float size=1.0f);
        void MatWorldMult(glm::mat4 mat);
        void SetMatWorld(glm::mat4 mat=glm::mat4(1));
        glm::mat4* GetMatWorld();
        
        float GetModelRadius();
        
        void calculateTB(GLfloat* arrVertex, GLfloat* arrTexCoord);
        bool setupVertexAttrib(GLint loc, GLuint vbo, int dim, GLfloat* buff);
        
    protected:
        float m_fRadius;
        glm::mat4 m_world;
        
        GLfloat* m_arrVertex;
        GLfloat* m_arrNormal;
        GLfloat* m_arrTexCoord;
        GLfloat* m_arrTangent;
        GLfloat* m_arrBinormal;
        GLuint* m_arrIdx;
        
        int m_countElements;
        GLuint m_countIdx;
        
        GLuint m_vbo[5];
        GLuint m_vboIdx;
        GLuint m_vao;
        
        CGLShaderProgram* m_shader;
        
        bool InitializeGLBuffer();
        void cleanupBuffers();
        void setTanBin(int idx, GLfloat* u1, GLfloat* u2, GLfloat* v1, GLfloat* v2);
        
    };
    
}
#endif /* CHEIGHTMESH_H_ */
