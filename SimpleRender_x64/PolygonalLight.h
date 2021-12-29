#pragma once
#include"ObjectBase.h"
#include"light.h"
#include"Interface.h"
class PolygonalLight:public ObjectBase ,public light
{
public:
	virtual void Draw(Shader shader) override;
	virtual void loadModel(string path) override;
	//int lightNum;
	int currentLightIndex;
	PolygonalLight() {}
	PolygonalLight(QOpenGLFunctions_4_3_Core *core)
	{
		meshesNum = 0;
		verticesNum = 0;
		this->core =core;
	}
};

