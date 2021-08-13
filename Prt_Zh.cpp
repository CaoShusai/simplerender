#include "Prt_Zh.h"
void Prt_Zh::prepareData(model& model, PolygonalLight& light)
{
	FILE* TFile = fopen("dragon_and_bunny/dragon_and_bunny_coeff.txt", "r");
	//FILE* TFile = fopen("easymodel/asian_dragon_coeff.txt", "r");
	for (int i = 0; i < model.meshes.size(); i++)
	{
		model.meshes[i].preComputePerMesh();
		for (int j = 0; j < model.meshes[i].numV * 81; j++)
		{
			fscanf(TFile, "%f", &model.meshes[i].Tcoeff[j]);
		}
	}
	fclose(TFile);
	lightPos = new float[light.verticesNum * 3];
	lightEmissive= new float[light.verticesNum];
	//lightNormal = new float[light.polygonalLight.triangleNum];
	int k = 0;
	for (int i = 0; i < light.meshesNum; i++)
	{
		for (int j = 0; j < light.meshes[i].numV; j++)
		{
			lightPos[k*3] = light.meshes[i].vertexPos[j * 3];
			lightPos[k*3+1] = light.meshes[i].vertexPos[j * 3+1];
			lightPos[k * 3 + 2] = light.meshes[i].vertexPos[j * 3 + 2];
			/*lightEmissive[k * 3] = light.polygonalLight.meshes[i].material.emissive[0];
			lightEmissive[k * 3 + 1] = light.polygonalLight.meshes[i].material.emissive[1];
			lightEmissive[k * 3 + 2] = light.polygonalLight.meshes[i].material.emissive[2];*/
			k++;
		}
	}
	k = 0;
	for (int i = 0; i < light.meshesNum; i++)
	{
		for (int j = 0; j < light.meshes[i].numV/3; j++)
		{
			lightEmissive[k * 3] = light.meshes[i].material.emissive[0];
			lightEmissive[k * 3 + 1] = light.meshes[i].material.emissive[1];
			lightEmissive[k * 3 + 2] = light.meshes[i].material.emissive[2];
			//lightNormal[k*3]= light.polygonalLight.meshes[i].vertexNormal[j]
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
			createTextures2D(textureParameters, &model.meshes[i].zhID, GL_COLOR_ATTACHMENT2, 512, 459);
			createTextures2D(textureParameters, &model.meshes[i].shID, GL_COLOR_ATTACHMENT4, 512, 243);
			transferToTexture2D(model.meshes[i].gShGrad, model.meshes[i].shID, textureParameters, 512, 243);
			transferToTexture2D(model.meshes[i].gZhgrad, model.meshes[i].zhID, textureParameters, 512, 459);

			core->glGenBuffers(1, &model.meshes[i].verPosID);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i].verPosID);
			core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i].numV * 3 * sizeof(model.meshes[i].vertexPos[0]), &model.meshes[i].vertexPos[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

			core->glGenBuffers(1, &model.meshes[i].verColID);

			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i].verColID);

			core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i].numV * 3 * sizeof(model.meshes[i].vertexColor[0]), &model.meshes[i].vertexColor[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

			core->glGenBuffers(1, &model.meshes[i].TcoeffID);

			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i].TcoeffID);

			core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i].numV * 81 * sizeof(model.meshes[i].Tcoeff[0]), &model.meshes[i].Tcoeff[0], GL_DYNAMIC_DRAW);
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
			createTextures2D(textureParameters, &model.meshes[i].zhID, attchment, 512, 459);
			attchment++;
			createTextures2D(textureParameters, &model.meshes[i].shID, attchment, 512, 243);
			transferToTexture2D(model.meshes[i].gZhgrad, model.meshes[i].zhID, textureParameters, 512, 459);
			transferToTexture2D(model.meshes[i].gShGrad, model.meshes[i].shID, textureParameters, 512, 243);


			core->glGenBuffers(1, &model.meshes[i].shTmpID);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i].shTmpID);
			core->glBufferData(GL_SHADER_STORAGE_BUFFER, 512*243*4* sizeof(model.meshes[i].gShTmpGrad[0]), &model.meshes[i].gShTmpGrad[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

			core->glGenBuffers(1, &model.meshes[i].verPosID);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i].verPosID);
			core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i].numV * 3 * sizeof(model.meshes[i].vertexPos[0]), &model.meshes[i].vertexPos[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

			core->glGenBuffers(1, &model.meshes[i].verColID);

			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i].verColID);

			core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i].numV * 3 * sizeof(model.meshes[i].vertexColor[0]), &model.meshes[i].vertexColor[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

			core->glGenBuffers(1, &model.meshes[i].TcoeffID);

			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i].TcoeffID);

			core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i].numV * 81 * sizeof(model.meshes[i].Tcoeff[0]), &model.meshes[i].Tcoeff[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


			core->glGenBuffers(1, &model.meshes[i].verNorID);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i].verNorID);

			core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i].numV * 3 * sizeof(model.meshes[i].vertexNormal[0]), &model.meshes[i].vertexNormal[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


			core->glGenBuffers(1, &model.meshes[i].loID);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i].loID);

			core->glBufferData(GL_SHADER_STORAGE_BUFFER, model.meshes[i].numV * 75 * sizeof(model.meshes[i].vertexNormal[0]), &model.meshes[i].lo[0], GL_DYNAMIC_DRAW);
			core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}
	}
}


