#include "model.h"
#include<QMessageBox>

void model::loadModel(string path)
{
	//this->core= QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
	Assimp::Importer importer;
	//const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	uint32_t assimpFlags = aiProcessPreset_TargetRealtime_MaxQuality |
		aiProcess_FlipUVs |
		0;
	//uint32_t assimpFlags = aiProcess_FlipUVs | 0;

	//assimpFlags &= ~(aiProcess_CalcTangentSpace); // Never use Assimp's tangent gen code
	assimpFlags &= ~(aiProcess_FindDegenerates); // Avoid converting degenerated triangles to lines
	assimpFlags &= ~(aiProcess_OptimizeGraph); // Never use as it doesn't handle transforms with negative determinants
	//if (is_set(builderFlags, SceneBuilder::Flags::DontMergeMeshes)) assimpFlags &= ~aiProcess_OptimizeMeshes; // Avoid merging original meshes

	
	const aiScene* scene = importer.ReadFile(path, assimpFlags);
	
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
	meshesNum = meshes.size();
	for (int i = 0; i < meshes.size(); i++)
	{
		verticesNum += meshes[i]->numV;
	}
	
}


//void model::loadLightModel(string path)
//{
//	this->core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
//	Assimp::Importer importer;
//	uint32_t assimpFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
//	//const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
//	//uint32_t assimpFlags = aiProcessPreset_TargetRealtime_MaxQuality |
//	//	aiProcess_FlipUVs |
//	//	0;
//	////uint32_t assimpFlags = aiProcess_FlipUVs | 0;
//
//	////assimpFlags &= ~(aiProcess_CalcTangentSpace); // Never use Assimp's tangent gen code
//	//assimpFlags &= ~(aiProcess_FindDegenerates); // Avoid converting degenerated triangles to lines
//	//assimpFlags &= ~(aiProcess_OptimizeGraph); // Never use as it doesn't handle transforms with negative determinants
//	//if (is_set(builderFlags, SceneBuilder::Flags::DontMergeMeshes)) assimpFlags &= ~aiProcess_OptimizeMeshes; // Avoid merging original meshes
//
//
//	const aiScene* scene = importer.ReadFile(path, assimpFlags);
//
//	// check for errors
//	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
//	{
//		qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
//		return;
//	}
//	// retrieve the directory path of the filepath
//	directory = path.substr(0, path.find_last_of('/'));
//
//	// process ASSIMP's root node recursively
//	processNode(scene->mRootNode, scene);
//	meshesNum = meshes.size();
//	for (int i = 0; i < meshes.size(); i++)
//	{
//		verticesNum += meshes[i]->numV;
//	}
//	
//}
//
//void model::processNode(aiNode *node, const aiScene *scene)
//{
//	for (unsigned int i = 0; i < node->mNumMeshes; i++)
//	{
//		//meshesNum += node->mNumMeshes;
//		// the node object only contains indices to index the actual objects in the scene. 
//		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//		meshes.push_back(processMesh(mesh, scene));
//	}
//	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
//	for (unsigned int i = 0; i < node->mNumChildren; i++)
//	{
//		processNode(node->mChildren[i], scene);
//	}
//}
//
//mesh model::processMesh(aiMesh *mesh, const aiScene *scene)
//{
//	aiMaterial *raw_material = scene->mMaterials[mesh->mMaterialIndex];
//
//	aiReturn ret;
//	aiColor3D materialAmbient(0.0f, 0.0f, 0.0f);
//	aiColor3D materialDiffuse(0.0f, 0.0f, 0.0f);
//	aiColor3D materialSpecular(0.0f, 0.0f, 0.0f);
//	aiColor3D materialEmissive(0.0f, 0.0f, 0.0f);
//	float shininess;
//
//	ret = raw_material->Get(AI_MATKEY_COLOR_AMBIENT, materialAmbient);
//	ret = raw_material->Get(AI_MATKEY_COLOR_DIFFUSE, materialDiffuse);
//	ret = raw_material->Get(AI_MATKEY_COLOR_SPECULAR, materialSpecular);
//	ret = raw_material->Get(AI_MATKEY_COLOR_EMISSIVE, materialEmissive);
//	ret = raw_material->Get(AI_MATKEY_SHININESS, shininess);
//
//	Material myMaterial;
//	myMaterial.albedo = glm::vec3(materialDiffuse.r, materialDiffuse.g, materialDiffuse.b);
//	myMaterial.specular = glm::vec3(materialSpecular.r, materialSpecular.g, materialSpecular.b);
//	myMaterial.emissive = glm::vec3(materialEmissive.r, materialEmissive.g, materialEmissive.b);
//	myMaterial.shininess = shininess;
//
//	vector<Vertex> vertices;
//	vector<unsigned int> indices;
//	vector<Texture> textures;
//
//	/*
//	box.bottom = 1e10;
//	box.left = 1e10;
//	box.right = -1e10;
//	box.top = -1e10;*/
//	float xMax = -1e10;
//	float xMin = 1e10;
//	float yMax = -1e10;
//	float yMin = 1e10;
//	float zMax = -1e10;
//	float zMin = 1e10;
//	// walk through each of the mesh's vertices
//	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
//	{
//		Vertex vertex;
//		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
//		// positions
//		vector.x = mesh->mVertices[i].x;
//		vector.y = mesh->mVertices[i].y;
//		vector.z = mesh->mVertices[i].z;
//		xMax = max(vector.x, xMax);
//		yMax = max(vector.y, yMax);
//		zMax = max(vector.z, zMax);
//		xMin = min(vector.x, xMin);
//		yMin = min(vector.y, yMin);
//		zMin = min(vector.z, zMin);
//
//		vertex.Position = vector;
//		// normals
//		if (mesh->HasNormals())
//		{
//			vector.x = mesh->mNormals[i].x;
//			vector.y = mesh->mNormals[i].y;
//			vector.z = mesh->mNormals[i].z;
//			vertex.Normal = vector;
//		}
//		// texture coordinates
//		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
//		{
//			glm::vec2 vec;
//			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
//			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
//			vec.x = mesh->mTextureCoords[0][i].x;
//			vec.y = mesh->mTextureCoords[0][i].y;
//			vertex.TexCoords = vec;
//			// tangent
//			vector.x = mesh->mTangents[i].x;
//			vector.y = mesh->mTangents[i].y;
//			vector.z = mesh->mTangents[i].z;
//			vertex.Tangent = vector;
//			// bitangent
//			vector.x = mesh->mBitangents[i].x;
//			vector.y = mesh->mBitangents[i].y;
//			vector.z = mesh->mBitangents[i].z;
//			vertex.Bitangent = vector;
//		}
//		else
//			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
//
//		vertices.push_back(vertex);
//	}
//	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
//	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
//	{
//		aiFace face = mesh->mFaces[i];
//		// retrieve all indices of the face and store them in the indices vector
//		for (unsigned int j = 0; j < face.mNumIndices; j++)
//			indices.push_back(face.mIndices[j]);
//	}
//	// process materials
//	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
//	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
//	// Same applies to other texture as the following list summarizes:
//	// diffuse: texture_diffuseN
//	// specular: texture_specularN
//	// normal: texture_normalN
//
//	// 1. diffuse maps
//	vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//	// 2. specular maps
//	vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//	// 3. normal maps
//	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
//	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
//	// 4. height maps
//	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
//	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
//
//	// return a mesh object created from the extracted mesh data
//
//	
//	Box tmpBox;
//	tmpBox.xmax = xMax + 0.5;
//	tmpBox.ymax = yMax + 0.5;
//	tmpBox.zmax = zMax + 0.5;
//	tmpBox.xmin = xMin - 0.5;
//	tmpBox.ymin = yMin - 0.5;
//	tmpBox.zmin = zMin - 0.5;
//	tmpBox.widthx = xMax - xMin + 1.0;
//	tmpBox.widthy = yMax - yMin + 1.0;
//	tmpBox.widthz = zMax - zMin + 1.0;
//
//	/*tmpBox.xmax = xMax - 0.2;
//	tmpBox.ymax = yMax - 0.2;
//	tmpBox.zmax = zMax - 0.2;
//	tmpBox.xmin = xMin + 0.2;
//	tmpBox.ymin = yMin + 0.2;
//	tmpBox.zmin = zMin + 0.2;
//	tmpBox.widthx = xMax - xMin - 0.4;
//	tmpBox.widthy = yMax - yMin - 0.4;
//	tmpBox.widthz = zMax - zMin - 0.4;*/
//	return mesh::mesh(vertices, indices, textures,tmpBox, myMaterial);
//}
//
//vector<Texture> model::loadMaterialTextures(aiMaterial *mat, aiTextureType type,string typeName)
//{
//	vector<Texture> textures;
//	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
//	{
//		aiString str;
//		mat->GetTexture(type, i, &str);
//		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
//		bool skip = false;
//		for (unsigned int j = 0; j < textures_loaded.size(); j++)
//		{
//			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
//			{
//				textures.push_back(textures_loaded[j]);
//				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
//				break;
//			}
//		}
//		if (!skip)
//		{   // if texture hasn't been loaded already, load it
//			Texture texture;
//			texture.id = TextureFromFile(str.C_Str(), this->directory);
//			texture.type = typeName;
//			texture.path = str.C_Str();
//			textures.push_back(texture);
//			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
//		}
//	}
//	return textures;
//}


