#include "mesh.h"
Mesh:: Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> texture,Box tmpBox, Material material, QOpenGLFunctions_4_3_Core *core)
{
	this->numV = vertices.size();
	this->material = material;
	vertexColor = new float[numV * 3];
	vertexPos = new float[numV * 3];
	vertexNormal = new float[numV * 3];
	vertexTexCoords = new float[numV * 2];
	for (int i = 0; i < numV; i++)
	{
		vertexs.push_back(glm::vec3(vertices[i].Position[0],
			vertices[i].Position[1], vertices[i].Position[2]));
		vertexPos[i * 3] = vertices[i].Position[0];
		vertexPos[i * 3+1] = vertices[i].Position[1];
		vertexPos[i * 3+2] = vertices[i].Position[2];
		vertexNormal[i * 3] = vertices[i].Normal[0];
		vertexNormal[i * 3+1] = vertices[i].Normal[1];
		vertexNormal[i * 3+2] = vertices[i].Normal[2];
		vertexTexCoords[i * 2] = vertices[i].TexCoords[0];
		vertexTexCoords[i * 2+1] = vertices[i].TexCoords[1];
		/*vertexColor[i * 3] = vertices[i].color[0];
		vertexColor[i * 3+1] = vertices[i].color[1];
		vertexColor[i * 3+2] = vertices[i].color[2];*/
	}
	//this->vertices = vertices;
	this->indices = indices;
	if(textures.size()!=0)
	  this->textures = textures;
	this->boundingBox = tmpBox;
	this->core =core;
	
	for (int i = 0; i < numV; i++)
	{
		//this->vertices[i].color = glm::vec3(0.5,0.2,0.2);
	}
	for (int i = 0; i < numV*3; i++)
	{
		vertexColor[i] = 0.5;
	}
	/*Vertex v;
	v.Position = glm::vec3(0.5f, 0.5f, 0.0f);
	this->vertices.push_back(v);
	v.Position = glm::vec3(0.5f, -0.5f, 0.0f);
	this->vertices.push_back(v);
	v.Position = glm::vec3(-0.5f, -0.5f, 0.0f);
	this->vertices.push_back(v);
	v.Position = glm::vec3(-0.5f, 0.5f, 0.0f);
	this->vertices.push_back(v);

	this->indices.push_back(0);
	this->indices.push_back(1);
	this->indices.push_back(3);
	this->indices.push_back(1);
	this->indices.push_back(2);
	this->indices.push_back(3);*/

	// now that we have all the required data, set the vertex buffers and its attribute pointers.
	setupMesh();
}


void  Mesh::prtDraw(Shader shader)
{
	//unsigned int diffuseNr = 1;
	//unsigned int specularNr = 1;
	//unsigned int normalNr = 1;
	//unsigned int heightNr = 1;
	//for (unsigned int i = 0; i < textures.size(); i++)
	//{
	//	core->glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
	//	// retrieve texture number (the N in diffuse_textureN)
	//	string number;
	//	string name = textures[i].type;
	//	if (name == "texture_diffuse")
	//		number = std::to_string(diffuseNr++);
	//	else if (name == "texture_specular")
	//		number = std::to_string(specularNr++); // transfer unsigned int to stream
	//	else if (name == "texture_normal")
	//		number = std::to_string(normalNr++); // transfer unsigned int to stream
	//	else if (name == "texture_height")
	//		number = std::to_string(heightNr++); // transfer unsigned int to stream

	//	// now set the sampler to the correct texture unit
	//	//core->glUniform1i(shader.uniformLocation((name + number).c_str()), i);
	//	core->glUniform1i(core->glGetUniformLocation(shader.ID, (name + number).c_str()), i);
	//	// and finally bind the texture
	//	core->glBindTexture(GL_TEXTURE_2D, textures[i].id);
	//}


	// draw mesh
	shader.use();
	core->glBindVertexArray(VAO);
	core->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	core->glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	core->glActiveTexture(GL_TEXTURE0);
	//shader.release();
}

void  Mesh::Draw(Shader shader)
{
	
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		core->glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

		// now set the sampler to the correct texture unit
		//core->glUniform1i(shader.uniformLocation((name + number).c_str()), i);
		core->glUniform1i(core->glGetUniformLocation(shader.ID, (name + number).c_str()), i);
		// and finally bind the texture
		core->glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	
	// draw mesh
	shader.use();
	shader.setVec3("emissive",this->material.emissive);
	shader.setVec3("albedo", this->material.albedo);
	shader.setVec3("specular", this->material.specular);
	core->glBindVertexArray(VAO);
	core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//core->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPos[0])*numV * 3, &vertexPos[0]);

	core->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//core->glBindBuffer(GL_ARRAY_BUFFER, 0);
	core->glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	core->glActiveTexture(GL_TEXTURE0);
	//shader.release();
	
}

