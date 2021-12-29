#include "MyRender.h"


void MyRender::openFile(QString path)
{
	char *ch;
	QByteArray ba = path.toLatin1();
	ch = ba.data();


	model ourModel(core);
	ourModel.loadModel(ch);
	this->ourModel = ourModel;

	prtZh.dataExist = false;
	prtZh.precompute(*ShGlossyShader, *zhToShGlossyShader, *hermiteGlossyShader, *hermiteSecondGlossyShader, *TcoeffShader, mylight, this->ourModel,this->ourModelVetexSplit,camera_ptr->cameraPos);
	//prtZh.precompute(*ShShader, *zhToShShader, *hermiteShader, mylight, this->ourModel);
}


void MyRender::precompute()
{
	prtZh.precompute(*ShGlossyShader, *zhToShGlossyShader, *hermiteGlossyShader, *hermiteSecondGlossyShader, *TcoeffShader, mylight, this->ourModel, this->ourModelVetexSplit, camera_ptr->cameraPos);
	//prtZh.precompute(*ShShader, *zhToShShader, *hermiteShader, mylight, this->ourModel);
}

void MyRender::ImportLight(QString path)
{
	/*light TmpLight(core);
	this->mylight = TmpLight;*/
}

void MyRender::DrawPolLight(Shader shader)
{
	shader.use();
	glm::mat4 model = glm::mat4(1.0);
	
	/*if (this->lightAnimate==true)
	{

	}*/
	model = glm::rotate(model,(float)glm::radians(this->currentTime*10.0), glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, (float)glm::radians(180.0), glm::vec3(1.0f, 0.0f, 0.0f));


	glm::mat4 view;
	view = camera_ptr->getView();
	glm::mat4 projection;

	projection = glm::perspective(glm::radians(45.0f), (float)1.5, 0.1f, 50.0f);
	//core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	core->glEnable(GL_DEPTH_TEST);
	core->glDepthFunc(GL_LESS);
	//core->glDisable(GL_CULL_FACE);
	

	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	mylight.Draw(shader);
}

void MyRender::Draw()
{
	//csTest(*ShShader);
	core->glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	core->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


	/*preSSAO(*GBufferShader,*computeSSAOShader);
	
	Blur(*SimpleBlurShader, ssaoColorBuffer);
	debugTexture(ssaoColorBufferBlur);*/
	//excSSAO(*SSAOShader);

	//cubemap点光源
	//genShadowCubeMap(*simpleCubeDepthShader);
	//DrawScene2(*finalPbrPcssShader);
	//DrawSkyBox(*skyBoxShader);
	



	//genShadowMap(*simpleDepthShader);
	////shadowMap();
	//DrawScene(*pbrPcssShader);


	animateDragonAndBunny();
	prtViewer(*prtShader);
	DrawPolLight(*emitShader);


	/*if (iDebug==0)
	{
		animateDragonAndBunny();
		iDebug++;
	}*/
	//Fps = timer.getFps();
	
}

void MyRender::prtViewer(Shader shader)
{
	precompute();
	shader.use();
	
	glm::mat4 model = glm::mat4(1.0);

	glm::mat4 view;
	view = camera_ptr->getView();
	glm::mat4 projection;

	projection = glm::perspective(glm::radians(45.0f), (float)1.5, 0.1f, 50.0f);
	//core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	core->glClear(GL_DEPTH_BUFFER_BIT);
	core->glEnable(GL_DEPTH_TEST);
	core->glDepthFunc(GL_LESS);
	//core->glEnable(GL_CULL_FACE);

	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	ourModel.Draw(shader);
	//core->glDeleteTextures(1, &IshadowMap);
}


void MyRender::setCameraPos(int front, int back, int left, int right)
{
	camera_ptr->CameraKeyboardCrtl(front, back, left, right);
}
void MyRender::setCameraFront(QPointF firstPoint, QPointF nextPoint)
{
	camera_ptr->CameraMouseCrtl(firstPoint, nextPoint);
	
}

void MyRender::debugTexture(GLuint inputTexID)
{
	core->glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
	core->glClear(GL_COLOR_BUFFER_BIT);
	debugDepthQuad->use();
	debugDepthQuad->setInt("texture1",0);
	debugDepthQuad->setFloat("near_plane", 0.1f);
	debugDepthQuad->setFloat("far_plane", 50.0f);
	
	debugDepthQuad->setInt("texture1", 2);

	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	// setup plane VAO


	core->glGenVertexArrays(1, &quadVAO);
	core->glGenBuffers(1, &quadVBO);
	core->glBindVertexArray(quadVAO);
	core->glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	core->glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	core->glEnableVertexAttribArray(0);
	core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	core->glEnableVertexAttribArray(1);
	core->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	core->glActiveTexture(GL_TEXTURE2);
	core->glBindTexture(GL_TEXTURE_2D, inputTexID);
	
	
	core->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	core->glBindVertexArray(0);
	
	
}

