#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"ObjectBase.h"
#include<map>
#include"shader.h"
class SceneBase
{
protected:
	map<int,unique_ptr<ObjectBase>> objects;//��ͨ�����
	int objectsNum;
public:
	//��Ӷ���
	virtual void AddObject(glm::vec3 pos, glm::vec3 rotate, glm::vec3 scale, string path, unique_ptr<ObjectBase> object_ptr) {};//�������
	virtual void RemoveObject(int objectId) = 0;
	virtual void Close() = 0;
	virtual void Change() = 0;
	virtual void Save()= 0;
	virtual void Load()= 0;
	virtual ~SceneBase() {};
};

