/*
 * CMeshOBJ.cp
 *
 *  Created on: Dec 25, 2012
 *      Author: numb3r23
 *
 * base code from http://code.google.com/p/opengl-tutorial-org/source/browse/common/objloader.cpp
 *
 * added:
 * + support for files without texcoord, normal or both
 * + support for groups (will be joined)
 * + calculation of perVertex normal
 *
 * note:
 * - if no texcoord present, (0,0) will be added (fake, unusable generation - better than nothing)
 * - draws each triangle "as is" - could be optimized with index-support, less CPU-GPU transfer!
 */
#include "stdafx.h"
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <fstream>

#include <iostream>

#include "CGLInfo.h"
#include "CMeshOBJ.h"

SciIllLib::CMeshOBJ::CMeshOBJ()
:CMesh(){
    m_fRadius = 0.0f;
}

/**
 * Delete all arrays (if not already done before....)
 */
SciIllLib::CMeshOBJ::~CMeshOBJ(){
}

bool SciIllLib::CMeshOBJ::LoadFile(std::string fname){
	FILE * file = fopen(fname.c_str(), "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		return false;
	}
    
    bool bNeedsNormals = false;
    int offset = 0;
    
	while( 1 ){
        
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
        
		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
            char line[256];
            fgets(line, 256, file);
			int matches = sscanf(line, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
                                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                 &vertexIndex[2], &uvIndex[2], &normalIndex[2],
                                 &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
            
			if (matches < 9){
                int matches = sscanf(line, "%d//%d %d//%d %d//%d %d//%d\n",
                                     &vertexIndex[0], &normalIndex[0],
                                     &vertexIndex[1], &normalIndex[1],
                                     &vertexIndex[2], &normalIndex[2],
                                     &vertexIndex[3], &normalIndex[3]);
                if ( matches >= 6){
                    //has vertices + normals
                    temp_uvs.push_back(glm::vec2(0));
                    int idxUV = temp_uvs.size();
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    uvIndices    .push_back(idxUV);
                    uvIndices    .push_back(idxUV);
                    uvIndices    .push_back(idxUV);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[1]);
                    normalIndices.push_back(normalIndex[2]);
                    if (matches == 8){
                        vertexIndices.push_back(vertexIndex[0]);
                        vertexIndices.push_back(vertexIndex[2]);
                        vertexIndices.push_back(vertexIndex[3]);
                        uvIndices    .push_back(idxUV);
                        uvIndices    .push_back(idxUV);
                        uvIndices    .push_back(idxUV);
                        normalIndices.push_back(normalIndex[0]);
                        normalIndices.push_back(normalIndex[2]);
                        normalIndices.push_back(normalIndex[3]);
                    }
                }else{
                    //check for texture coordinates...
                    int matches = sscanf(line, "%d/%d %d/%d %d/%d %d/%d\n",
                                         &vertexIndex[0], &uvIndex[0],
                                         &vertexIndex[1], &uvIndex[1],
                                         &vertexIndex[2], &uvIndex[2],
                                         &vertexIndex[3], &uvIndex[3]);
                    if (matches >= 6){
                        bNeedsNormals = true;
                        temp_normals.push_back(glm::vec3(0));
                        int idxNrm = temp_normals.size();
                        vertexIndices.push_back(vertexIndex[0]);
                        vertexIndices.push_back(vertexIndex[1]);
                        vertexIndices.push_back(vertexIndex[2]);
                        uvIndices    .push_back(uvIndex[0]);
                        uvIndices    .push_back(uvIndex[1]);
                        uvIndices    .push_back(uvIndex[2]);
                        normalIndices    .push_back(idxNrm);
                        normalIndices    .push_back(idxNrm);
                        normalIndices    .push_back(idxNrm);
                        if (matches == 8){
                            vertexIndices.push_back(vertexIndex[0]);
                            vertexIndices.push_back(vertexIndex[2]);
                            vertexIndices.push_back(vertexIndex[3]);
                            uvIndices    .push_back(uvIndex[0]);
                            uvIndices    .push_back(uvIndex[2]);
                            uvIndices    .push_back(uvIndex[3]);
                            normalIndices    .push_back(idxNrm);
                            normalIndices    .push_back(idxNrm);
                            normalIndices    .push_back(idxNrm);
                        }
                    }else{
                     //last chance - only vertices???
                        int matches = sscanf(line, "%d %d %d %d\n",
                                             &vertexIndex[0],&vertexIndex[1],
                                             &vertexIndex[2], &vertexIndex[3]);
                        if (matches >= 3){
                            bNeedsNormals = true;
                            temp_uvs.push_back(glm::vec2(0));
                            temp_normals.push_back(glm::vec3(0));
                            int idxUV = temp_uvs.size();
                            int idxNrm = temp_normals.size();
                            
                            vertexIndices.push_back(vertexIndex[0]);
                            vertexIndices.push_back(vertexIndex[1]);
                            vertexIndices.push_back(vertexIndex[2]);
                            uvIndices    .push_back(idxUV);
                            uvIndices    .push_back(idxUV);
                            uvIndices    .push_back(idxUV);
                            normalIndices    .push_back(idxNrm);
                            normalIndices    .push_back(idxNrm);
                            normalIndices    .push_back(idxNrm);
                            if (matches == 4){
                                vertexIndices.push_back(vertexIndex[0]);
                                vertexIndices.push_back(vertexIndex[2]);
                                vertexIndices.push_back(vertexIndex[3]);
                                uvIndices    .push_back(idxUV);
                                uvIndices    .push_back(idxUV);
                                uvIndices    .push_back(idxUV);
                                normalIndices    .push_back(idxNrm);
                                normalIndices    .push_back(idxNrm);
                                normalIndices    .push_back(idxNrm);
                            }
                        }
                    }
                }
			}else{
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices    .push_back(uvIndex[0]);
                uvIndices    .push_back(uvIndex[1]);
                uvIndices    .push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
                
                if (matches == 12){
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[2]);
                    vertexIndices.push_back(vertexIndex[3]);
                    uvIndices    .push_back(uvIndex[0]);
                    uvIndices    .push_back(uvIndex[2]);
                    uvIndices    .push_back(uvIndex[3]);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[2]);
                    normalIndices.push_back(normalIndex[3]);
                }
            }
		}
        else if ( strcmp( lineHeader, "g" ) == 0 ){
            offset = vertexIndices.size();
        }else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
        
	}
    
    addGroup(bNeedsNormals);
    
    if (m_fRadius > 0.0f)
        m_fRadius = sqrt(m_fRadius);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    
    glGenBuffers(5, m_vbo);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VBO_VERTEX]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VBO_TEXCOORD]);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VBO_NORMAL]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    
    m_countElements = vertices.size();
}

