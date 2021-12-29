#pragma once
#include "SceneBase.h"
#include"Camera.h"
#include"model.h"

class SceneRenderStatic :
	public SceneBase
{
public:

	virtual void Draw(Shader shader);

	virtual void AddObject(glm::vec3 pos, glm::vec3 rotate, glm::vec3 scale,  string path, unique_ptr<ObjectBase> object_ptr);//ÃÌº”ŒÔÃÂ
	virtual void RemoveObject(int objectId);
	virtual void Close();
	virtual void Change();
	virtual void Save();
	virtual void Load();
	virtual ~SceneRenderStatic() {};
};