void MyRender::DrawScene(Shader shader)
{
	shader.use();
	core->glViewport(0,0,1440, 960);
	shader.setInt("depthMap",0);
	
	shader.setFloat("near_plane", 0.1f);
	shader.setFloat("far_plane", 50.0f);
	glm::mat4 model = glm::mat4(1.0);

	glm::mat4 view;
	view = camera_ptr->getView();
	glm::mat4 projection;

	projection = glm::perspective(glm::radians(45.0f), (float)1.5, 0.1f, 50.0f);
	//core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	core->glClear(GL_DEPTH_BUFFER_BIT);
	core->glEnable(GL_DEPTH_TEST);
	core->glDepthFunc(GL_LESS);

	
	/*int modelLoc = shaderProgram.uniformLocation();
	core->glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));*/
	shader.setMat4("model", model);
	/*int viewLoc = shaderProgram.uniformLocation("view");
	core->glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));*/
	shader.setMat4("view", view);
	/*int perLoc = shaderProgram.uniformLocation("projection");
	core->glUniformMatrix4fv(perLoc, 1, GL_FALSE, glm::value_ptr(projection));*/
	shader.setMat4("projection", projection);

	GLfloat near_plane = 0.1f, far_plane = 50.0f;
	//glm::mat4 projection;

	

	/*int locationLightSpace = shaderProgram.uniformLocation("lightSpaceMatrix");
	core->glUniformMatrix4fv(locationLightSpace, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));*/

	/*QVector3D lightPositions[] = {
	   QVector3D(0.0f,  10.0f, 15.0f),
	   QVector3D(10.0f,  10.0f, 15.0f),
	   QVector3D(-10.0f, 10.0f, 15.0f),
	   QVector3D(0.0f, 5.0f, 10.0f),
	};*/

	glm::vec3 lightPositions[] = {
	   glm::vec3(0.0f,  18.0f, 5.0f),
	   glm::vec3(10.0f,  20.0f, 5.0f),
	   glm::vec3(0.0f, 22.0f, 6.0f),
	  glm::vec3(0.0f, 17.0f, 4.0f),
	};

	/*QVector3D lightColors[] = {
		QVector3D(1000.0f, 1000.0f, 1000.0f),
		QVector3D(1000.0f, 1000.0f, 1000.0f),
		QVector3D(1000.0f, 1000.0f, 1000.0f),
		QVector3D(1000.0f, 1000.0f, 1000.0f)
	};*/

	glm::vec3 lightColors[] = {
		glm::vec3(1000.0f, 1000.0f, 1000.0f),
		glm::vec3(1000.0f, 1000.0f, 1000.0f),
		glm::vec3(1000.0f, 1000.0f, 1000.0f),
		glm::vec3(1000.0f, 1000.0f, 1000.0f)
	};
	for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); i++)
	{
		/*string strtmp = "lightPositions[" + std::to_string(i) + "]";
		const char* ch = strtmp.c_str();
		int locationPos = shaderProgram.uniformLocation(ch);
		int locationCol = shaderProgram.uniformLocation(("lightColors[" + std::to_string(i) + "]").c_str());
		shaderProgram.setUniformValue(locationPos, lightPositions[i]);
		shaderProgram.setUniformValue(locationCol, lightColors[i]);*/

		shader.setVec3(("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);
		shader.setVec3(("lightPositions[" + std::to_string(i) + "]").c_str(), lightPositions[i]);
	}
	/*int locationAlbedo = shaderProgram.uniformLocation("albedo");
	int locationMetallic = shaderProgram.uniformLocation("metallic");
	int locationRoughness = shaderProgram.uniformLocation("roughness");
	int locationAo = shaderProgram.uniformLocation("ao");
	shaderProgram.setUniformValue(locationAlbedo, QVector3D(0.6, 0.3, 0.3));
	shaderProgram.setUniformValue(locationMetallic, 0.1f);
	shaderProgram.setUniformValue(locationRoughness, 0.5f);
	shaderProgram.setUniformValue(locationAo, 1.0f);
	int locationcamPos = shaderProgram.uniformLocation("camPos");
	QVector3D camPos = QVector3D(cameraControl->cameraPos.x, cameraControl->cameraPos.y, cameraControl->cameraPos.z);

	shaderProgram.setUniformValue(locationcamPos, camPos);*/
	shader.setVec3("albedo", glm::vec3(0.6, 0.3, 0.3));
	shader.setFloat("metallic", 0.1f);
	shader.setFloat("roughness", 0.5f);
	shader.setFloat("ao", 1.0f);
	shader.setVec3("camPos", camera_ptr->cameraPos);
	glm::mat4 projection2 = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 50.0f);
	//glm::mat4 projection2 = glm::perspective(glm::radians(45.0f), (float)1.5, 0.1f, 50.0f);
	glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f, 20.0f, 5.0f), glm::vec3(0.0f, 19.0f, 5.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = projection2 * lightView;
	shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	core->glActiveTexture(GL_TEXTURE0);
	core->glBindTexture(GL_TEXTURE_2D, IshadowMap);
	ourModel.Draw(shader);
	//core->glDeleteTextures(1, &IshadowMap);
	
}

