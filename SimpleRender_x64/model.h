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
#include"ObjectBase.h"
struct structTextureParameters {
	GLenum texTarget;
	GLenum texInternalFormat;
	GLenum texFormat;
	char *shader_source;
};
class model:public ObjectBase
{
public:
	

	model(QOpenGLFunctions_4_3_Core *core)
	{
		meshesNum = 0;
		verticesNum = 0;
		this->core = core;
		//this->core = core;
		//this->context = context;
		//loadModel(path);

	}
	model()
	{

	}
	virtual void Draw(Shader shader) override;
	virtual void loadModel(string path) override;
	/*unsigned int TextureFromFile(const char *path, const string &directory);
	vector<mesh> meshes;*/
private:
	/*  模型数据  */
	//QOpenGLShaderProgram shader;
	//QOpenGLFunctions_4_3_Core *core;
	//vector<Texture> textures_loaded;
	//
	//string directory;
	//unsigned int VAO, VBO, EBO;
	////QOpenGLContext* context;
	///*  函数   */
	//
	//
	////void loadLightModel(string path);
	//void processNode(aiNode *node, const aiScene *scene);
	//mesh processMesh(aiMesh *mesh, const aiScene *scene);
	//vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,string typeName);
	
public slots:
	void actionOpenMesh();
};
