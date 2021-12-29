#pragma once
#include<iostream>
using namespace std;
#include<vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"model.h"
#include"ObjectBase.h"
#define POINT_LIGHT 0
#define POLYGONAL_LIGHT 1
struct directionalLight
{
	glm::vec3 direction;
};

struct spotLight
{
	glm::vec3  position;
	glm::vec3  direction;
	float cutOff;
};

class light
{
public:
	//vector<directionalLight> directionalLights;
	//vector<pointLight> pointLights;
	//vector<spotLight>spotLights;
	
	//model polygonalLight;
	int LightType;
	/*light(int flag,QString path)
	{
		LightType = flag;
		if (LightType== POINT_LIGHT)
		{

		}
		else if (LightType== POLYGONAL_LIGHT)
		{
			this->lightLoadFile(path);
			lightNum = this->polygonalLight.verticesNum / 3;
		}
	};*/
	light()
	{

	};
	void lightLoadFile(QString path);
	//QOpenGLFunctions_4_3_Core *core;
	/*int lightNum;
	int currentLightIndex;*/
	
};

