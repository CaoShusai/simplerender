#include "Prt_Zh.h"
void Prt_Zh::computeT(model& model, ModelVetexSplit& modelSplit, Shader TcoeffShader)
{
	TcoeffShader.use();
	int bvhNum = 0;
	int triNum = 0;
	int* rootBVHIndex = new int[model.meshesNum];
	int* firstTriIndex = new int[model.meshesNum];

	for (int i = 0; i < modelSplit.meshesNum; i++)
	{
		for (int j = 0; j < modelSplit.meshes[i]->numV; j += 3)
		{
			Triangle *t = new Triangle(modelSplit.meshes[i]->vertexs[j], modelSplit.meshes[i]->vertexs[j + 1], modelSplit.meshes[i]->vertexs[j + 2]);
			modelSplit.meshes[i]->triangles.push_back(t);
		}
		modelSplit.meshes[i]->root = modelSplit.meshes[i]->buildBVH(0, modelSplit.meshes[i]->triangles.size() - 1, 1);
		reverse(modelSplit.meshes[i]->bvh.begin(), modelSplit.meshes[i]->bvh.end());
		for (int j = 0; j < modelSplit.meshes[i]->bvh.size(); j++) {
			modelSplit.meshes[i]->bvh[j]->idx = j;
		}

		rootBVHIndex[i] = bvhNum;
		firstTriIndex[i] = triNum;
		bvhNum += modelSplit.meshes[i]->bvh.size();
		triNum += modelSplit.meshes[i]->triangles.size();
	}
	float* pmin = new float[bvhNum * 3];
	float* pmax = new float[bvhNum * 3];
	int* left = new int[bvhNum];
	int* right = new int[bvhNum];
	int* num = new int[bvhNum];
	int* triIdx = new int[bvhNum];
	float* p1 = new float[triNum * 3];
	float* p2 = new float[triNum * 3];
	float* p3 = new float[triNum * 3];

	for (int i = 0; i < modelSplit.meshesNum; i++)
	{
		for (int j = 0; j < modelSplit.meshes[i]->bvh.size(); j++)
		{
			int ID = rootBVHIndex[i] + j;
			for (int k = 0; k < 3; k++)
			{
				pmin[ID * 3 + k] = modelSplit.meshes[i]->bvh[j]->AA[k];
				pmax[ID * 3 + k] = modelSplit.meshes[i]->bvh[j]->BB[k];
			}
			if (modelSplit.meshes[i]->bvh[j]->left != NULL)
			{
				left[ID] = modelSplit.meshes[i]->bvh[j]->left->idx + rootBVHIndex[i];
				right[ID] = modelSplit.meshes[i]->bvh[j]->right->idx + rootBVHIndex[i];
				triIdx[ID] = -1;
			}
			else
			{
				left[ID] = -1;
				right[ID] = -1;
				triIdx[ID] = modelSplit.meshes[i]->bvh[j]->index + firstTriIndex[i];
				num[ID] = modelSplit.meshes[i]->bvh[j]->n;
			}
		}

		for (int j = 0; j < modelSplit.meshes[i]->triangles.size(); j++)
		{
			int ID = firstTriIndex[i] + j;
			for (int k = 0; k < 3; k++)
			{
				p1[ID * 3 + k] = modelSplit.meshes[i]->triangles[j]->p1[k];
				p2[ID * 3 + k] = modelSplit.meshes[i]->triangles[j]->p2[k];
				p3[ID * 3 + k] = modelSplit.meshes[i]->triangles[j]->p3[k];
			}
		}
	}
	GLuint* pminID = new GLuint;
	core->glGenBuffers(1, pminID);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, *pminID);
	core->glBufferData(GL_SHADER_STORAGE_BUFFER, bvhNum * 3 * sizeof(float), pmin, GL_DYNAMIC_DRAW);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	GLuint* pmaxID = new GLuint;
	core->glGenBuffers(1, pmaxID);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, *pmaxID);
	core->glBufferData(GL_SHADER_STORAGE_BUFFER, bvhNum * 3 * sizeof(float), pmax, GL_DYNAMIC_DRAW);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	GLuint* leftID = new GLuint;
	core->glGenBuffers(1, leftID);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, *leftID);
	core->glBufferData(GL_SHADER_STORAGE_BUFFER, bvhNum * sizeof(int), left, GL_DYNAMIC_DRAW);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	GLuint* rightID = new GLuint;
	core->glGenBuffers(1, rightID);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, *rightID);
	core->glBufferData(GL_SHADER_STORAGE_BUFFER, bvhNum * sizeof(int), right, GL_DYNAMIC_DRAW);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	

	GLuint* triID = new GLuint;
	core->glGenBuffers(1, triID);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, *triID);
	core->glBufferData(GL_SHADER_STORAGE_BUFFER, bvhNum * sizeof(int), triIdx, GL_DYNAMIC_DRAW);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	GLuint* p1ID = new GLuint;
	core->glGenBuffers(1, p1ID);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, *p1ID);
	core->glBufferData(GL_SHADER_STORAGE_BUFFER, triNum * 3 * sizeof(float), p1, GL_DYNAMIC_DRAW);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	GLuint* p2ID = new GLuint;
	core->glGenBuffers(1, p2ID);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, *p2ID);
	core->glBufferData(GL_SHADER_STORAGE_BUFFER, triNum * 3 * sizeof(float), p2, GL_DYNAMIC_DRAW);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	GLuint* p3ID = new GLuint;
	core->glGenBuffers(1, p3ID);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, *p3ID);
	core->glBufferData(GL_SHADER_STORAGE_BUFFER, triNum * 3 * sizeof(float), p3, GL_DYNAMIC_DRAW);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	int ssbo_binding = 3;
	int block_index = core->glGetProgramResourceIndex(TcoeffShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferPmin");
	core->glShaderStorageBlockBinding(TcoeffShader.ID, block_index, ssbo_binding);
	core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, *pminID);

	ssbo_binding = 4;
	block_index = core->glGetProgramResourceIndex(TcoeffShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferPmax");
	core->glShaderStorageBlockBinding(TcoeffShader.ID, block_index, ssbo_binding);
	core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, *pmaxID);

	ssbo_binding = 5;
	block_index = core->glGetProgramResourceIndex(TcoeffShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferLeft");
	core->glShaderStorageBlockBinding(TcoeffShader.ID, block_index, ssbo_binding);
	core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, *leftID);

	ssbo_binding = 6;
	block_index = core->glGetProgramResourceIndex(TcoeffShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferRight");
	core->glShaderStorageBlockBinding(TcoeffShader.ID, block_index, ssbo_binding);
	core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, *rightID);

	

	ssbo_binding = 8;
	block_index = core->glGetProgramResourceIndex(TcoeffShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferTriIdx");
	core->glShaderStorageBlockBinding(TcoeffShader.ID, block_index, ssbo_binding);
	core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, *triID);

	ssbo_binding = 9;
	block_index = core->glGetProgramResourceIndex(TcoeffShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferP1");
	core->glShaderStorageBlockBinding(TcoeffShader.ID, block_index, ssbo_binding);
	core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, *p1ID);

	ssbo_binding = 10;
	block_index = core->glGetProgramResourceIndex(TcoeffShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferP2");
	core->glShaderStorageBlockBinding(TcoeffShader.ID, block_index, ssbo_binding);
	core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, *p2ID);

	ssbo_binding = 11;
	block_index = core->glGetProgramResourceIndex(TcoeffShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferP3");
	core->glShaderStorageBlockBinding(TcoeffShader.ID, block_index, ssbo_binding);
	core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, *p3ID);


	//core->glUniform3fv(core->glGetUniformLocation(TcoeffShader.ID, "pmin"), bvhNum, pmin);
	//core->glUniform3fv(core->glGetUniformLocation(TcoeffShader.ID, "pmax"), bvhNum, pmax);
	//core->glUniform1iv(core->glGetUniformLocation(TcoeffShader.ID, "left"), bvhNum, left);
	//core->glUniform1iv(core->glGetUniformLocation(TcoeffShader.ID, "right"), bvhNum, right);
	//core->glUniform1iv(core->glGetUniformLocation(TcoeffShader.ID, "leaf"), bvhNum, leaf);
	//core->glUniform1iv(core->glGetUniformLocation(TcoeffShader.ID, "triIdx"), bvhNum, triIdx);
	////core->glUniform3fv(core->glGetUniformLocation(TcoeffShader.ID, "p1"), triNum, &p1[0][0]);
	////core->glUniform3fv(core->glGetUniformLocation(TcoeffShader.ID, "p2"), triNum, &p2[0][0]);
	////core->glUniform3fv(core->glGetUniformLocation(TcoeffShader.ID, "p3"), triNum, &p3[0][0]);
	//core->glUniform3fv(core->glGetUniformLocation(TcoeffShader.ID, "p1"), triNum, p1);
	//core->glUniform3fv(core->glGetUniformLocation(TcoeffShader.ID, "p2"), triNum, p2);
	//core->glUniform3fv(core->glGetUniformLocation(TcoeffShader.ID, "p3"), triNum, p3);

	float* rdSeed = new float[4];
	for (int i = 0; i < 4; i++)
	{
		float t = rand() / float(RAND_MAX);
		rdSeed[i] = t;
	}
	core->glUniform1fv(core->glGetUniformLocation(TcoeffShader.ID, "rdSeed"), 4, rdSeed);
	core->glUniform1iv(core->glGetUniformLocation(TcoeffShader.ID, "rootBVHIndex"), model.meshesNum, rootBVHIndex);



	FILE* TFile = fopen("dragon_and_bunny/dragon_and_bunny_coeff.txt", "w");
	for (int i = 0; i < model.meshesNum; i++)
	{
		core->glGenBuffers(1, &model.meshes[i]->verPosID);
		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->verPosID);
		core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->numV * 3 * sizeof(model.meshes[i]->vertexPos[0]), &model.meshes[i]->vertexPos[0], GL_DYNAMIC_DRAW);
		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		core->glGenBuffers(1, &model.meshes[i]->verNorID);
		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->verNorID);
		core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->numV * 3 * sizeof(model.meshes[i]->vertexNormal[0]), &model.meshes[i]->vertexNormal[0], GL_DYNAMIC_DRAW);
		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		core->glGenBuffers(1, &model.meshes[i]->TcoeffID);
		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->TcoeffID);
		core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->numV * 81 * sizeof(model.meshes[i]->Tcoeff[0]), &model.meshes[i]->Tcoeff[0], GL_DYNAMIC_DRAW);
		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		ssbo_binding = 0;
		block_index = core->glGetProgramResourceIndex(TcoeffShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferT");
		core->glShaderStorageBlockBinding(TcoeffShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->TcoeffID);

		ssbo_binding = 1;
		block_index = core->glGetProgramResourceIndex(TcoeffShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferPosition");
		core->glShaderStorageBlockBinding(TcoeffShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->verPosID);

		ssbo_binding = 2;
		block_index = core->glGetProgramResourceIndex(TcoeffShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferNormals");
		core->glShaderStorageBlockBinding(TcoeffShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->verNorID);


		TcoeffShader.setInt("numV", model.meshes[i]->numV);
		glm::uvec3 groups = glm::uvec3((model.meshes[i]->numV/128+1), 1, 1);
		//glm::uvec3 groups = glm::uvec3(32, 1, 1);
		
		performCompute(groups);


		





		float *tmp = new float[model.meshes[i]->numV * 81];


		//RayTracing rt;

		//rt.pmin = pmin;
		//rt.pmax = pmax;
		//rt.left = left;
		//rt.right = right;
		//
		//rt.triIdx = triIdx;
		//rt.p1 = p1;
		//rt.p2 = p2;
		//rt.p3 = p3;

		//for (int ii = 0; ii < bvhNum; ii++)
		//{
		//	if (left[ii] > bvhNum || right[ii] > bvhNum)
		//	{
		//		int c = ii;
		//		int a = left[ii];
		//		int b = right[ii];
		//		int d = 1;
		//	}
		//
		//		
		//}

		//for (int k = 0; k < model.meshes[i]->numV; k++)
		//{
		//	glm::vec2 sIdx;
		//	//sIdx.x = gl_LocalInvocationID.x;
		//	uint vIndex = k;

		//	

		//	glm::vec3 curr_pos = glm::vec3(model.meshes[i]->vertexPos[3 * vIndex], model.meshes[i]->vertexPos[3 * vIndex + 1], model.meshes[i]->vertexPos[3 * vIndex + 2]);
		//	glm::vec3 curr_normal = glm::vec3(model.meshes[i]->vertexNormal[3 * vIndex], model.meshes[i]->vertexNormal[3 * vIndex + 1], model.meshes[i]->vertexNormal[3 * vIndex + 2]);
		//	glm::vec3 tangent = rt.getTangent(curr_normal);
		//	glm::vec3 bitangent = glm::cross(curr_normal, tangent);

		//	float SHcoeffs[81];
		//	Ray ray;
		//	ray.origin = curr_pos;

		//	float res[81];
		//	for (int i = 0; i < 81; i++) res[i] = 0.0;

		//	for (int x = 0; x < 50; x++)
		//	{
		//		for (int y = 0; y < 50; y++)
		//		{
		//			sIdx.x = float(x);
		//			sIdx.y = float(y);
		//			glm::vec2 rnd;
		//			rnd.x = rt.random(sIdx - glm::vec2(rdSeed[0], rdSeed[1]));
		//			rnd.y = rt.random(sIdx - glm::vec2(rdSeed[2], rdSeed[3]));

		//			float u = (float(sIdx.x) + rnd.x) / 50.0;
		//			float v = (float(sIdx.y) + rnd.y) / 50.0;



		//			float pdf;
		//			glm::vec3 dirLocal = rt.sample_cosine_hemisphere_concentric(glm::vec2(u, v));
		//			glm::vec3 dir = dirLocal.x * tangent + dirLocal.y * bitangent + dirLocal.z * curr_normal;

		//			//dir = normalize(vec3(-1.f));
		//			ray.direction = dir;



		//			bool flag = false;


		//			//int offset=(x*128+y)*81*10;
		//			for (int ii = 0; ii < 3; ii++) {
		//				//DebugHit(ray, rootBVHIndex[i], offset);
		//				if (rt.hit(ray, rootBVHIndex[ii]))
		//				{
		//					flag = true;
		//					break;
		//				}
		//			}
		//			if (!flag) {
		//				rt.accumShProjection(dir, 1.f);
		//				for (uint j = 0; j < 81; j++)
		//					res[j] += rt.SHcoeffs[j];

		//			}

		//		}
		//	}
		//	for (int ii = 0; ii < 81; ii++)	tmp[81 * vIndex + ii] = res[ii];
		//}
		//




















		
		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->TcoeffID);
		core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, model.meshes[i]->numV * 81 * sizeof(float), tmp);

		float normFactor = 1.f / (50.f*50.f);
		for (int j = 0; j < model.meshes[i]->numV * 81; j++)
		{
			fprintf(TFile, "%.6f", tmp[j] * normFactor);
			if (j % 81 == 80)	fprintf(TFile, "\n");
			else fprintf(TFile, " ");
		}
		free(tmp);

	}
	fclose(TFile);

}


