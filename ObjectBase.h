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
class ObjectBase
{
public:
	int meshesNum;
	int verticesNum;
	/*  函数   */

	
	virtual void Draw(Shader shader)=0;
	virtual void loadModel(string path)=0;
	unsigned int TextureFromFile(const char *path, const string &directory);
	vector<mesh> meshes;
protected:
	/*  模型数据  */
	//QOpenGLShaderProgram shader;
	QOpenGLFunctions_4_3_Core *core;
	vector<Texture> textures_loaded;

	string directory;
	
	//QOpenGLContext* context;
	/*  函数   */

	
	
	void processNode(aiNode *node, const aiScene *scene);
	mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

	glm::vec3 translate;
	glm::vec3 rotate;
	glm::vec3 scale;
	glm::mat4x4 Transform;

};

