/*
 * STextureLoader.cpp
 *
 *  Created on: Mar 2, 2011
 *      Author: numb3r23
 */

#include <string>
#include <fstream>
#include <sstream>

#include "STextureLoader.h"

SciIllLib::STextureLoader::STextureLoader()
{

}

SciIllLib::STextureLoader::~STextureLoader()
{

}

/**
 * Load a FloatTXT as an OpenGL texture and set the Min/Mag Filter to Nearest
 * @param fname The filename that should be loaded
 * @returns id or 0
 */
GLuint SciIllLib::STextureLoader::FTXTLoader(std::string fname) {
    GLuint tex = 0;
	int height = 0;
	int width = 0;
    
	float* data = LoadFTXTData(fname, &width, &height);
	if (data != NULL){
        tex = CreateTexture(GL_FLOAT, data, width, height);
        delete data;
    }else{
        std::cout << "! failed loading " << fname << std::endl;
    }
    
    return tex;
}

/**
 * Create an OpenGL texture
 * @param data-type data pointer to float data
 * @param width width of the texture
 * @param height height of the texture
 * @return id or 0
 */
GLuint SciIllLib::STextureLoader::CreateTexture(GLint type, void* data, int width, int height){
    GLuint tex = 0;
    glGenTextures(1, &tex);
    if (tex <= 0)
        return tex;
    
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, type, data);
    
    return tex;
}


/**
 * Load data from a FTXT file so it can be set as an OpenGL texture
 * @param fname The filename that should be loaded
 * @param items The number of elemnts found
 * @param width the width of the texture
 * @param height the height of the texture
 * @returns a pointer to the raw data array
 */

float* SciIllLib::STextureLoader::LoadFTXTData(std::string fname, int* width, int *height)
{
	int items = 0;
	*width = 0;
	*height = 0;

    std::ifstream ifs(fname.c_str());
    std::string temp;

	if (getline( ifs, temp ))
	{
		int p      = temp.find_first_of(";");
		*width  = std::atoi(temp.substr(0, p).c_str());
		int p_     = temp.find_first_of(";",p+1);
		*height = std::atoi(temp.substr(p + 1, p_ - p - 1).c_str());
		items  = *width * *height * 4;

		if (items <= 0 ){
            *width = 0;
            *height = 0;
            return NULL;
        }
        
        float* data = new float[items];

		int idx = 0;
		int p1, p2, p3, p4;
		while( getline( ifs, temp ) )
		{
			p1 = temp.find_first_of(";");
			p2 = temp.find_first_of(";",p1+1);
			p3 = temp.find_first_of(";",p2+1);
			p4 = temp.find_first_of(";",p3+1);

			data[idx++] = std::atof(temp.substr(0, p1).c_str());
			data[idx++] = std::atof(temp.substr(p1 + 1, p2 - p1 - 1).c_str());
			data[idx++] = std::atof(temp.substr(p2 + 1, p3 - p2 - 1).c_str());
			data[idx++] = std::atof(temp.substr(p3 + 1, p4 - p3 - 1).c_str());
		}
		return data;
	}
	else
		return NULL;

	return NULL;
}
