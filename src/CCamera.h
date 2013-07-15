/*
 * CCamera.h
 *
 *  Created on: Jan 15, 2011
 *      Author: numb3r23
 */

#ifndef CCAMERA_H_
#define CCAMERA_H_

#include "CGLShaderProgram.h"

namespace SciIllLib {
    
    /**
     * A Camera (position, direction) that can set its position to a shader
     */
    class CCamera{
    public:
        CCamera();
        void Reset();
        
        glm::vec3* GetPosition();
        
        void Translate(float x, float y, float z);
        
        void SetMousePosition(int x, int y);
        void SetMouseMove(int x, int y);
        
        void SetMouseRotationDelta(float dx, float dy);
        void SetMouseRotation(float dx, float dy);
        
        void SetDistanceDelta(float distance);
        void SetDistance(float distance);
        float GetDistance();
        
        void SetValuesDelta(float theta, float phi, float distance);
        void SetValues(float theta, float phi, float distance);

		void ShowInfo();
        
        void PosStore();
        void PosRecall();
        
        glm::mat4 GetViewMatrix();
        
        void SetViewMatrix(CGLShaderProgram* shader, const char* name);
		void SetViewPosition(CGLShaderProgram* shader, const char* name);
    private:
        glm::mat4 m_mat4View;
        
        float m_fMouseX;
        float m_fMouseY;
        float m_theta;
        float m_phi;
        float m_distance;
        
        //glm
        glm::vec3 v3Position;
        glm::vec3 v3SpherePosition;
        
        glm::vec3 v3Target;
        glm::vec3 v3Up;
        
        glm::vec3 v3PosStore;
        
        void Calculate();
    };
    
}
#endif /* CCAMERA_H_ */

