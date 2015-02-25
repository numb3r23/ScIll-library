/*
 * CCamera.h
 *
 *  Created on: Jan 15, 2011
 *      Author: numb3r23
 */

#ifndef CCAMERA_H_
#define CCAMERA_H_

#include "CGLShaderProgram.hpp"

namespace SciIllLib {
    
  /**
   * A Camera (position, direction) that can set its position to a shader
   */
  class CCamera{
  public:
    CCamera();
    
    /**
     * Reinitialize the camer to its default position:
     * theta, phi = 0
     * distance = 3
     */
    void Reset();
    
    glm::vec3& GetPosition();
    
    void Translate(const float x, const float y, const float z);
    
    void SetMousePosition(const int x, const int y);
    /**
     * Set changed mouse position - delta is calculated and applied!
     * @param x the x-coordinate
     * @param y the y-coordinate
     */
    void SetMouseMove(const int x, const int y);
    /**
     * Set a delta mouse rotation that is added to the angles
     * the new position is immediately recalculated
     * @param dx delta movement on the x-axis -> theta
     * @param dy delta movement on the y-axis -> phi
     */
    void SetMouseRotationDelta(const float dx, const float dy);
    /**
     * Set an abolute mouse rotation  -> set the angles
     * the new position is immediately recalculated
     * @param dx value (angle) for theta
     * @param dy value (angle) for phi
     */
    void SetMouseRotation(const float dx, const float dy);
    
    /**
     * Set a delta distances that is added to the current distance
     * the new position is immediately recalculated
     * @param distance delta distance
     */
    void SetDistanceDelta(const float distance);
    /**
     * Set an abolute camera distance
     * the new position is immediately recalculated
     * @param distance the new distance between camera and object
     */
    void SetDistance(const float distance);
    
    float GetDistance() const;
    
    /**
     * Set a delta distance and angles that are added to the current values
     * the new position is immediately recalculated
     * @param theta delta movement on the x-axis -> theta
     * @param phi delta movement on the y-axis -> phi
     * @param distance delta distance
     */
    void SetValuesDelta(const float theta, const float phi, const float distance);
    /**
     * Set absolut values for the camera
     * the new position is immediately recalculated
     * @param theta value (angle) for theta
     * @param phi value (angle) for phi
     * @param distance the new distance between camera and object
     */
    void SetValues(const float theta, const float phi, const float distance);

    void ShowInfo() const;
      
    void PosStore();
    void PosRecall();
    
    glm::mat4& GetViewMatrix();
      
    /**
     * Set the current view matrix (a.k.a. lookat) using the current values to a shader as a mat 4x4.
     * The shader has to be bound!
     * @param shader a pointer to a CGLShaderProgram instance
     * @param name the name of the shader parameter that is stores the ViewMatrix as a mat4x4
     */
    void SetViewMatrix(CGLShaderProgram& shader, const char* name);
    /**
     * Set the current camera position to a shader as a vec3.
     * The shader has to be bound!
     * @param shader a pointer to a CGLShaderProgram instance
     * @param name the name of the shader parameter that stores thecamera position as a vec3
     */
    void SetViewPosition(CGLShaderProgram& shader, const char* name);
  private:
    glm::mat4 m_mat4View;
    
    float m_fMouseX;
    float m_fMouseY;
    float m_theta;
    float m_phi;
    float m_distance;
    
    glm::vec3 m_v3Position;
    glm::vec3 m_v3SpherePosition;
    
    glm::vec3 m_v3Target;
    glm::vec3 m_v3Up;
    
    glm::vec3 m_v3PosStore;
    
    void Calculate();
  };
}
#endif /* CCAMERA_H_ */

