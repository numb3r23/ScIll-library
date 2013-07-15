/*
 * CMeshOBJ.h
 *
 *  Created on: Dec 25, 2012
 *      Author: numb3r23
 */

#ifndef CMESHOBJ_H_
#define CMESHOBJ_H_

#include "stdafx.h"

#include "CGLShaderProgram.h"
#include "CMesh.h"

#include <vector>

namespace SciIllLib {
    
    /**
     * The HeightMesh maps a heightmap onto a grid (every pixel is mapped onto a vertex).
     * The height is read from the red channel, a mask (0/1) is stored in the blue channel.
     */
    class CMeshOBJ : public CMesh
    {
    public:
        CMeshOBJ();
        ~CMeshOBJ();
        
        bool LoadFile(std::string);
        virtual GLfloat* GetBuffer(VBO element);
        void SetTB();
    private:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> tangents;
        std::vector<glm::vec3> binormals;
        
        std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
        std::vector<glm::vec3> temp_vertices;
        std::vector<glm::vec2> temp_uvs;
        std::vector<glm::vec3> temp_normals;

        void addGroup(bool bNeedsNormals);
    };
    
}
#endif /* CMESHOBJ_H_ */
