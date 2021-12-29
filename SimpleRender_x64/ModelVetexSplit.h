#pragma once
#include"ObjectBase.h"
class  ModelVetexSplit :public ObjectBase 
{
public:
	virtual void Draw(Shader shader) override;
	virtual void loadModel(string path) override;
	//int lightNum;
	int currentLightIndex;
	ModelVetexSplit() {}
	ModelVetexSplit(QOpenGLFunctions_4_3_Core *core)
	{
		meshesNum = 0;
		verticesNum = 0;
		this->core =core;
	}
};

