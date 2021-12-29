#pragma once
//系统内置形状和导入模型这一层需要后面抽象出来，这里我先不改了，后面需求大再改

#include<mesh.h>
#include"ObjectBase.h"
class CubeMesh:public ObjectBase
{
public:
	void createInstance(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale, QOpenGLFunctions_4_3_Core *core);
	
	virtual void Draw(Shader shader) override;
	//virtual void loadModel(string path) override;
};