void MyRender::genShadowMap(Shader shader)
{
	core->glViewport(0, 0,1024, 1024);
	GLint m_oldFBO;
	core->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_oldFBO);
	core->glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//bool result = m_FrameBufferObj->bind();

	
	core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	



	shader.use();



	core->glClear(GL_DEPTH_BUFFER_BIT);
	core->glEnable(GL_DEPTH_TEST);
	core->glDepthFunc(GL_LESS);
	glm::mat4 model = glm::mat4(1.0);
	shader.setMat4("model", model);
	glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 50.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1.5, 0.1f, 50.0f);
	glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f, 20.0f, 5.0f), glm::vec3(0.0f, 19.0f, 5.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = projection * lightView;

	


	shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	ourModel.Draw(*simpleDepthShader);


	/*IshadowMap = m_FrameBufferObj->texture();
	QImage fboImage(m_FrameBufferObj->toImage());
	QImage image(fboImage.constBits(), fboImage.width(), fboImage.height(), QImage::Format_ARGB32);
	QPixmap::fromImage(image).save("result2.jpg", "jpg");
	m_FrameBufferObj->release();*/
	
	core->glBindFramebuffer(GL_FRAMEBUFFER, m_oldFBO);
}

void MyRender::animateDragonAndBunny()
{
	this->currentTime = timer.getTime();
	prtZh.updateLightPos(currentTime*10,glm::vec3(1.0,0.0,0.0),mylight);
	
}


void MyRender::genShadowCubeMap(Shader shader)
{
	core->glViewport(0, 0, 1024, 1024);
	GLint m_oldFBO;
	core->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_oldFBO);
	core->glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFBO);
	//bool result = m_FrameBufferObj->bind();


	shader.use();


	core->glClear(GL_DEPTH_BUFFER_BIT);
	core->glEnable(GL_DEPTH_TEST);
	core->glDepthFunc(GL_LESS);
	glm::mat4 model = glm::mat4(1.0);
	shader.setMat4("model", model);
	
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)1, 0.1f, 50.0f);
	


	glm::vec3 lightPos = glm::vec3(2.0f, 5.0f, 5.0f);
	glm::mat4 shadowProj = projection;
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	for (GLuint i = 0; i < 6; ++i)
	    shader.setMat4(("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowTransforms[i]);
	
	shader.setFloat("far_plane", 50.0f);
	shader.setVec3("lightPos", lightPos);
	

	//shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	ourModel.Draw(shader);


	/*IshadowMap = m_FrameBufferObj->texture();
	QImage fboImage(m_FrameBufferObj->toImage());
	QImage image(fboImage.constBits(), fboImage.width(), fboImage.height(), QImage::Format_ARGB32);
	QPixmap::fromImage(image).save("result2.jpg", "jpg");
	m_FrameBufferObj->release();*/

	core->glBindFramebuffer(GL_FRAMEBUFFER, m_oldFBO);
}

//with finalPbrPcss
void MyRender::DrawScene2(Shader shader)
{
	shader.use();
	core->glViewport(0, 0, 1440, 960);
	shader.setInt("depthMap", 0);

	shader.setFloat("near_plane", 0.1f);
	shader.setFloat("far_plane", 50.0f);
	glm::mat4 model = glm::mat4(1.0);

	glm::mat4 view;
	view = camera_ptr->getView();
	glm::mat4 projection;

	projection = glm::perspective(glm::radians(90.0f), (float)1.5, 0.1f, 50.0f);
	core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	core->glEnable(GL_DEPTH_TEST);
	core->glDepthFunc(GL_LESS);



	shader.setMat4("model", model);

	shader.setMat4("view", view);

	shader.setMat4("projection", projection);



	glm::vec3 lightPositions[] = {
	   glm::vec3(2.0f, 5.0f, 5.0f),
	   glm::vec3(10.0f,  20.0f, 5.0f),
	   glm::vec3(0.0f, 22.0f, 6.0f),
	  glm::vec3(0.0f, 17.0f, 4.0f),
	};

	

	glm::vec3 lightColors[] = {
		glm::vec3(1000.0f, 1000.0f, 1000.0f),
		glm::vec3(1000.0f, 1000.0f, 1000.0f),
		glm::vec3(1000.0f, 1000.0f, 1000.0f),
		glm::vec3(1000.0f, 1000.0f, 1000.0f)
	};
	for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); i++)
	{

		shader.setVec3(("lightColors[" + std::to_string(i) + "]").c_str(), lightColors[i]);
		shader.setVec3(("lightPositions[" + std::to_string(i) + "]").c_str(), lightPositions[i]);
	}

	//glm::vec3
	shader.setVec3("albedo", glm::vec3(0.6f, 0.2f, 0.2f));
	
	shader.setVec3("viewPos", camera_ptr->cameraPos);
	
	shader.setFloat("metallic", 0.1f);
	shader.setFloat("roughness", 0.5f);
	shader.setFloat("ao", 1.0f);


	

	core->glActiveTexture(GL_TEXTURE0);
	core->glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	ourModel.Draw(shader);
	//core->glDeleteTextures(1, &IshadowMap);

}