void  Mesh::setupMesh()
{
	core->glGenVertexArrays(1, &VAO);
	core->glGenBuffers(1, &VBO);
	core->glGenBuffers(1, &EBO);

	core->glBindVertexArray(VAO);

	// load data into vertex buffers

	core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	

	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	GLuint sizeT = sizeof(vertexColor[0]) * 11 * numV;
	core->glBufferData(GL_ARRAY_BUFFER, sizeT, NULL, GL_DYNAMIC_DRAW);
	/*core->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex)+numV* sizeof(vertexColor[0])*3, &vertices[0], GL_DYNAMIC_DRAW);
	core->glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), sizeof(vertexColor[0])*numV*3,&vertexColor[0]);*/

	core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	core->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	core->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPos[0])*numV * 3, &vertexPos[0]);
	core->glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPos[0])*numV * 3, sizeof(vertexNormal[0])*numV * 3, &vertexNormal[0]);
	core->glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPos[0])*numV * 6, sizeof(vertexTexCoords[0])*numV * 2, &vertexTexCoords[0]);
	core->glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPos[0])*numV * 8, sizeof(vertexColor[0])*numV * 3, &vertexColor[0]);

	core->glEnableVertexAttribArray(0);
	core->glEnableVertexAttribArray(1);
	core->glEnableVertexAttribArray(2);
	core->glEnableVertexAttribArray(3);
	core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(vertexPos[0])*numV * 3));
	core->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(vertexPos[0])*numV * 6));
	core->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(vertexPos[0])*numV * 8));
	//// set the vertex attribute pointers
	//// vertex Positions
	//core->glEnableVertexAttribArray(0);
	//core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//// vertex normals
	//core->glEnableVertexAttribArray(1);
	//core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//// vertex texture coords
	//core->glEnableVertexAttribArray(2);
	//core->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	//core->glEnableVertexAttribArray(3);
	////core->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	//core->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3, (void*)(vertices.size() * sizeof(Vertex)));

	//// vertex tangent
	//core->glEnableVertexAttribArray(4);
	//core->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	//// vertex bitangent
	//core->glEnableVertexAttribArray(5);
	//core->glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	core->glBindVertexArray(0);
	core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	core->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void  Mesh::preComputePerMesh()
{
	this->gZhgrad = new float[1836 * 512];
	for (int i = 0; i < 512*1836; i++)
	{
		gZhgrad[i] = 0;
	}
	//gShGrad = new float[3 * 81 * 512 * 4];

	this->gShGrad = new float[243*4 * 512];
	for (int i = 0; i < 243*4*512; i++)
	{
		gShGrad[i] = 1;
	}

	this->gShTmpGrad = new float[243 * 4 * 512];
	for (int i = 0; i < 243 * 4 * 512; i++)
	{
		gShTmpGrad[i] = 0.0f;
	}

	Tcoeff = new float[81 * numV];
	for (int i = 0; i < 81*numV; i++)
	{
		Tcoeff[0] = 0.0f;
	}
	lo = new float[numV * 75];
}
bool xcmp(Triangle* t1, Triangle* t2) {
	return t1->center.x < t2->center.x;
}
bool ycmp(Triangle* t1, Triangle* t2) {
	return t1->center.y < t2->center.y;
}
bool zcmp(Triangle* t1, Triangle* t2) {
	return t1->center.z < t2->center.z;
}
bool cmp(BVHNode* n1, BVHNode* n2) {
	return n1->idx < n2->idx;
}
void  Mesh::sortBVH() {
	sort(bvh.begin(), bvh.end(), cmp);
}

BVHNode*  Mesh::buildBVH(int l, int r, int n) {
	if (l > r) return NULL;
	BVHNode* node = new BVHNode();
	node->AA = glm::vec3(100000000.f);
	node->BB = glm::vec3(-100000000.f);


	for (int i = l; i <= r; i++) {
		// 最小点 AA
		float minx = std::min(triangles[i]->p1.x, std::min(triangles[i]->p2.x, triangles[i]->p3.x));
		float miny = std::min(triangles[i]->p1.y, std::min(triangles[i]->p2.y, triangles[i]->p3.y));
		float minz = std::min(triangles[i]->p1.z, std::min(triangles[i]->p2.z, triangles[i]->p3.z));
		node->AA.x = std::min(node->AA.x, minx);
		node->AA.y = std::min(node->AA.y, miny);
		node->AA.z = std::min(node->AA.z, minz);
		// 最大点 BB
		float maxx = std::max(triangles[i]->p1.x, std::max(triangles[i]->p2.x, triangles[i]->p3.x));
		float maxy = std::max(triangles[i]->p1.y, std::max(triangles[i]->p2.y, triangles[i]->p3.y));
		float maxz = std::max(triangles[i]->p1.z, std::max(triangles[i]->p2.z, triangles[i]->p3.z));
		node->BB.x = std::max(node->BB.x, maxx);
		node->BB.y = std::max(node->BB.y, maxy);
		node->BB.z = std::max(node->BB.z, maxz);
	}

	//不多于n个三角形 返回叶子节点
	if ((r - l + 1) <= n) {
		node->index = l;
		node->leaf = 1;
		node->n = r - l + 1;
		node->idx = bvh.size();
		bvh.push_back(node);
		return node;
	}
	node->leaf = 0;
	//否则递归建树
	float lenx = node->BB.x - node->AA.x;
	float leny = node->BB.y - node->AA.y;
	float lenz = node->BB.z - node->AA.z;

	// 按 x 划分
	if (lenx >= leny && lenx >= lenz)
		sort(triangles.begin() + l, triangles.begin() + r + 1, xcmp);
	// 按 y 划分
	else if (leny >= lenx && leny >= lenz)
		sort(triangles.begin() + l, triangles.begin() + r + 1, ycmp);
	// 按 z 划分
	else if (lenz >= lenx && lenz >= leny)
		sort(triangles.begin() + l, triangles.begin() + r + 1, zcmp);

	//递归
	int mid = (l + r) / 2;
	node->left = buildBVH(l, mid, n);
	node->right = buildBVH(mid + 1, r, n);

	node->idx = bvh.size();
	bvh.push_back(node);
	return node;
}


void  Mesh::pushVerticesColor()
{


	core->glBindVertexArray(VAO);

	core->glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	core->glBindBuffer(GL_ARRAY_BUFFER, verColID);
	core->glEnableVertexAttribArray(4);
	core->glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/*core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	core->glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPos[0])*numV * 8, sizeof(vertexColor[0])*numV * 3, &vertexColor[0]);*/
	core->glBindBuffer(GL_ARRAY_BUFFER, 0);
	core->glBindVertexArray(0);
}

