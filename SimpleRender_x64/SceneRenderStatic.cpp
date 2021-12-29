#include "SceneRenderStatic.h"

//现在只支持一个场景一个shader
void SceneRenderStatic::Draw(Shader shader)
{
	for (auto iter = this->objects.begin(); iter != objects.end(); iter++)
	{
		iter->second->Draw(shader);
	}
}

//这个接口以后要改成没有这些位置旋转尺寸的，这些让用户自己点击去调整
void SceneRenderStatic::AddObject(glm::vec3 pos, glm::vec3 rotate, glm::vec3 scale, string path,unique_ptr<ObjectBase> object_ptr)
{

	//目前只支持model级别的旋转平移
	object_ptr->loadModel(path);
	object_ptr->position = pos;
	object_ptr->rotation = rotate;
	object_ptr->scale = scale;

	this->objectsNum = this->objects.size();//末尾加一个
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