#pragma once
//ϵͳ������״�͵���ģ����һ����Ҫ�������������������Ȳ����ˣ�����������ٸ�

#include<mesh.h>
#include"ObjectBase.h"
class CubeMesh:public ObjectBase
{
public:
	void createInstance(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale, QOpenGLFunctions_4_3_Core *core);
	
	virtual void Draw(Shader shader) override;
	//virtual void loadModel(string path) override;
};

