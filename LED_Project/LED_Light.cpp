#include "LED_Light.h"


void CLED_Light::light_forwardBack(float step)
{
	
	m_position = m_position + glm::vec3(0, step, 0);
}

void CLED_Light::light_upDown(float step)
{
	m_position = m_position + glm::vec3(0, 0, step);
}

void CLED_Light::light_rightLeft(float step)
{
	m_position = m_position + glm::vec3(step, 0, 0);
}