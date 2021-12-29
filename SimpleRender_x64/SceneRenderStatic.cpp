#include "SceneRenderStatic.h"

//����ֻ֧��һ������һ��shader
void SceneRenderStatic::Draw(Shader shader)
{
	for (auto iter = this->objects.begin(); iter != objects.end(); iter++)
	{
		iter->second->Draw(shader);
	}
}

//����ӿ��Ժ�Ҫ�ĳ�û����Щλ����ת�ߴ�ģ���Щ���û��Լ����ȥ����
void SceneRenderStatic::AddObject(glm::vec3 pos, glm::vec3 rotate, glm::vec3 scale, string path,unique_ptr<ObjectBase> object_ptr)
{

	//Ŀǰֻ֧��model�������תƽ��
	object_ptr->loadModel(path);
	object_ptr->position = pos;
	object_ptr->rotation = rotate;
	object_ptr->scale = scale;

	this->objectsNum = this->objects.size();//ĩβ��һ��
	this->objects.insert(pair<int, std::unique_ptr<ObjectBase>>(objectsNum,std::move(object_ptr)));
	
}


void SceneRenderStatic::RemoveObject(int objectId)
{

}

void SceneRenderStatic::Close()
{

}

void SceneRenderStatic::Change()
{

}

void SceneRenderStatic::Save()
{


}

void SceneRenderStatic::Load()
{

}