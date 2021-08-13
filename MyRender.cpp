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
	prtZh.precompute(*ShGlossyShader, *zhToShGlossyShader, *hermiteGlossyShader, *hermiteSecondGlossyShader, mylight, this->ourModel,cameraControl.cameraPos);
	//prtZh.precompute(*ShShader, *zhToShShader, *hermiteShader, mylight, this->ourModel);
}
void MyRender::precompute()
{
	prtZh.precompute(*ShGlossyShader, *zhToShGlossyShader, *hermiteGlossyShader, *hermiteSecondGlossyShader, mylight, this->ourModel, cameraControl.cameraPos);
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
	view = cameraControl.view;
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
	core->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	core->glClear(GL_COLOR_BUFFER_BIT);

	//genShadowMap(*simpleDepthShader);
	//shadowMap();
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
	view = cameraControl.view;
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
	cameraControl.translate(front, back, left, right);
}
void MyRender::setCameraFront(QPointF firstPoint, QPointF nextPoint)
{
	cameraControl.createRotation(firstPoint, nextPoint);
}

void MyRender::shadowMap()
{
	core->glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
	core->glClear(GL_COLOR_BUFFER_BIT);
	debugDepthQuad->use();
	debugDepthQuad->setInt("texture1",0);
	debugDepthQuad->setFloat("near_plane", 0.1f);
	debugDepthQuad->setFloat("far_plane", 50.0f);
	
	debugDepthQuad->setInt("texture1", 0);

	float quadVertices[] = {
		// positions        // texture Coords
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
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

	core->glActiveTexture(GL_TEXTURE0);
	core->glBindTexture(GL_TEXTURE_2D, inputTexID);
	
	
	core->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	core->glBindVertexArray(0);
	
	
}

void MyRender::DrawScene(Shader shader)
{
	shader.use();
	core->glViewport(0,0,900, 600);
	shader.setInt("depthMap",0);
	
	shader.setFloat("near_plane", 0.1f);
	shader.setFloat("far_plane", 50.0f);
	glm::mat4 model = glm::mat4(1.0);

	glm::mat4 view;
	view = cameraControl.view;
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
	QVector3D camPos = QVector3D(cameraControl.cameraPos.x, cameraControl.cameraPos.y, cameraControl.cameraPos.z);

	shaderProgram.setUniformValue(locationcamPos, camPos);*/
	shader.setVec3("albedo", glm::vec3(0.6, 0.3, 0.3));
	shader.setFloat("metallic", 0.1f);
	shader.setFloat("roughness", 0.5f);
	shader.setFloat("ao", 1.0f);
	shader.setVec3("camPos", cameraControl.cameraPos);
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

	core->glClearColor(1.0, 0.0, 1.0, 1.0);
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





