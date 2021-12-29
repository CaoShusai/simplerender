#include "PolygonalLight.h"
void PolygonalLight::Draw(Shader shader)
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
		core->glActiveTexture(GL_TEXTURE0);
	}
}


void PolygonalLight::loadModel(string path)
{
	//this->core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
	Assimp::Importer importer;
	uint32_t assimpFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
	//const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	//uint32_t assimpFlags = aiProcessPreset_TargetRealtime_MaxQuality |
	//	aiProcess_FlipUVs |
	//	0;
	////uint32_t assimpFlags = aiProcess_FlipUVs | 0;

	////assimpFlags &= ~(aiProcess_CalcTangentSpace); // Never use Assimp's tangent gen code
	//assimpFlags &= ~(aiProcess_FindDegenerates); // Avoid converting degenerated triangles to lines
	//assimpFlags &= ~(aiProcess_OptimizeGraph); // Never use as it doesn't handle transforms with negative determinants
	//if (is_set(builderFlags, SceneBuilder::Flags::DontMergeMeshes)) assimpFlags &= ~aiProcess_OptimizeMeshes; // Avoid merging original meshes


	const aiScene* scene = importer.ReadFile(path, assimpFlags);

	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		qDebug() << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
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
