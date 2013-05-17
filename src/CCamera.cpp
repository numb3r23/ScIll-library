/*
 * CCamera.cpp
 *
 *  Created on: Jan 15, 2011
 *      Author: numb3r23
 */

#include "stdafx.h"

#include "CCamera.h"


SciIllLib::CCamera::CCamera(){
	Reset();
}

/**
 * Reinitialize the camer to its default position: 
 * theta, phi = 0
 * distance = 3
 */
void SciIllLib::CCamera::Reset(){
	m_theta = 0.0f;
	m_phi = 0.0f;
	m_distance = 4.0f;
    
    m_fMouseX = 0.0f;
    m_fMouseY = 0.0f;

	v3Position = glm::vec3(0, 1, 5);
	v3SpherePosition = v3Position;

	v3Target = glm::vec3(0, 0, 0);
	v3Up = glm::vec3(0, 1, 0);

	Calculate();
}

/**
 * Return the position
 * @returns the position as a vec3
 */
glm::vec3* SciIllLib::CCamera::GetPosition()
{
	return &v3Position;
}

void SciIllLib::CCamera::PosStore(){
    v3PosStore = glm::vec3(m_theta, m_phi, m_distance); 
}

void SciIllLib::CCamera::PosRecall(){
    m_theta = v3PosStore.x;
    m_phi = v3PosStore.y;
    m_distance = v3PosStore.z; 
    Calculate();
}


/**
 * Translate the center of the camera
 * @param x the translation on the x-axis
 * @param y the translation on the y-axis
 * @param z the translation on the z-axis
 */
void SciIllLib::CCamera::Translate(float x, float y, float z){
	v3Target += glm::vec3(x, y, z);
	v3Position = v3SpherePosition + v3Target;
}

/**
 * Set absolute mouse position (no change to rotation - use for mousedown)
 * @param x the x-coordinate
 * @param y the y-coordinate
 */
void SciIllLib::CCamera::SetMousePosition(int x, int y){
    m_fMouseX = x;
    m_fMouseY = y;
}

/**
 * Set changed mouse position - delta is calculated and applied!
 * @param x the x-coordinate
 * @param y the y-coordinate
 */
void SciIllLib::CCamera::SetMouseMove(int x, int y){
    SetMouseRotationDelta(x - m_fMouseX, y - m_fMouseY);
    
    m_fMouseX = x;
    m_fMouseY = y;
}

/**
 * Set a delta mouse rotation that is added to the angles
 * the new position is immediately recalculated
 * @param dx delta movement on the x-axis -> theta
 * @param dy delta movement on the y-axis -> phi
 */
void SciIllLib::CCamera::SetMouseRotationDelta(float dx, float dy){
	m_theta -= dx;
	m_phi += dy;

	Calculate();
}

/**
 * Set an abolute mouse rotation  -> set the angles
 * the new position is immediately recalculated
 * @param dx value (angle) for theta 
 * @param dy value (angle) for phi 
 */
void SciIllLib::CCamera::SetMouseRotation(float dx, float dy){
	m_theta = dx;
	m_phi = dy;

	Calculate();
}

/**
 * Set a delta distances that is added to the current distance
 * the new position is immediately recalculated
 * @param distance delta distance 
 */
void SciIllLib::CCamera::SetDistanceDelta(float distance)
{
	m_distance += distance;

	Calculate();
}

/**
 * Set an abolute camera distance
 * the new position is immediately recalculated
 * @param distance the new distance between camera and object
 */
void SciIllLib::CCamera::SetDistance(float distance)
{
	m_distance = distance;

	Calculate();
}

/**
 * Set a delta distance and angles that are added to the current values
 * the new position is immediately recalculated
 * @param theta delta movement on the x-axis -> theta
 * @param phi delta movement on the y-axis -> phi
 * @param distance delta distance 
 */
void SciIllLib::CCamera::SetValuesDelta(float theta, float phi, float distance)
{
	m_theta -= theta;
	m_phi += phi;
	m_distance += distance;

	Calculate();
}

float SciIllLib::CCamera::GetDistance()
{
	return m_distance;
}

/**
 * Set absolut values for the camera
 * the new position is immediately recalculated
 * @param theta value (angle) for theta 
 * @param phi value (angle) for phi 
 * @param distance the new distance between camera and object
 */
void SciIllLib::CCamera::SetValues(float theta, float phi, float distance)
{
	m_theta = theta;
	m_phi = phi;
	m_distance = distance;

	Calculate();
}

/**
 * Set the current view matrix (a.k.a. lookat) using the current values to a shader as a mat 4x4.
 * The shader has to be bound!
 * @param shader a pointer to a CGLShaderProgram instance
 * @param name the name of the shader parameter that is stores the ViewMatrix as a mat4x4
 */
void SciIllLib::CCamera::SetViewMatrix(CGLShaderProgram* shader, const char* name)
{
	shader->setUniformValue(name, m_mat4View);
}

glm::mat4 SciIllLib::CCamera::GetViewMatrix(){
    return m_mat4View;
}

/**
 * Set the current camera position to a shader as a vec3.
 * The shader has to be bound!
 * @param shader a pointer to a CGLShaderProgram instance
 * @param name the name of the shader parameter that stores thecamera position as a vec3
 */
void SciIllLib::CCamera::SetViewPosition(CGLShaderProgram* shader, const char* name)
{
	shader->setUniformValue(name, v3Position.x, v3Position.y, v3Position.z);
}

/**
 * Recalculate position and up vector of the camera
 */
void SciIllLib::CCamera::Calculate()
{
//glm	
	glm::mat4 mat4 = glm::mat4(1.0f);
	mat4 = glm::rotate(mat4, m_theta, glm::vec3(0, 1, 0));
	mat4 = glm::rotate(mat4, m_phi, glm::vec3(1, 0, 0));

	glm::vec4 v4B = glm::vec4(0, 0, m_distance, 0);
	glm::vec4 v4U = glm::vec4(0, 1, 0, 0);
	v4B = mat4 * v4B;
	v4U = mat4 * v4U;

	v3SpherePosition = glm::vec3(v4B);
	v3Up = glm::vec3(v4U);
	v3Position = glm::vec3(v3SpherePosition + v3Target);
    m_mat4View = glm::lookAt(
                             v3Position,
                             v3Target,
                             v3Up
                             );

}

void SciIllLib::CCamera::ShowInfo(){
	std::cout << " Status nformation" << std::endl;
	std::cout << "- Position: (" << v3Position.x << ", " << v3Position.y << ", " << v3Position.z << ")" << std::endl;
    std::cout << "- Target: (" << v3Target.x << ", " << v3Target.y << ", " << v3Target.z << ")" << std::endl;
    std::cout << "- Polar: (" << m_theta << ", " << m_phi << ", " << m_distance << ")" << std::endl;
}