void Prt_Zh::prepareData(model& model, ModelVetexSplit& modelSplit, PolygonalLight& light, Shader TcoeffShader)
{
	FILE* TFile = fopen("dragon_and_bunny/dragon_and_bunny_coeff.txt", "r");
	//FILE* TFile = fopen("easymodel/asian_dragon_coeff.txt", "r");

	if (TFile == NULL)
	{
		computeT(model, modelSplit, TcoeffShader);
		TFile = fopen("dragon_and_bunny/dragon_and_bunny_coeff.txt", "r");
	}

	for (int i = 0; i < model.meshes.size(); i++)
	{
		model.meshes[i]->preComputePerMesh();
		for (int j = 0; j < model.meshes[i]->numV * 81; j++)
		{
			fscanf(TFile, "%f", &model.meshes[i]->Tcoeff[j]);
		}
	}
	fclose(TFile);
	lightPos = new float[light.verticesNum * 3];
	lightEmissive= new float[light.verticesNum];
	//lightNormal = new float[light.polygonalLight.triangleNum];
	int k = 0;
	for (int i = 0; i < light.meshesNum; i++)
	{
		for (int j = 0; j < light.meshes[i]->numV; j++)
		{
			lightPos[k*3] = light.meshes[i]->vertexPos[j * 3];
			lightPos[k*3+1] = light.meshes[i]->vertexPos[j * 3+1];
			lightPos[k * 3 + 2] = light.meshes[i]->vertexPos[j * 3 + 2];
			/*lightEmissive[k * 3] = light.polygonalLight.meshes[i]->material.emissive[0];
			lightEmissive[k * 3 + 1] = light.polygonalLight.meshes[i]->material.emissive[1];
			lightEmissive[k * 3 + 2] = light.polygonalLight.meshes[i]->material.emissive[2];*/
			k++;
		}
	}
	k = 0;
	for (int i = 0; i < light.meshesNum; i++)
	{
		for (int j = 0; j < light.meshes[i]->numV/3; j++)
		{
			lightEmissive[k * 3] = light.meshes[i]->material.emissive[0];
			lightEmissive[k * 3 + 1] = light.meshes[i]->material.emissive[1];
			lightEmissive[k * 3 + 2] = light.meshes[i]->material.emissive[2];
			//lightNormal[k*3]= light.polygonalLight.meshes[i]->vertexNormal[j]
			k++;
		}
	}
	for (int i = 0; i <1*3 ; i++)
	{
		qDebug() <<  lightEmissive[i];
		qDebug() << lightPos[i * 3] << lightPos[i * 3 + 1] << lightPos[i * 3 + 2];
	}
	int a = 5;
}


