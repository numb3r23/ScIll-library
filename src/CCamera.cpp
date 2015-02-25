/*
 * CCamera.cpp
 *
 *  Created on: Jan 15, 2011
 *      Author: numb3r23
 */

#include "stdafx.h"

#include "CCamera.hpp"


SciIllLib::CCamera::CCamera()
{
	Reset();
}

void SciIllLib::CCamera::Reset()
{
	m_theta = 0.0f;
	m_phi = 0.0f;
	m_distance = 4.0f;
    
  m_fMouseX = 0.0f;
  m_fMouseY = 0.0f;

	m_v3Position = glm::vec3(0, 1, 5);
	m_v3SpherePosition = m_v3Position;

	m_v3Target = glm::vec3(0, 0, 0);
	m_v3Up = glm::vec3(0, 1, 0);

	Calculate();
}

glm::vec3& SciIllLib::CCamera::GetPosition()
{
	return m_v3Position;
}

void SciIllLib::CCamera::PosStore()
{
  m_v3PosStore = glm::vec3(m_theta, m_phi, m_distance);
}

void SciIllLib::CCamera::PosRecall()
{
  m_theta = m_v3PosStore.x;
  m_phi = m_v3PosStore.y;
  m_distance = m_v3PosStore.z;
  Calculate();
}

void SciIllLib::CCamera::Translate(const float x, const float y, const float z)
{
	m_v3Target += glm::vec3(x, y, z);
	m_v3Position = m_v3SpherePosition + m_v3Target;
}

void SciIllLib::CCamera::SetMousePosition(const int x, const int y)
{
  m_fMouseX = x;
  m_fMouseY = y;
}

void SciIllLib::CCamera::SetMouseMove(const int x, const int y)
{
  SetMouseRotationDelta(x - m_fMouseX, y - m_fMouseY);
  
  m_fMouseX = x;
  m_fMouseY = y;
}

void SciIllLib::CCamera::SetMouseRotationDelta(const float dx, const float dy)
{
	m_theta -= dx * 0.25f;
	m_phi += dy * 0.25f;

	Calculate();
}

void SciIllLib::CCamera::SetMouseRotation(const float dx, const float dy)
{
	m_theta = dx;
	m_phi = dy;

	Calculate();
}

void SciIllLib::CCamera::SetDistanceDelta(const float distance)
{
	m_distance += distance;

	Calculate();
}

void SciIllLib::CCamera::SetDistance(const float distance)
{
	m_distance = distance;

	Calculate();
}

void SciIllLib::CCamera::SetValuesDelta(const float theta, const float phi, const float distance)
{
	m_theta -= theta;
	m_phi += phi;
	m_distance += distance;

	Calculate();
}

float SciIllLib::CCamera::GetDistance() const
{
	return m_distance;
}

void SciIllLib::CCamera::SetValues(float theta, float phi, float distance)
{
	m_theta = theta;
	m_phi = phi;
	m_distance = distance;

	Calculate();
}

void SciIllLib::CCamera::SetViewMatrix(CGLShaderProgram& shader, const char* name)
{
	shader.setUniformValue(name, m_mat4View);
}

glm::mat4& SciIllLib::CCamera::GetViewMatrix(){
    return m_mat4View;
}

void SciIllLib::CCamera::SetViewPosition(CGLShaderProgram& shader, const char* name)
{
	shader.setUniformValue(name, m_v3Position.x, m_v3Position.y, m_v3Position.z);
}

void SciIllLib::CCamera::Calculate()
{
	glm::mat4 mat4 = glm::mat4(1.0f);
	mat4 = glm::rotate(mat4, m_theta, glm::vec3(0, 1, 0));
	mat4 = glm::rotate(mat4, m_phi, glm::vec3(1, 0, 0));

	glm::vec4 v4B = glm::vec4(0, 0, m_distance, 0);
	glm::vec4 v4U = glm::vec4(0, 1, 0, 0);
	v4B = mat4 * v4B;
	v4U = mat4 * v4U;

	m_v3SpherePosition = glm::vec3(v4B);
	m_v3Up = glm::vec3(v4U);
	m_v3Position = m_v3SpherePosition + m_v3Target;
  m_mat4View = glm::lookAt(m_v3Position,
                           m_v3Target,
                           m_v3Up);
}

void SciIllLib::CCamera::ShowInfo() const
{
	std::cout << " Status nformation" << std::endl;
	std::cout << "- Position: (" << m_v3Position.x << ", "
                               << m_v3Position.y << ", "
                               << m_v3Position.z << ")" << std::endl;
  std::cout << "- Target: (" << m_v3Target.x << ", "
                             << m_v3Target.y << ", "
                             << m_v3Target.z << ")" << std::endl;
  std::cout << "- Polar: (" << m_theta << ", " << m_phi << ", " << m_distance << ")" << std::endl;
}
