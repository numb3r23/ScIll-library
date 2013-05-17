/*
 * STextureProcessor
 *
 *  Created on: Mar 2, 2011
 *      Author: numb3r23
 */

#ifndef STEXTURELOADER_H_
#define STEXTURELOADER_H_

#include "stdafx.h"
#include "CMesh.h"
namespace SciIllLib {
    
    class SGeometryGen{
    public:
        static CMesh* GenerateSphere(float radius, int stack=10, int slices=10);
    protected:
        SGeometryGen();
        virtual ~SGeometryGen();
    };
    
}
#endif /* CTEXTURELOADER_H_ */