void Prt_Zh::prepareBufferImage(model& model, PolygonalLight& sceneLight, int MaterialFlag)
{
	if (MaterialFlag==DIFFUSE)
	{
		core->glGenBuffers(1, &lightPosID);

		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightPosID);

		core->glBufferData(GL_SHADER_STORAGE_BUFFER, sceneLight.verticesNum * 3 * sizeof(lightPos[0]), &lightPos[0], GL_DYNAMIC_DRAW);
		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);






		core->glGenBuffers(1, &lightEmissiveID);

		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightEmissiveID);

		core->glBufferData(GL_SHADER_STORAGE_BUFFER, sceneLight.verticesNum * sizeof(lightEmissive[0]), &lightEmissive[0], GL_DYNAMIC_DRAW);
		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		

		structTextureParameters textureParameters1D;
		textureParameters1D.texTarget = GL_TEXTURE_1D;
		textureParameters1D.texInternalFormat = GL_RGBA32F;
		textureParameters1D.texFormat = GL_RGBA;
		createTextures1D(textureParameters1D, &legendre_2345ID, GL_COLOR_ATTACHMENT0, 10001);
		transferToTexture1D(legendre_2345, legendre_2345ID, textureParameters1D, 10001);

		createTextures1D(textureParameters1D, &legendre_6789ID, GL_COLOR_ATTACHMENT1, 10001);
		transferToTexture1D(legendre_6789, legendre_6789ID, textureParameters1D, 10001);


		structTextureParameters textureParameters;
		// create variables for GL
		textureParameters.texTarget = GL_TEXTURE_2D;
		textureParameters.texInternalFormat = GL_RGBA32F;
		textureParameters.texFormat = GL_RGBA;




		for (int i = 0; i < model.meshes.size(); i++)
		{
			createTextures2D(textureParameters, &model.meshes[i]->zhID, GL_COLOR_ATTACHMENT2, 512, 459);
			createTextures2D(textureParameters, &model.meshes[i]->shID, GL_COLOR_ATTACHMENT4, 512, 243);
			transferToTexture2D(model.meshes[i]->gShGrad, model.meshes[i]->shID, textureParameters, 512, 243);
			transferToTexture2D(model.meshes[i]->gZhgrad, model.meshes[i]->zhID, textureParameters, 512, 459);

			core->glGenBuffers(1, &model.meshes[i]->verPosID);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->verPosID);
			core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->numV * 3 * sizeof(model.meshes[i]->vertexPos[0]), &model.meshes[i]->vertexPos[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

			core->glGenBuffers(1, &model.meshes[i]->verColID);

			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->verColID);

			core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->numV * 3 * sizeof(model.meshes[i]->vertexColor[0]), &model.meshes[i]->vertexColor[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

			core->glGenBuffers(1, &model.meshes[i]->TcoeffID);

			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->TcoeffID);

			core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->numV * 81 * sizeof(model.meshes[i]->Tcoeff[0]), &model.meshes[i]->Tcoeff[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}
	}
	else if (MaterialFlag == GLOSSY)
	{
		core->glGenBuffers(1, &lightPosID);

		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightPosID);

		core->glBufferData(GL_SHADER_STORAGE_BUFFER, sceneLight.verticesNum * 3 * sizeof(lightPos[0]), &lightPos[0], GL_DYNAMIC_DRAW);
		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);






		core->glGenBuffers(1, &lightEmissiveID);

		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightEmissiveID);

		core->glBufferData(GL_SHADER_STORAGE_BUFFER, sceneLight.verticesNum * sizeof(lightEmissive[0]), &lightEmissive[0], GL_DYNAMIC_DRAW);
		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		structTextureParameters textureParameters1D;
		textureParameters1D.texTarget = GL_TEXTURE_1D;
		textureParameters1D.texInternalFormat = GL_RGBA32F;
		textureParameters1D.texFormat = GL_RGBA;
		createTextures1D(textureParameters1D, &legendre_2345ID, GL_COLOR_ATTACHMENT0, 10001);
		transferToTexture1D(legendre_2345, legendre_2345ID, textureParameters1D, 10001);

		createTextures1D(textureParameters1D, &legendre_6789ID, GL_COLOR_ATTACHMENT1, 10001);
		transferToTexture1D(legendre_6789, legendre_6789ID, textureParameters1D, 10001);


		structTextureParameters textureParameters;
		// create variables for GL
		textureParameters.texTarget = GL_TEXTURE_2D;
		textureParameters.texInternalFormat = GL_RGBA32F;
		textureParameters.texFormat = GL_RGBA;


		GLuint attchment = GL_COLOR_ATTACHMENT1;
		
		for (int i = 0; i < model.meshes.size(); i++)
		{
			attchment++;
			createTextures2D(textureParameters, &model.meshes[i]->zhID, attchment, 512, 459);
			attchment++;
			createTextures2D(textureParameters, &model.meshes[i]->shID, attchment, 512, 243);
			transferToTexture2D(model.meshes[i]->gZhgrad, model.meshes[i]->zhID, textureParameters, 512, 459);
			transferToTexture2D(model.meshes[i]->gShGrad, model.meshes[i]->shID, textureParameters, 512, 243);


			core->glGenBuffers(1, &model.meshes[i]->shTmpID);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->shTmpID);
			core->glBufferData(GL_SHADER_STORAGE_BUFFER, 512*243*4* sizeof(model.meshes[i]->gShTmpGrad[0]), &model.meshes[i]->gShTmpGrad[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

			core->glGenBuffers(1, &model.meshes[i]->verPosID);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->verPosID);
			core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->numV * 3 * sizeof(model.meshes[i]->vertexPos[0]), &model.meshes[i]->vertexPos[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

			core->glGenBuffers(1, &model.meshes[i]->verColID);

			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->verColID);

			core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->numV * 3 * sizeof(model.meshes[i]->vertexColor[0]), &model.meshes[i]->vertexColor[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

			core->glGenBuffers(1, &model.meshes[i]->TcoeffID);

			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->TcoeffID);

			core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->numV * 81 * sizeof(model.meshes[i]->Tcoeff[0]), &model.meshes[i]->Tcoeff[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


			core->glGenBuffers(1, &model.meshes[i]->verNorID);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->verNorID);

			core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->numV * 3 * sizeof(model.meshes[i]->vertexNormal[0]), &model.meshes[i]->vertexNormal[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


			core->glGenBuffers(1, &model.meshes[i]->loID);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->loID);

			core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->numV * 75 * sizeof(model.meshes[i]->vertexNormal[0]), &model.meshes[i]->lo[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}
	}
}


