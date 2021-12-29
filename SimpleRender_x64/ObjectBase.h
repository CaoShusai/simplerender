#pragma once
#include"mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include<QOpenGLFunctions_4_3_Core>
#include<QOpenGLShader>
#include<QOpenGLShaderProgram>
#include"stb_image.h"
#include"shader.h"
#include<cmath>
#include<stdio.h>

//�ֽ׶Σ�objectbase�����������
class ObjectBase
{
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;


	int meshesNum;
	int verticesNum;
	/*  ����   */
	int objectId;
	
	virtual void Draw(Shader shader)=0;
	virtual void loadModel(string path)=0;
	unsigned int TextureFromFile(const char *path, const string &directory);
	//vector<mesh> meshes;
	vector<shared_ptr<Mesh>> meshes;
protected:
	/*  ģ������  */
	//QOpenGLShaderProgram shader;
	QOpenGLFunctions_4_3_Core *core;
	vector<Texture> textures_loaded;

	string directory;
	
	//QOpenGLContext* context;
	/*  ����   */

	
	
	void processNode(aiNode *node, const aiScene *scene);
	unique_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

};

