#pragma once
#include<iostream>
using namespace std;
#include<vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<QOpenGLFunctions_4_3_Core>
#include<QOpenGLShader>
#include<QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include"shader.h"
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 color;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	
};
struct Texture {
	unsigned int id;
	string type;
	string path;
};
struct Material
{
	glm::vec3 albedo;
	glm::vec3 specular;
	glm::vec3 emissive;
	glm::vec3 roughness;
	float shininess;
	//glm::vec3 ;
};
struct Box
{
	float xmax, xmin, ymax, ymin, zmax, zmin;
	float widthx, widthy, widthz;
};
class mesh {
public:
	/*  网格数据  */
	//vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	Material material;
	long long numV;
	
	/*  函数  */
	
	mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures ,Box tmpBox,Material material,  QOpenGLFunctions_4_3_Core *core);
	~mesh()
	{
		//delete[] gZhgrad;
		//delete[] gShGrad;
		//delete[] Tcoeff;
		//delete[] vertexColor;
		//delete[] vertexPos;
		//delete[] vertexNormal;
		//delete[] vertexTexCoords;
		//delete[] test;
	}
	void Draw(Shader shader);
	void prtDraw(Shader shader);
	void preComputePerMesh();
	void pushVerticesColor();
	QOpenGLFunctions_4_3_Core *core;
	Box boundingBox;
	float* gZhgrad = nullptr;

	float* Tcoeff = nullptr;
	float* gShGrad = nullptr;
	float* gShTmpGrad = nullptr;
	float* vertexColor=nullptr;
	float* vertexPos = nullptr;
	float* vertexNormal = nullptr;
	float* vertexTexCoords = nullptr;
	float* lo = nullptr;
	float* test = nullptr;
	//float* 
	GLuint zhID;
	GLuint shID;
	GLuint shTmpID;
	GLuint verPosID;
	GLuint verColID;
	GLuint TcoeffID;
	GLuint verNorID;
	GLuint loID;
	GLuint testID;

	unsigned int VAO, VBO, EBO;
private:
	/*  渲染数据  */
	
	//QOpenGLContext* context;
	/*  函数  */
	void setupMesh();
};