void Prt_Zh::precompute(Shader shShader, Shader zhToShShader, Shader hermiteShader, Shader hermiteSecondShader, Shader TcoeffShader, PolygonalLight& sceneLight, model& model, ModelVetexSplit& modelSplit, glm::vec3 cameraPos)
{
	if (dataExist == false)
	{
		prepareData(model, modelSplit, sceneLight,TcoeffShader);
		prepareBufferImage(model, sceneLight,GLOSSY);
		dataExist = true;
	}
	
	
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightPosID);
	core->glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sceneLight.verticesNum * 3 * sizeof(lightPos[0]), &lightPos[0]);
	core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	//FILE* TFile = fopen("easymodel/asian_dragon_coeff.txt", "r");
	GLuint attchment = GL_COLOR_ATTACHMENT1;
	for (int i = 0; i < model.meshes.size(); i++)
	{


		//memset(model.meshes[i]->gZhgrad, 0.0f, 4 * 1836 * 512);

		GLint m_oldFBO;
		core->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_oldFBO);
		core->glGenFramebuffers(1, &prtFbo);
		core->glBindFramebuffer(GL_FRAMEBUFFER, prtFbo);

		glm::uvec3 totalSizeSH = glm::uvec3(512, 17, 1);
		glm::uvec3 groupsizeSh = glm::uvec3(128, 1, 1);
		glm::uvec3 groups = totalSizeSH / groupsizeSh;
		//groups = glm::uvec3(1, 1, 1);

		structTextureParameters textureParameters1D;
		textureParameters1D.texTarget = GL_TEXTURE_1D;
		textureParameters1D.texInternalFormat = GL_RGBA32F;
		textureParameters1D.texFormat = GL_RGBA;

		structTextureParameters textureParameters;
		// create variables for GL
		textureParameters.texTarget = GL_TEXTURE_2D;
		textureParameters.texInternalFormat = GL_RGBA32F;
		textureParameters.texFormat = GL_RGBA;

		

		glm::vec3 gridMin = glm::vec3(model.meshes[i]->boundingBox.xmin, model.meshes[i]->boundingBox.ymin, model.meshes[i]->boundingBox.zmin);
		glm::uvec3 reso = glm::ivec3(8, 8, 8);
		glm::vec3 widthxyz = glm::vec3(model.meshes[i]->boundingBox.widthx, model.meshes[i]->boundingBox.widthy, model.meshes[i]->boundingBox.widthz);
		glm::vec3 gridstep = widthxyz / 8.0f - glm::vec3(0.00002);
		shShader.use();
		shShader.setVec3("gridMin", gridMin);
		shShader.setuVec3("reso", reso);
		shShader.setVec3("widthxyz", widthxyz);
		shShader.setInt("lightNum", sceneLight.verticesNum / 3);


		//createTextures1D(textureParameters1D, &legendre_2345ID, attchment, 10001);
		

		//createTextures1D(textureParameters1D, &legendre_6789ID, attchment, 10001);
		

		//createTextures2D(textureParameters, &model.meshes[i]->zhID, attchment, 512, 459);
		


		addTexture(legendre_6789ID, 1);

		attchment++;
		addTexture(model.meshes[i]->zhID, 2);
		//addTexture()
		//transferToTexture1D(legendre_2345, legendre_2345ID, textureParameters1D, 10001);
		addTexture(legendre_2345ID, 0);


		//transferToTexture1D(legendre_6789, legendre_6789ID, textureParameters1D, 10001);
		






		/*core->glGenBuffers(1, &lightPosID);

		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightPosID);

		core->glBufferData(GL_SHADER_STORAGE_BUFFER, sceneLight.polygonalLight.verticesNum * 3 * sizeof(lightPos[0]), &lightPos[0], GL_DYNAMIC_DRAW);
		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);*/
		

	

		int ssbo_binding = 3;
		int block_index = core->glGetProgramResourceIndex(shShader.ID, GL_SHADER_STORAGE_BLOCK, "lightPosBuffers");
		core->glShaderStorageBlockBinding(shShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, lightPosID);



		/*core->glGenBuffers(1, &lightEmissiveID);

		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightEmissiveID);

		core->glBufferData(GL_SHADER_STORAGE_BUFFER, sceneLight.polygonalLight.verticesNum  * sizeof(lightEmissive[0]), &lightEmissive[0], GL_DYNAMIC_DRAW);
		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);*/


		ssbo_binding = 4;
		block_index = core->glGetProgramResourceIndex(shShader.ID, GL_SHADER_STORAGE_BLOCK, "lightEmissiveBuffers");
		core->glShaderStorageBlockBinding(shShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, lightEmissiveID);




		performCompute(groups);
		// get GPU results


		//transferFromTexture2D(model.meshes[i]->gZhgrad, textureParameters, 512, 459, attchment);


		




		//core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightEmissiveID);
		//core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sceneLight.polygonalLight.triangleNum  * sizeof(lightEmissive[0]), &lightEmissive[0]);

		//for (int j = 0; j < sceneLight.polygonalLight.triangleNum ; j++)
		//{
		//	qDebug() << lightEmissive[j];
		//}


		/*for (int j = 512*4*0; j < 512 * 459 * 4; j++)
		{
			qDebug() << model.meshes[i]->gZhgrad[j];
		}
		qDebug() << "finish";*/


		zhToShShader.use();



		totalSizeSH = glm::uvec3(512, 3, 1);
		groupsizeSh = glm::uvec3(128, 1, 1);
		groups = totalSizeSH / groupsizeSh;
		//groups = glm::uvec3(1, 1, 1);

		//createTextures2D(textureParameters, &model.meshes[i]->zhID, attchment, 512, 459);
		
		addTexture(model.meshes[i]->zhID, 0);
		//transferToTexture2D(model.meshes[i]->gZhgrad, model.meshes[i]->zhID, textureParameters, 512, 459);



		//createTextures2D(textureParameters, &model.meshes[i]->shID, attchment, 512, 243);
		attchment++;

		//!!
		//addTexture(model.meshes[i]->shID, 1);


		//transferToTexture2D(model.meshes[i]->gShGrad, model.meshes[i]->shID, textureParameters, 512, 243);


		ssbo_binding = 2;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferSh");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->shTmpID);


		performCompute(groups);
		//transferFromTexture2D(model.meshes[i]->gShGrad, textureParameters, 512, 243, GL_COLOR_ATTACHMENT4);

		//for (int j = 0; j < 512*243*4 ; j++) {

		//	//qDebug() << model.meshes[i]->gShGrad[j];
		//}
		//qDebug() << "finish";

		//attchment = GL_COLOR_ATTACHMENT5;
		hermiteShader.use();

		hermiteShader.setVec3("gridStep", gridstep);
		hermiteShader.setVec3("gridMin", gridMin);
		hermiteShader.setInt("numVertex", model.meshes[i]->numV);
		hermiteShader.setuVec3("gridReso", reso);
		/*hermiteShader.setVec3("cameraPos", cameraPos);
		hermiteShader.setVec3("diffuse", model.meshes[i]->material.albedo);
		hermiteShader.setVec3("specular", model.meshes[i]->material.specular);
		hermiteShader.setFloat("ns", model.meshes[i]->material.shininess);*/

		//createTextures2D(textureParameters, &model.meshes[i]->shID, attchment, 512, 243);

		//addTexture(model.meshes[i]->shID, 0);
		//!!
		//addTextureReadOnly(model.meshes[i]->shID, 0);


		//transferToTexture2D(model.meshes[i]->gShGrad, model.meshes[i]->shID, textureParameters, 512, 243);


		/*createTextures2D(textureParameters, &meshes[i]->verColID, attchment, 512, tempY);
		attchment++;
		addTexture(meshes[i]->verColID, 2);
		transferToTexture2D(meshes[i]->vertexColor, meshes[i]->verColID, textureParameters, 512, tempY);


		int tempY2 = meshes[i]->numV / 100 + 1;

		createTextures2D(textureParameters, &meshes[i]->TcoeffID, attchment, 2700, tempY2);
		attchment++;
		addTexture(meshes[i]->TcoeffID, 3);

		transferToTexture2D(Tcoff, meshes[i]->TcoeffID, textureParameters, 2700, tempY2);*/



		//std::vector<glm::vec4> data = { glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec4(1.0, 0.0, 0.0, 1.0) };

		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		ssbo_binding = 0;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferSh");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->shTmpID);



		ssbo_binding = 1;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferPosition");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->verPosID);







		/*ssbo_binding = 2;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferVertexColors");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->verColID);










		ssbo_binding = 3;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferTcoeffs");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->TcoeffID);


		ssbo_binding = 4;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferVertexNormals");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->verNorID);
*/
		ssbo_binding = 5;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferLo");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->loID);


		totalSizeSH = glm::uvec3(model.meshes[i]->numV, 3, 1);
		groupsizeSh = glm::uvec3(64, 3, 1);
		groups = totalSizeSH / groupsizeSh;
		groups.x++;
		//groups = glm::uvec3(1, 1, 1);
		//core->glFinish();
		performCompute(groups);
		//core->glFinish();
		// get GPU results
		/*core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
		core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, model.meshes[i]->numV * 3 * sizeof(model.meshes[i]->vertexPos[0]), model.meshes[i]->vertexPos);*/


		/*core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->verColID);
		core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, model.meshes[i]->numV * 3 * sizeof(model.meshes[i]->vertexColor[0]), model.meshes[i]->vertexColor);*/


		/*core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO2);
		core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, model.meshes[i]->numV * 81 * sizeof(model.meshes[i]->Tcoeff[0]), model.meshes[i]->Tcoeff);*/

		//transferFromTexture2D(meshes[i]->vertexColor, textureParameters, 512, tempY, GL_COLOR_ATTACHMENT7);

		//for (int j = 0; j < model.meshes[i]->numV*3; j++) {

		//	//qDebug() << model.meshes[i]->vertexPos[j];
		//	qDebug() << model.meshes[i]->vertexColor[j];
		//	//qDebug() << model.meshes[i]->Tcoeff[j*9];
		//}
		//qDebug() << "finish";
		hermiteSecondShader.use();


		hermiteSecondShader.setVec3("gridStep", gridstep);
		hermiteSecondShader.setVec3("gridMin", gridMin);
		hermiteSecondShader.setInt("numVertex", model.meshes[i]->numV);
		hermiteSecondShader.setuVec3("gridReso", reso);
		hermiteSecondShader.setVec3("cameraPos", cameraPos);
		hermiteSecondShader.setVec3("diffuse", model.meshes[i]->material.albedo);
		hermiteSecondShader.setVec3("specular", model.meshes[i]->material.specular);
		hermiteSecondShader.setFloat("ns", model.meshes[i]->material.shininess);

		
		



		ssbo_binding = 1;
		block_index = core->glGetProgramResourceIndex(hermiteSecondShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferPosition");
		core->glShaderStorageBlockBinding(hermiteSecondShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->verPosID);







		ssbo_binding = 2;
		block_index = core->glGetProgramResourceIndex(hermiteSecondShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferVertexColors");
		core->glShaderStorageBlockBinding(hermiteSecondShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->verColID);










		ssbo_binding = 3;
		block_index = core->glGetProgramResourceIndex(hermiteSecondShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferTcoeffs");
		core->glShaderStorageBlockBinding(hermiteSecondShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->TcoeffID);


		ssbo_binding = 4;
		block_index = core->glGetProgramResourceIndex(hermiteSecondShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferVertexNormals");
		core->glShaderStorageBlockBinding(hermiteSecondShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->verNorID);

		ssbo_binding = 5;
		block_index = core->glGetProgramResourceIndex(hermiteSecondShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferLo");
		core->glShaderStorageBlockBinding(hermiteSecondShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->loID);


		totalSizeSH = glm::uvec3(model.meshes[i]->numV, 3, 1);
		groupsizeSh = glm::uvec3(64, 3, 1);
		groups = totalSizeSH / groupsizeSh;
		groups.x++;
		//groups = glm::uvec3(1, 1, 1);
		//core->glFinish();
		performCompute(groups);



		model.meshes[i]->pushVerticesColor();



		core->glBindFramebuffer(GL_FRAMEBUFFER, m_oldFBO);

	}
}

void Prt_Zh::precompute(Shader shShader, Shader zhToShShader, Shader hermiteShader, Shader TcoeffShader, PolygonalLight& sceneLight,model& model, ModelVetexSplit& modelSplit )
{
	if (dataExist == false)
	{
		prepareData(model, modelSplit, sceneLight,TcoeffShader);
		prepareBufferImage(model, sceneLight,DIFFUSE);
		dataExist = true;
	}

	//FILE* TFile = fopen("easymodel/asian_dragon_coeff.txt", "r");

	for (int i = 0; i < model.meshes.size(); i++)
	{


		memset(model.meshes[i]->gZhgrad, 0.0f, 4 * 1836 * 512);

		GLint m_oldFBO;
		core->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_oldFBO);
		core->glGenFramebuffers(1, &prtFbo);
		core->glBindFramebuffer(GL_FRAMEBUFFER, prtFbo);

		glm::uvec3 totalSizeSH = glm::uvec3(512, 17, 1);
		glm::uvec3 groupsizeSh = glm::uvec3(512, 1, 1);
		glm::uvec3 groups = totalSizeSH / groupsizeSh;

		structTextureParameters textureParameters1D;
		textureParameters1D.texTarget = GL_TEXTURE_1D;
		textureParameters1D.texInternalFormat = GL_RGBA32F;
		textureParameters1D.texFormat = GL_RGBA;

		structTextureParameters textureParameters;
		// create variables for GL
		textureParameters.texTarget = GL_TEXTURE_2D;
		textureParameters.texInternalFormat = GL_RGBA32F;
		textureParameters.texFormat = GL_RGBA;

		GLuint attchment = GL_COLOR_ATTACHMENT0;

		glm::vec3 gridMin = glm::vec3(model.meshes[i]->boundingBox.xmin, model.meshes[i]->boundingBox.ymin, model.meshes[i]->boundingBox.zmin);
		glm::uvec3 reso = glm::ivec3(8, 8, 8);
		glm::vec3 widthxyz = glm::vec3(model.meshes[i]->boundingBox.widthx, model.meshes[i]->boundingBox.widthy, model.meshes[i]->boundingBox.widthz);
		glm::vec3 gridstep = widthxyz / 8.0f - glm::vec3(0.00002);
		shShader.use();
		shShader.setVec3("gridMin", gridMin);
		shShader.setuVec3("reso", reso);
		shShader.setVec3("widthxyz", widthxyz);
		shShader.setInt("lightNum", sceneLight.verticesNum / 3);


		//createTextures1D(textureParameters1D, &legendre_2345ID, attchment, 10001);
		attchment++;

		//createTextures1D(textureParameters1D, &legendre_6789ID, attchment, 10001);
		attchment++;

		//createTextures2D(textureParameters, &model.meshes[i]->zhID, attchment, 512, 459);
		attchment++;


		addTexture(legendre_6789ID, 1);
		addTexture(model.meshes[i]->zhID, 2);
		//addTexture()
		//transferToTexture1D(legendre_2345, legendre_2345ID, textureParameters1D, 10001);
		addTexture(legendre_2345ID, 0);


		//transferToTexture1D(legendre_6789, legendre_6789ID, textureParameters1D, 10001);
		transferToTexture2D(model.meshes[i]->gZhgrad, model.meshes[i]->zhID, textureParameters, 512, 459);






		/*core->glGenBuffers(1, &lightPosID);

		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightPosID);

		core->glBufferData(GL_SHADER_STORAGE_BUFFER, sceneLight.polygonalLight.verticesNum * 3 * sizeof(lightPos[0]), &lightPos[0], GL_DYNAMIC_DRAW);
		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);*/

		//int k = 0;
		//for (int i = 0; i < sceneLight.polygonalLight.meshesNum; i++)
		//{
		//	for (int j = 0; j < sceneLight.polygonalLight.meshes[i]->numV; j++)
		//	{
		//		lightPos[k * 3] = sceneLight.polygonalLight.meshes[i]->vertexPos[j * 3];
		//		lightPos[k * 3 + 1] = sceneLight.polygonalLight.meshes[i]->vertexPos[j * 3 + 1];
		//		lightPos[k * 3 + 2] = sceneLight.polygonalLight.meshes[i]->vertexPos[j * 3 + 2];
		//		/*lightEmissive[k * 3] = light.polygonalLight.meshes[i]->material.emissive[0];
		//		lightEmissive[k * 3 + 1] = light.polygonalLight.meshes[i]->material.emissive[1];
		//		lightEmissive[k * 3 + 2] = light.polygonalLight.meshes[i]->material.emissive[2];*/
		//		k++;
		//	}
		//}

		int ssbo_binding = 3;
		int block_index = core->glGetProgramResourceIndex(shShader.ID, GL_SHADER_STORAGE_BLOCK, "lightPosBuffers");
		core->glShaderStorageBlockBinding(shShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, lightPosID);



		/*core->glGenBuffers(1, &lightEmissiveID);

		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightEmissiveID);

		core->glBufferData(GL_SHADER_STORAGE_BUFFER, sceneLight.polygonalLight.verticesNum  * sizeof(lightEmissive[0]), &lightEmissive[0], GL_DYNAMIC_DRAW);
		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);*/


		ssbo_binding = 4;
		block_index = core->glGetProgramResourceIndex(shShader.ID, GL_SHADER_STORAGE_BLOCK, "lightEmissiveBuffers");
		core->glShaderStorageBlockBinding(shShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, lightEmissiveID);




		performCompute(groups);
		// get GPU results


		transferFromTexture2D(model.meshes[i]->gZhgrad, textureParameters, 512, 459, GL_COLOR_ATTACHMENT2);





		//core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightEmissiveID);
		//core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sceneLight.polygonalLight.triangleNum  * sizeof(lightEmissive[0]), &lightEmissive[0]);

		//for (int j = 0; j < sceneLight.polygonalLight.triangleNum ; j++)
		//{
		//	qDebug() << lightEmissive[j];
		//}


	/*	for (int j = 512*4*0; j < 512 * 2 * 4; j++) 
		{
			qDebug() << model.meshes[i]->gZhgrad[j];
		}
		qDebug() << "finish";*/


		zhToShShader.use();



		totalSizeSH = glm::uvec3(512, 3, 1);
		groupsizeSh = glm::uvec3(512, 1, 1);
		groups = totalSizeSH / groupsizeSh;


		//createTextures2D(textureParameters, &model.meshes[i]->zhID, attchment, 512, 459);
		attchment++;
		addTexture(model.meshes[i]->zhID, 0);
		//transferToTexture2D(model.meshes[i]->gZhgrad, model.meshes[i]->zhID, textureParameters, 512, 459);



		//createTextures2D(textureParameters, &model.meshes[i]->shID, attchment, 512, 243);
		attchment++;
		addTexture(model.meshes[i]->shID, 1);
		//transferToTexture2D(model.meshes[i]->gShGrad, model.meshes[i]->shID, textureParameters, 512, 243);

		performCompute(groups);
		//transferFromTexture2D(model.meshes[i]->gShGrad, textureParameters, 512, 243, GL_COLOR_ATTACHMENT4);

		//for (int j = 0; j < 512*243*4 ; j++) {

		//	//qDebug() << model.meshes[i]->gShGrad[j];
		//}
		//qDebug() << "finish";

		//attchment = GL_COLOR_ATTACHMENT5;
		hermiteShader.use();

		hermiteShader.setVec3("gridStep", gridstep);
		hermiteShader.setVec3("gridMin", gridMin);
		hermiteShader.setInt("numVertex", model.meshes[i]->numV);
		hermiteShader.setuVec3("gridReso", reso);




		//createTextures2D(textureParameters, &model.meshes[i]->shID, attchment, 512, 243);
		attchment++;
		addTexture(model.meshes[i]->shID, 0);
		//transferToTexture2D(model.meshes[i]->gShGrad, model.meshes[i]->shID, textureParameters, 512, 243);


		/*createTextures2D(textureParameters, &meshes[i]->verColID, attchment, 512, tempY);
		attchment++;
		addTexture(meshes[i]->verColID, 2);
		transferToTexture2D(meshes[i]->vertexColor, meshes[i]->verColID, textureParameters, 512, tempY);


		int tempY2 = meshes[i]->numV / 100 + 1;

		createTextures2D(textureParameters, &meshes[i]->TcoeffID, attchment, 2700, tempY2);
		attchment++;
		addTexture(meshes[i]->TcoeffID, 3);

		transferToTexture2D(Tcoff, meshes[i]->TcoeffID, textureParameters, 2700, tempY2);*/



		//std::vector<glm::vec4> data = { glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec4(1.0, 0.0, 0.0, 1.0) };






		ssbo_binding = 4;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferPosition");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->verPosID);







		ssbo_binding = 5;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferVertexColors");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->verColID);










		ssbo_binding = 6;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferTcoeffs");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i]->TcoeffID);



		totalSizeSH = glm::uvec3(model.meshes[i]->numV, 3, 1);
		groupsizeSh = glm::uvec3(64, 3, 1);
		groups = totalSizeSH / groupsizeSh;
		groups.x++;
		core->glFinish();
		performCompute(groups);

		// get GPU results
		/*core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
		core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, model.meshes[i]->numV * 3 * sizeof(model.meshes[i]->vertexPos[0]), model.meshes[i]->vertexPos);*/
		/*core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i]->verColID);
		core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, model.meshes[i]->numV * 3 * sizeof(model.meshes[i]->vertexColor[0]), model.meshes[i]->vertexColor);*/
		/*core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO2);
		core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, model.meshes[i]->numV * 81 * sizeof(model.meshes[i]->Tcoeff[0]), model.meshes[i]->Tcoeff);*/

		//transferFromTexture2D(meshes[i]->vertexColor, textureParameters, 512, tempY, GL_COLOR_ATTACHMENT7);

		/*for (int j = model.meshes[i]->numV * 3-240; j < model.meshes[i]->numV*3; j++) {

			qDebug() << model.meshes[i]->vertexPos[j];
			qDebug() << model.meshes[i]->vertexColor[j];
			qDebug() << model.meshes[i]->Tcoeff[j*9];
		}
		qDebug() << "finish";*/
		model.meshes[i]->pushVerticesColor();



		core->glBindFramebuffer(GL_FRAMEBUFFER, m_oldFBO);

	}
	
}


void Prt_Zh::transferToTexture2D(float *data, GLuint texID, structTextureParameters textureParameters, GLuint width, GLuint height) {
	core->glBindTexture(textureParameters.texTarget, texID);
	core->glTexSubImage2D(textureParameters.texTarget, 0, 0, 0, width, height, textureParameters.texFormat, GL_FLOAT, data);
}

void Prt_Zh::transferToTexture1D(float *data, GLuint texID, structTextureParameters textureParameters, GLuint size) {
	core->glBindTexture(textureParameters.texTarget, texID);
	core->glTexSubImage1D(textureParameters.texTarget, 0, 0, size, textureParameters.texFormat, GL_FLOAT, data);
}

void Prt_Zh::transferFromTexture2D(float *data, structTextureParameters textureParameters, GLuint width, GLuint height, GLuint attchment) {
	core->glReadBuffer(attchment);
	core->glReadPixels(0, 0, width, height, textureParameters.texFormat, GL_FLOAT, data);
}

void Prt_Zh::setupTexture2D(const GLuint texID, structTextureParameters textureParameters, GLuint width, GLuint height) {
	// make active and bind
	core->glBindTexture(textureParameters.texTarget, texID);
	core->glTexStorage2D(textureParameters.texTarget, 1, GL_RGBA32F, width, height);
	// turn off filtering and wrap modes
	core->glTexParameteri(textureParameters.texTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	core->glTexParameteri(textureParameters.texTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	core->glTexParameteri(textureParameters.texTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
	core->glTexParameteri(textureParameters.texTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void Prt_Zh::setupTexture1D(const GLuint texID, structTextureParameters textureParameters, GLuint size) {
	// make active and bind
	core->glBindTexture(textureParameters.texTarget, texID);
	core->glTexStorage1D(textureParameters.texTarget, 1, GL_RGBA32F, size);
	// turn off filtering and wrap modes
	core->glTexParameteri(textureParameters.texTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	core->glTexParameteri(textureParameters.texTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	core->glTexParameteri(textureParameters.texTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
	core->glTexParameteri(textureParameters.texTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

//void model::performCompute(const GLuint inputTexID, const GLuint outputTexID, Shader shader) {
//	// enable GLSL program
//
//	shader.setVec4("v", v);
//	addTexture(inputTexID, 0);
//	addTexture(outputTexID, 1);
//
//	core->glDispatchCompute(2, 1, 1);
//	core->glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
//	//core->glFinish();
//	/*lTime = timer.getTime();
//	cout << "Time elapsed: " << lTime << " ms." << endl;*/
//}

void Prt_Zh::performCompute(glm::uvec3 group) {
	// enable GLSL program

	core->glDispatchCompute(group.x, group.y, group.z);
	core->glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	//core->glFinish();
	/*lTime = timer.getTime();
	cout << "Time elapsed: " << lTime << " ms." << endl;*/
}

void Prt_Zh::addTexture(const GLuint ID, const GLuint bind)
{
	core->glBindImageTexture(bind, ID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

void Prt_Zh::addTextureReadOnly(const GLuint ID, const GLuint bind)
{
	core->glBindImageTexture(bind, ID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
}

void Prt_Zh::createTextures2D(structTextureParameters textureParameters, GLuint* ID, GLuint attchment, GLuint width, GLuint height)
{
	core->glGenTextures(1, ID);
	// set up textures
	setupTexture2D(*ID, textureParameters, width, height);
	core->glFramebufferTexture2D(GL_FRAMEBUFFER, attchment, textureParameters.texTarget, *ID, 0);
}


void Prt_Zh::createTextures1D(structTextureParameters textureParameters, GLuint* ID, GLuint attachment, GLuint size) {

	core->glGenTextures(1, ID);
	// set up textures
	setupTexture1D(*ID, textureParameters, size);
	core->glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textureParameters.texTarget, *ID, 0);
}

void Prt_Zh::updateLightPos(float angle, glm::vec3 dir, PolygonalLight& light)
{

	
	
	int k = 0;
	for (int i = 0; i < light.meshesNum; i++)
	{
		for (int j = 0; j < light.meshes[i]->numV; j++)
		{
			float x = light.meshes[i]->vertexPos[j * 3];
			float y = light.meshes[i]->vertexPos[j * 3 + 1];
			float z = light.meshes[i]->vertexPos[j * 3 + 2];
			glm::mat4x4 trans(1.0f);
			glm::vec3 pos(x, y, z);
			glm::vec4 result(1.0);
			trans = glm::rotate(trans, glm::radians(angle), dir);
			result = trans * glm::vec4(pos, 1.0);
			lightPos[k * 3] = result.x;
			lightPos[k * 3 + 1] = result.y;
			lightPos[k * 3 + 2] = result.z;
			
			k++;
		}
	}
}