void MyRender::debugCubeTexture(Shader shader)
{
	
}

void MyRender::DrawSkyBox(Shader shader)
{
	shader.use();
	//core->glClear(GL_DEPTH_BUFFER_BIT);
	shader.setInt("skybox", 0);
	core->glViewport(0, 0, 1440, 960);
	core->glEnable(GL_DEPTH_TEST);
	core->glDepthFunc(GL_LEQUAL);
	SkyBox skyBox;
	skyBox.createSkyBox(core);

	
	core->glActiveTexture(GL_TEXTURE0);
	core->glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	glm::mat4 view = glm::mat4(glm::mat3(camera_ptr->getView()));
	shader.setMat4("view", view);
	glm::mat4x4 projection = glm::perspective(glm::radians(90.0f), (float)1.5, 0.1f, 50.0f);
	shader.setMat4("projection", projection);
	skyBox.Draw(shader);


	core->glDepthFunc(GL_LESS);
}

GLfloat lerp(GLfloat a, GLfloat b, GLfloat f)
{
	return a + f * (b - a);
}

void MyRender::preSSAO(Shader shader,Shader computeSSAOShader)
{
	














	GLint m_oldFBO;
	core->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_oldFBO);
	core->glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	
	core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	core->glEnable(GL_DEPTH_TEST);
	core->glDepthFunc(GL_LESS);
	core->glDepthMask(GL_TRUE);
	shader.use();
	glm::mat4 model = glm::mat4(1.0);

	glm::mat4 view;
	view = camera_ptr->getView();
	glm::mat4 projection;

	projection = glm::perspective(glm::radians(90.0f), (float)1.5, 0.1f, 50.0f);
	

	


	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	

	ourModel.Draw(shader);
	
	core->glBindFramebuffer(GL_FRAMEBUFFER, m_oldFBO);




	






	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // 随机浮点数，范围0.0 - 1.0
	std::default_random_engine generator;
	std::vector<glm::vec3> ssaoKernel;
	for (GLuint i = 0; i < 64; ++i)
	{
		glm::vec3 sample(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator)
		);
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		GLfloat scale = GLfloat(i) / 64.0;
		ssaoKernel.push_back(sample);
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}
	std::vector<glm::vec3> ssaoNoise;
	for (GLuint i = 0; i < 16; i++)
	{
		glm::vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
		ssaoNoise.push_back(noise);
	}

	GLuint noiseTexture;
	core->glGenTextures(1, &noiseTexture);
	core->glBindTexture(GL_TEXTURE_2D, noiseTexture);
	core->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	


	// 使用G缓冲渲染SSAO纹理
	core->glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	core->glEnable(GL_DEPTH_TEST);
	core->glDepthFunc(GL_LESS);

	computeSSAOShader.use();

	computeSSAOShader.setInt("gPosition", 0);
	computeSSAOShader.setInt("gNormal", 1);
	computeSSAOShader.setInt("texNoise", 2);
	computeSSAOShader.setInt("ssao", 3);
	core->glActiveTexture(GL_TEXTURE0);
	core->glBindTexture(GL_TEXTURE_2D, gPosition);
	core->glActiveTexture(GL_TEXTURE1);
	core->glBindTexture(GL_TEXTURE_2D, gNormal);
	core->glActiveTexture(GL_TEXTURE2);
	core->glBindTexture(GL_TEXTURE_2D, noiseTexture);
	core->glActiveTexture(GL_TEXTURE2);
	core->glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);

	
	
	


	computeSSAOShader.setMat4("model", model);
	computeSSAOShader.setMat4("view", view);
	computeSSAOShader.setMat4("projection", projection);
	
	for (int i = 0; i < 64; i++)
	{
		computeSSAOShader.setVec3("samples["+ std::to_string(i) + "]", ssaoKernel[i]);
	}
	
	ourModel.Draw(computeSSAOShader);


	core->glBindFramebuffer(GL_FRAMEBUFFER, m_oldFBO);

}