void model::Draw(Shader shader)
{
	for (int i = meshes.size() - 1; i >= 0; i--)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < meshes[i]->textures.size(); i++)
		{
			core->glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			string number;
			string name = meshes[i]->textures[i].type;
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
			core->glBindTexture(GL_TEXTURE_2D, meshes[i]->textures[i].id);
		}


		// draw mesh
		shader.use();

		shader.setVec3("emissive", meshes[i]->material.emissive);
		shader.setVec3("albedo", meshes[i]->material.albedo);
		shader.setVec3("specular", meshes[i]->material.specular);
		core->glBindVertexArray(meshes[i]->VAO);
		core->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->EBO);
		//core->glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//core->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPos[0])*numV * 3, &vertexPos[0]);

		core->glDrawElements(GL_TRIANGLES, meshes[i]->indices.size(), GL_UNSIGNED_INT, 0);
		//core->glBindBuffer(GL_ARRAY_BUFFER, 0);
		core->glBindVertexArray(0);

		// always good practice to set everything back to defaults once configured.
		//core->glActiveTexture(GL_TEXTURE0);
	}
		
}



//void model::prtDraw(Shader shader)
//{
//	for (int i = meshes.size() - 1; i >= 0; i--)
//		meshes[i]->prtDraw(shader);
//}
//
//unsigned int model::TextureFromFile(const char *path, const string &directory)
//{
//	string filename = string(path);
//	filename = directory + '/' + filename;
//
//	unsigned int textureID;
//	core->glGenTextures(1, &textureID);
//	
//	int width, height, nrComponents;
//	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
//	if (data)
//	{
//		GLenum format;
//		if (nrComponents == 1)
//			format = GL_RED;
//		else if (nrComponents == 3)
//			format = GL_RGB;
//		else if (nrComponents == 4)
//			format = GL_RGBA;
//
//		core->glBindTexture(GL_TEXTURE_2D, textureID);
//		core->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		core->glGenerateMipmap(GL_TEXTURE_2D);
//
//		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		stbi_image_free(data);
//	}
//	else
//	{
//		std::cout << "Texture failed to load at path: " << path << std::endl;
//		stbi_image_free(data);
//	}
//
//	return textureID;
//}

