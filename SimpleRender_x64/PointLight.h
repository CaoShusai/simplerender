#pragma once
#include "light.h"
struct pointLight
{
	glm::vec3 position;
	glm::vec3 emissive;
};
class PointLight :
	public light
{
	QOpenGLFunctions_4_3_Core *core;
	PointLight() {}
	PointLight(QOpenGLFunctions_4_3_Core *core)
	{
		this->core = core;
		lightNum = 0;
	}
	void addPointLight(glm::vec3 position,glm::vec3 emissive )
	{
		pointLight light;
		light.emissive = emissive;
		light.position = position;
		lightNum++;
	}
	void Draw()
	{

	}
	int lightNum;
	vector<pointLight> pointLights;

};