void MyRender::Blur(Shader shader, GLuint inputTexID)
{
	GLint m_oldFBO;
	core->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_oldFBO);
	core->glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	core->glClear(GL_COLOR_BUFFER_BIT);
	shader.use();
	core->glActiveTexture(GL_TEXTURE0);
	core->glBindTexture(GL_TEXTURE_2D, inputTexID);
	glm::mat4 model = glm::mat4(1.0);

	glm::mat4 view;
	view = camera_ptr->getView();
	glm::mat4 projection;

	projection = glm::perspective(glm::radians(90.0f), (float)1.5, 0.1f, 50.0f);

	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	ourModel.Draw(shader);

	core->glBindFramebuffer(GL_FRAMEBUFFER, m_oldFBO);
}

void MyRender::excSSAO(Shader shader)
{
	GLint m_oldFBO;
	GLuint  SCR_WIDTH = 1440;
	GLuint SCR_HEIGHT = 960;
	core->glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	core->glEnable(GL_DEPTH_TEST);
	core->glDepthFunc(GL_LESS);

	//core->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_oldFBO);


	//core->glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
	//core->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_oldFBO); // Write to default framebuffer
	//core->glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	//core->glBindFramebuffer(GL_FRAMEBUFFER, m_oldFBO);

	/*if (core->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		qDebug() << "Framebuffer not complete!" << endl;*/

	shader.use();

	glm::mat4 model = glm::mat4(1.0);
	glm::mat4 view;
	view = camera_ptr->getView();
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(90.0f), (float)1.5, 0.1f, 50.0f);
	


	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("viewPos", camera_ptr->cameraPos);
	int NR_LIGHTS = 32;
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
	srand(13);
	for (GLuint i = 0; i < NR_LIGHTS; i++)
	{
		// Calculate slightly random offsets
		GLfloat xPos = ((rand() % 100) / 100.0) * 6.0 - 0.0;
		GLfloat yPos = ((rand() % 100) / 100.0) * 6.0 - 0.0;
		GLfloat zPos = ((rand() % 100) / 100.0) * 6.0 - 0.0;
		lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		// Also calculate random color
		GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
		GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
		GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
		lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}
	

	for (GLuint i = 0; i < lightPositions.size(); i++)
	{
		shader.setVec3(("lights[" + std::to_string(i) + "].Position").c_str(), lightPositions[i]);
		shader.setVec3(("lights[" + std::to_string(i) + "].Color").c_str(), lightColors[i]);
	}

	shader.setInt("gPosition", 0);
	shader.setInt("gNormal", 1);
	shader.setInt("gAlbedoSpec", 2);

	core->glActiveTexture(GL_TEXTURE0);
	core->glBindTexture(GL_TEXTURE_2D, gPosition);
	core->glActiveTexture(GL_TEXTURE1);
	core->glBindTexture(GL_TEXTURE_2D, gNormal);
	core->glActiveTexture(GL_TEXTURE2);
	core->glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);

	ourModel.Draw(shader);

}

void MyRender::setCameraMode(int flag)
{
	//模式不变
	if (flag == CameraModeCur)
		return;
	else
		CameraModeCur = flag;
	if (flag == 1)
	{
		
		camera_ptr.reset(new CameraFreeMode());
	}
	else if(flag==2)
	{
		camera_ptr.reset(new CameraCenterMode());
	}
}