void Prt_Zh::precompute(Shader shShader, Shader zhToShShader, Shader hermiteShader, Shader hermiteSecondShader, PolygonalLight& sceneLight, model& model, glm::vec3 cameraPos)
{
	if (dataExist == false)
	{
		prepareData(model, sceneLight);
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


		//memset(model.meshes[i].gZhgrad, 0.0f, 4 * 1836 * 512);

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

		

		glm::vec3 gridMin = glm::vec3(model.meshes[i].boundingBox.xmin, model.meshes[i].boundingBox.ymin, model.meshes[i].boundingBox.zmin);
		glm::uvec3 reso = glm::ivec3(8, 8, 8);
		glm::vec3 widthxyz = glm::vec3(model.meshes[i].boundingBox.widthx, model.meshes[i].boundingBox.widthy, model.meshes[i].boundingBox.widthz);
		glm::vec3 gridstep = widthxyz / 8.0f - glm::vec3(0.00002);
		shShader.use();
		shShader.setVec3("gridMin", gridMin);
		shShader.setuVec3("reso", reso);
		shShader.setVec3("widthxyz", widthxyz);
		shShader.setInt("lightNum", sceneLight.verticesNum / 3);


		//createTextures1D(textureParameters1D, &legendre_2345ID, attchment, 10001);
		

		//createTextures1D(textureParameters1D, &legendre_6789ID, attchment, 10001);
		

		//createTextures2D(textureParameters, &model.meshes[i].zhID, attchment, 512, 459);
		


		addTexture(legendre_6789ID, 1);

		attchment++;
		addTexture(model.meshes[i].zhID, 2);
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


		//transferFromTexture2D(model.meshes[i].gZhgrad, textureParameters, 512, 459, attchment);


		




		//core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightEmissiveID);
		//core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sceneLight.polygonalLight.triangleNum  * sizeof(lightEmissive[0]), &lightEmissive[0]);

		//for (int j = 0; j < sceneLight.polygonalLight.triangleNum ; j++)
		//{
		//	qDebug() << lightEmissive[j];
		//}


		/*for (int j = 512*4*0; j < 512 * 459 * 4; j++)
		{
			qDebug() << model.meshes[i].gZhgrad[j];
		}
		qDebug() << "finish";*/


		zhToShShader.use();



		totalSizeSH = glm::uvec3(512, 3, 1);
		groupsizeSh = glm::uvec3(128, 1, 1);
		groups = totalSizeSH / groupsizeSh;
		//groups = glm::uvec3(1, 1, 1);

		//createTextures2D(textureParameters, &model.meshes[i].zhID, attchment, 512, 459);
		
		addTexture(model.meshes[i].zhID, 0);
		//transferToTexture2D(model.meshes[i].gZhgrad, model.meshes[i].zhID, textureParameters, 512, 459);



		//createTextures2D(textureParameters, &model.meshes[i].shID, attchment, 512, 243);
		attchment++;

		//!!
		//addTexture(model.meshes[i].shID, 1);


		//transferToTexture2D(model.meshes[i].gShGrad, model.meshes[i].shID, textureParameters, 512, 243);


		ssbo_binding = 2;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferSh");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i].shTmpID);


		performCompute(groups);
		//transferFromTexture2D(model.meshes[i].gShGrad, textureParameters, 512, 243, GL_COLOR_ATTACHMENT4);

		//for (int j = 0; j < 512*243*4 ; j++) {

		//	//qDebug() << model.meshes[i].gShGrad[j];
		//}
		//qDebug() << "finish";

		//attchment = GL_COLOR_ATTACHMENT5;
		hermiteShader.use();

		hermiteShader.setVec3("gridStep", gridstep);
		hermiteShader.setVec3("gridMin", gridMin);
		hermiteShader.setInt("numVertex", model.meshes[i].numV);
		hermiteShader.setuVec3("gridReso", reso);
		/*hermiteShader.setVec3("cameraPos", cameraPos);
		hermiteShader.setVec3("diffuse", model.meshes[i].material.albedo);
		hermiteShader.setVec3("specular", model.meshes[i].material.specular);
		hermiteShader.setFloat("ns", model.meshes[i].material.shininess);*/

		//createTextures2D(textureParameters, &model.meshes[i].shID, attchment, 512, 243);

		//addTexture(model.meshes[i].shID, 0);
		//!!
		//addTextureReadOnly(model.meshes[i].shID, 0);


		//transferToTexture2D(model.meshes[i].gShGrad, model.meshes[i].shID, textureParameters, 512, 243);


		/*createTextures2D(textureParameters, &meshes[i].verColID, attchment, 512, tempY);
		attchment++;
		addTexture(meshes[i].verColID, 2);
		transferToTexture2D(meshes[i].vertexColor, meshes[i].verColID, textureParameters, 512, tempY);


		int tempY2 = meshes[i].numV / 100 + 1;

		createTextures2D(textureParameters, &meshes[i].TcoeffID, attchment, 2700, tempY2);
		attchment++;
		addTexture(meshes[i].TcoeffID, 3);

		transferToTexture2D(Tcoff, meshes[i].TcoeffID, textureParameters, 2700, tempY2);*/



		//std::vector<glm::vec4> data = { glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec4(1.0, 0.0, 0.0, 1.0) };

		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		ssbo_binding = 0;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferSh");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i].shTmpID);



		ssbo_binding = 1;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferPosition");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i].verPosID);







		/*ssbo_binding = 2;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferVertexColors");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i].verColID);










		ssbo_binding = 3;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferTcoeffs");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i].TcoeffID);


		ssbo_binding = 4;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferVertexNormals");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i].verNorID);
*/
		ssbo_binding = 5;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferLo");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i].loID);


		totalSizeSH = glm::uvec3(model.meshes[i].numV, 3, 1);
		groupsizeSh = glm::uvec3(64, 3, 1);
		groups = totalSizeSH / groupsizeSh;
		groups.x++;
		//groups = glm::uvec3(1, 1, 1);
		//core->glFinish();
		performCompute(groups);
		//core->glFinish();
		// get GPU results
		/*core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
		core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, model.meshes[i].numV * 3 * sizeof(model.meshes[i].vertexPos[0]), model.meshes[i].vertexPos);*/


		/*core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i].verColID);
		core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, model.meshes[i].numV * 3 * sizeof(model.meshes[i].vertexColor[0]), model.meshes[i].vertexColor);*/


		/*core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO2);
		core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, model.meshes[i].numV * 81 * sizeof(model.meshes[i].Tcoeff[0]), model.meshes[i].Tcoeff);*/

		//transferFromTexture2D(meshes[i].vertexColor, textureParameters, 512, tempY, GL_COLOR_ATTACHMENT7);

		//for (int j = 0; j < model.meshes[i].numV*3; j++) {

		//	//qDebug() << model.meshes[i].vertexPos[j];
		//	qDebug() << model.meshes[i].vertexColor[j];
		//	//qDebug() << model.meshes[i].Tcoeff[j*9];
		//}
		//qDebug() << "finish";
		hermiteSecondShader.use();


		hermiteSecondShader.setVec3("gridStep", gridstep);
		hermiteSecondShader.setVec3("gridMin", gridMin);
		hermiteSecondShader.setInt("numVertex", model.meshes[i].numV);
		hermiteSecondShader.setuVec3("gridReso", reso);
		hermiteSecondShader.setVec3("cameraPos", cameraPos);
		hermiteSecondShader.setVec3("diffuse", model.meshes[i].material.albedo);
		hermiteSecondShader.setVec3("specular", model.meshes[i].material.specular);
		hermiteSecondShader.setFloat("ns", model.meshes[i].material.shininess);

		
		



		ssbo_binding = 1;
		block_index = core->glGetProgramResourceIndex(hermiteSecondShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferPosition");
		core->glShaderStorageBlockBinding(hermiteSecondShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i].verPosID);







		ssbo_binding = 2;
		block_index = core->glGetProgramResourceIndex(hermiteSecondShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferVertexColors");
		core->glShaderStorageBlockBinding(hermiteSecondShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i].verColID);










		ssbo_binding = 3;
		block_index = core->glGetProgramResourceIndex(hermiteSecondShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferTcoeffs");
		core->glShaderStorageBlockBinding(hermiteSecondShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i].TcoeffID);


		ssbo_binding = 4;
		block_index = core->glGetProgramResourceIndex(hermiteSecondShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferVertexNormals");
		core->glShaderStorageBlockBinding(hermiteSecondShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i].verNorID);

		ssbo_binding = 5;
		block_index = core->glGetProgramResourceIndex(hermiteSecondShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferLo");
		core->glShaderStorageBlockBinding(hermiteSecondShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i].loID);


		totalSizeSH = glm::uvec3(model.meshes[i].numV, 3, 1);
		groupsizeSh = glm::uvec3(64, 3, 1);
		groups = totalSizeSH / groupsizeSh;
		groups.x++;
		//groups = glm::uvec3(1, 1, 1);
		//core->glFinish();
		performCompute(groups);



		model.meshes[i].pushVerticesColor();



		core->glBindFramebuffer(GL_FRAMEBUFFER, m_oldFBO);

	}
}

