#pragma once
#include "ObjectBase.h"
class planeRect :
	public ObjectBase
{
public:
	void createInstance(glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale, QOpenGLFunctions_4_3_Core *core);
	virtual void Draw(Shader shader) override;

};

