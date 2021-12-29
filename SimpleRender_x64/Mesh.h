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

struct Triangle {
	glm::vec3 p1, p2, p3;   // 三点
	glm::vec3 center;       // 中心
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
		p1 = a, p2 = b, p3 = c;
		center = (p1 + p2 + p3) / 3.f;
	}
};
struct BVHNode {
	BVHNode* left = NULL;	//左右子树指针
	BVHNode* right = NULL;
	int idx;				//节点编号
	int leaf = 0;
	int n = -1, index = -1;			//叶子节点信息 n:三角形个数 index:三角形索引
	glm::vec3 AA, BB;		//aabb
};

class Mesh {
public:
	/*  网格数据  */
	//vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	Material material;
	long long numV;
	
	
	//禁用拷贝构造
	Mesh(const  Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;


	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures ,Box tmpBox,Material material,  QOpenGLFunctions_4_3_Core *core);
	~Mesh()
	{
		if (gZhgrad!=nullptr)
		    delete[] gZhgrad;
		if (gShGrad != nullptr)
		   delete[] gShGrad;
		if (Tcoeff != nullptr)
		    delete[] Tcoeff;
		if (vertexColor != nullptr)
		    delete[] vertexColor;
		if (vertexPos != nullptr)
		    delete[] vertexPos;
		if (vertexNormal != nullptr)
		    delete[] vertexNormal;
		if (vertexTexCoords != nullptr)
		    delete[] vertexTexCoords;
		if (test != nullptr)
		    delete[] test;
		if (lo != nullptr)
		    delete[] lo;
		if (gShTmpGrad != nullptr)
		    delete[] gShTmpGrad;
	}
	void Draw(Shader shader);
	void prtDraw(Shader shader);
	void preComputePerMesh();
	void pushVerticesColor();
	vector<glm::vec3> vertexs;
	vector<BVHNode*> bvh;
	vector<Triangle*> triangles;
	BVHNode* root;// = buildBVH(triangles, 0, triangles.size() - 1, 8, 0);
	void sortBVH();
	BVHNode* buildBVH(int l, int r, int n);

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