void Prt_Zh::precompute(Shader shShader, Shader zhToShShader, Shader hermiteShader, PolygonalLight& sceneLight,model& model)
{
	if (dataExist == false)
	{
		prepareData(model, sceneLight);
		prepareBufferImage(model, sceneLight,DIFFUSE);
		dataExist = true;
	}

	//FILE* TFile = fopen("easymodel/asian_dragon_coeff.txt", "r");

	for (int i = 0; i < model.meshes.size(); i++)
	{


		memset(model.meshes[i].gZhgrad, 0.0f, 4 * 1836 * 512);

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

		glm::vec3 gridMin = glm::vec3(model.meshes[i].boundingBox.xmin, model.meshes[i].boundingBox.ymin, model.meshes[i].boundingBox.zmin);
		glm::uvec3 reso = glm::ivec3(8, 8, 8);
		glm::vec3 widthxyz = glm::vec3(model.meshes[i].boundingBox.widthx, model.meshes[i].boundingBox.widthy, model.meshes[i].boundingBox.widthz);
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

		//createTextures2D(textureParameters, &model.meshes[i].zhID, attchment, 512, 459);
		attchment++;


		addTexture(legendre_6789ID, 1);
		addTexture(model.meshes[i].zhID, 2);
		//addTexture()
		//transferToTexture1D(legendre_2345, legendre_2345ID, textureParameters1D, 10001);
		addTexture(legendre_2345ID, 0);


		//transferToTexture1D(legendre_6789, legendre_6789ID, textureParameters1D, 10001);
		transferToTexture2D(model.meshes[i].gZhgrad, model.meshes[i].zhID, textureParameters, 512, 459);






		/*core->glGenBuffers(1, &lightPosID);

		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightPosID);

		core->glBufferData(GL_SHADER_STORAGE_BUFFER, sceneLight.polygonalLight.verticesNum * 3 * sizeof(lightPos[0]), &lightPos[0], GL_DYNAMIC_DRAW);
		core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);*/

		//int k = 0;
		//for (int i = 0; i < sceneLight.polygonalLight.meshesNum; i++)
		//{
		//	for (int j = 0; j < sceneLight.polygonalLight.meshes[i].numV; j++)
		//	{
		//		lightPos[k * 3] = sceneLight.polygonalLight.meshes[i].vertexPos[j * 3];
		//		lightPos[k * 3 + 1] = sceneLight.polygonalLight.meshes[i].vertexPos[j * 3 + 1];
		//		lightPos[k * 3 + 2] = sceneLight.polygonalLight.meshes[i].vertexPos[j * 3 + 2];
		//		/*lightEmissive[k * 3] = light.polygonalLight.meshes[i].material.emissive[0];
		//		lightEmissive[k * 3 + 1] = light.polygonalLight.meshes[i].material.emissive[1];
		//		lightEmissive[k * 3 + 2] = light.polygonalLight.meshes[i].material.emissive[2];*/
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


		transferFromTexture2D(model.meshes[i].gZhgrad, textureParameters, 512, 459, GL_COLOR_ATTACHMENT2);





		//core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightEmissiveID);
		//core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sceneLight.polygonalLight.triangleNum  * sizeof(lightEmissive[0]), &lightEmissive[0]);

		//for (int j = 0; j < sceneLight.polygonalLight.triangleNum ; j++)
		//{
		//	qDebug() << lightEmissive[j];
		//}


	/*	for (int j = 512*4*0; j < 512 * 2 * 4; j++) 
		{
			qDebug() << model.meshes[i].gZhgrad[j];
		}
		qDebug() << "finish";*/


		zhToShShader.use();



		totalSizeSH = glm::uvec3(512, 3, 1);
		groupsizeSh = glm::uvec3(512, 1, 1);
		groups = totalSizeSH / groupsizeSh;


		//createTextures2D(textureParameters, &model.meshes[i].zhID, attchment, 512, 459);
		attchment++;
		addTexture(model.meshes[i].zhID, 0);
		//transferToTexture2D(model.meshes[i].gZhgrad, model.meshes[i].zhID, textureParameters, 512, 459);



		//createTextures2D(textureParameters, &model.meshes[i].shID, attchment, 512, 243);
		attchment++;
		addTexture(model.meshes[i].shID, 1);
		//transferToTexture2D(model.meshes[i].gShGrad, model.meshes[i].shID, textureParameters, 512, 243);

		performCompute(groups);
		//transferFromTexture2D(model.meshes[i].gShGrad, textureParameters, 512, 243, GL_COLOR_ATTACHMENT4);

		//for (int j = 0; j < 512*243*4 ; j++) {

		//	//qDebug() << model.meshes[i].gShGrad[j];
		//}
		//qDebug() << "finish";

		//attchment = GL_COLOR_ATTACHMENT5;
		hermiteShader.use();

		hermiteShader.setVec3("gridStep", gridstep);
		hermiteShader.setVec3("gridMin", gridMin);
		hermiteShader.setInt("numVertex", model.meshes[i].numV);
		hermiteShader.setuVec3("gridReso", reso);




		//createTextures2D(textureParameters, &model.meshes[i].shID, attchment, 512, 243);
		attchment++;
		addTexture(model.meshes[i].shID, 0);
		//transferToTexture2D(model.meshes[i].gShGrad, model.meshes[i].shID, textureParameters, 512, 243);


		/*createTextures2D(textureParameters, &meshes[i].verColID, attchment, 512, tempY);
		attchment++;
		addTexture(meshes[i].verColID, 2);
		transferToTexture2D(meshes[i].vertexColor, meshes[i].verColID, textureParameters, 512, tempY);


		int tempY2 = meshes[i].numV / 100 + 1;

		createTextures2D(textureParameters, &meshes[i].TcoeffID, attchment, 2700, tempY2);
		attchment++;
		addTexture(meshes[i].TcoeffID, 3);

		transferToTexture2D(Tcoff, meshes[i].TcoeffID, textureParameters, 2700, tempY2);*/



		//std::vector<glm::vec4> data = { glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec4(1.0, 0.0, 0.0, 1.0) };






		ssbo_binding = 4;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferPosition");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i].verPosID);







		ssbo_binding = 5;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferVertexColors");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i].verColID);










		ssbo_binding = 6;
		block_index = core->glGetProgramResourceIndex(hermiteShader.ID, GL_SHADER_STORAGE_BLOCK, "bufferTcoeffs");
		core->glShaderStorageBlockBinding(hermiteShader.ID, block_index, ssbo_binding);
		core->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding, model.meshes[i].TcoeffID);



		totalSizeSH = glm::uvec3(model.meshes[i].numV, 3, 1);
		groupsizeSh = glm::uvec3(64, 3, 1);
		groups = totalSizeSH / groupsizeSh;
		groups.x++;
		core->glFinish();
		performCompute(groups);

		// get GPU results
		/*core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
		core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, model.meshes[i].numV * 3 * sizeof(model.meshes[i].vertexPos[0]), model.meshes[i].vertexPos);*/
		/*core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.meshes[i].verColID);
		core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, model.meshes[i].numV * 3 * sizeof(model.meshes[i].vertexColor[0]), model.meshes[i].vertexColor);*/
		/*core->glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO2);
		core->glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, model.meshes[i].numV * 81 * sizeof(model.meshes[i].Tcoeff[0]), model.meshes[i].Tcoeff);*/

		//transferFromTexture2D(meshes[i].vertexColor, textureParameters, 512, tempY, GL_COLOR_ATTACHMENT7);

		/*for (int j = model.meshes[i].numV * 3-240; j < model.meshes[i].numV*3; j++) {

			qDebug() << model.meshes[i].vertexPos[j];
			qDebug() << model.meshes[i].vertexColor[j];
			qDebug() << model.meshes[i].Tcoeff[j*9];
		}
		qDebug() << "finish";*/
		model.meshes[i].pushVerticesColor();



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
		for (int j = 0; j < light.meshes[i].numV; j++)
		{
			float x = light.meshes[i].vertexPos[j * 3];
			float y = light.meshes[i].vertexPos[j * 3 + 1];
			float z = light.meshes[i].vertexPos[j * 3 + 2];
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