void SciIllLib::CMeshOBJ::SetTB(){
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VBO_TANGENT]);
//    glBufferData(GL_ARRAY_BUFFER, m_countElements * 3 * sizeof(GLfloat), m_arrTangent, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, m_countElements * 3 * sizeof(GLfloat), &tangents[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VBO_BINORMAL]);
    glBufferData(GL_ARRAY_BUFFER, m_countElements * 3 * sizeof(GLfloat), &binormals[0], GL_STATIC_DRAW);
//    glBufferData(GL_ARRAY_BUFFER, m_countElements * 3 * sizeof(GLfloat), m_arrBinormal, GL_STATIC_DRAW);
    
    
}

GLfloat* SciIllLib::CMeshOBJ::GetBuffer(VBO vbo){
    switch (vbo) {
        case CMesh::VBO_VERTEX:
            return (GLfloat*) &vertices[0];
        case CMesh::VBO_TEXCOORD:
            return (GLfloat*) &uvs[0];
        case CMesh::VBO_NORMAL:
            return (GLfloat*) &normals[0];
        default:
            break;
    }
    return NULL;
}

void SciIllLib::CMeshOBJ::addGroup(bool bNeedsNormals){
    std::map<unsigned int, std::vector<unsigned int> > mapVec2Tris;
    // For each vertex of each triangle
    unsigned int offset = vertices.size();
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){
        
		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		
        m_fRadius = std::max((float)glm::dot(vertex, vertex), m_fRadius);
		// Put the attributes in buffers
		vertices.push_back(vertex);
		uvs     .push_back(uv);
		normals .push_back(normal);
        tangents.push_back(normal);
        binormals.push_back(normal);
        mapVec2Tris[vertexIndex].push_back(div(i, 3).quot);
	}
    
    //calc TB here
    //calculateTB((GLfloat*) &vertices[0], (GLfloat*) &uvs[0]);
    
    if (bNeedsNormals){
        std::cout << "calculating perVertex normal...";
        std::map<unsigned int, glm::vec3> mapVec2Nrm;
        for( unsigned int i=0; i<vertexIndices.size(); i++ ){
            mapVec2Nrm[vertexIndices[i]] = glm::vec3(0,0,0);
        }
        
        for (unsigned int i=0; i<vertices.size(); i+=3){
            glm::vec3 a = glm::normalize(vertices[offset + i + 1] - vertices[offset + i]);
            glm::vec3 b = glm::normalize(vertices[offset + i + 2] - vertices[offset + i]);
            glm::vec3 normal = glm::cross(a, b);
            mapVec2Nrm[vertexIndices[i]] += normal;
            mapVec2Nrm[vertexIndices[i + 1]] += normal;
            mapVec2Nrm[vertexIndices[i + 2]] += normal;
        }
        for( unsigned int i=0; i<vertexIndices.size(); i++ ){
            normals[i + offset] = glm::normalize(mapVec2Nrm[vertexIndices[i]]);
        }
        std::cout << "done" << std::endl;
    }
    
        std::cout << "calculating perVertex binormal/tangent...";
        std::map<unsigned int, glm::vec3> mapVec2Tan;
        std::map<unsigned int, glm::vec3> mapVec2Bin;
        for( unsigned int i=0; i<vertexIndices.size(); i++ ){
            mapVec2Tan[vertexIndices[i]] = glm::vec3(0,0,0);
            mapVec2Bin[vertexIndices[i]] = glm::vec3(0,0,0);
        }
        
        for (unsigned int i=0; i<vertices.size(); i+=3){
            glm::vec3 v0 = glm::normalize(vertices[offset + i + 1] - vertices[offset + i]);
            glm::vec3 v1 = glm::normalize(vertices[offset + i + 2] - vertices[offset + i]);
            
            glm::vec2 u0 = uvs[offset + i + 1] - uvs[offset + i];
            glm::vec2 u1 = uvs[offset + i + 2] - uvs[offset + i];
            
            float det = u0.x * u1.y - u1.x * u0.y;
            mapVec2Tan[vertexIndices[i]] += glm::vec3(
                                          (v0.x * u1.y - v1.x * u0.y) / det,
                                          (v0.y * u1.y - v1.y * u0.y) / det,
                                          (v0.z * u1.y - v1.z * u0.y) / det
            );
            mapVec2Bin[vertexIndices[i]] += glm::vec3(
                                          (-v0.x * u1.x + v1.x * u0.x) / det,
                                          (-v0.y * u1.x + v1.y * u0.x) / det,
                                          (-v0.z * u1.x + v1.z * u0.x) / det
                                          );

            v0 = glm::normalize(vertices[offset + i + 2] - vertices[offset + i + 1]);
            v1 = glm::normalize(vertices[offset + i] - vertices[offset + i + 1]);
            
            u0 = uvs[offset + i + 2] - uvs[offset + i + 1];
            u1 = uvs[offset + i] - uvs[offset + i + 1];
            
            det = u0.x * u1.y - u1.x * u0.y;
            mapVec2Tan[vertexIndices[i + 1]] += glm::vec3(
                                                      (v0.x * u1.y - v1.x * u0.y) / det,
                                                      (v0.y * u1.y - v1.y * u0.y) / det,
                                                      (v0.z * u1.y - v1.z * u0.y) / det
                                                      );
            mapVec2Bin[vertexIndices[i + 1]] += glm::vec3(
                                                      (-v0.x * u1.x + v1.x * u0.x) / det,
                                                      (-v0.y * u1.x + v1.y * u0.x) / det,
                                                      (-v0.z * u1.x + v1.z * u0.x) / det
                                                      );

            v0 = glm::normalize(vertices[offset + i] - vertices[offset + i + 2]);
            v1 = glm::normalize(vertices[offset + i + 1] - vertices[offset + i + 2]);
            
            u0 = uvs[offset + i] - uvs[offset + i + 2];
            u1 = uvs[offset + i + 1] - uvs[offset + i + 2];
            
            det = u0.x * u1.y - u1.x * u0.y;
            mapVec2Tan[vertexIndices[i + 2]] += glm::vec3(
                                                          (v0.x * u1.y - v1.x * u0.y) / det,
                                                          (v0.y * u1.y - v1.y * u0.y) / det,
                                                          (v0.z * u1.y - v1.z * u0.y) / det
                                                          );
            mapVec2Bin[vertexIndices[i + 2]] += glm::vec3(
                                                          (-v0.x * u1.x + v1.x * u0.x) / det,
                                                          (-v0.y * u1.x + v1.y * u0.x) / det,
                                                          (-v0.z * u1.x + v1.z * u0.x) / det
                                                          );
        }
        for( unsigned int i=0; i<vertexIndices.size(); i++ ){
            tangents[i + offset] = glm::normalize(mapVec2Tan[vertexIndices[i]]);
            binormals[i + offset] = glm::normalize(mapVec2Bin[vertexIndices[i]]);
        }
        std::cout << "done" << std::endl;
    vertexIndices.clear();
    uvIndices.clear();
    normalIndices.clear();
    
    temp_vertices.clear();
    temp_uvs.clear();
    temp_normals.clear();

}