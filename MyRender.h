#pragma once
#include<QOpenGLShaderProgram>
#include<QOpenGLShader>
#include"timeCtrl.h"
#include"stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"camera.h"
#include"model.h"
#include<QOpenGLFunctions_4_3_Core>
#include"shader.h"
#include<QOpenGLFramebufferObject>
#include<QOpenGLTexture>
#include "PolygonalLight.h"
#include"Prt_Zh.h"
#include"ObjectBase.h"

class MyRender
{
public:
	void Draw();
	MyRender() {};
	MyRender(QOpenGLFunctions_4_3_Core *core)
	{
		this->core = core;
		model ourModel(core);
		ObjectBase* object = &ourModel;
		object->loadModel("dragon_and_bunny/dragon_and_bunny.obj");
		
		//智能指针
		//unique_ptr<ObjectBase> object1(new model(core));
		//object = object1.get();

		this->ourModel = ourModel;

		Prt_Zh tmpPrt(core);
		this->prtZh = tmpPrt;

		//light TmpLight(1, "easymodel/0.obj");
		//light TmpLight(1, "easymodel/green_side_0.obj");
		PolygonalLight TmpLight(core);
		object = &TmpLight;
		object->loadModel("dragon_and_bunny/light.obj");
		this->mylight = TmpLight;
		//animateDragonAndBunny();

		simpleDepthShader = new Shader("shader//simpleDepthShader.vert", "shader//simpleDepthShader.frag",core);
		//simpleDepthShader->use();
		pbrPcssShader = new Shader("shader//PbrVertex.vert", "shader//PbrFragment.frag",core);
		//debugDepthQuad = new Shader("shader//Debug.vert", "shader//Debug.frag", core);
		debugDepthQuad = new Shader("shader//Debug.vert", "shader//Debug.frag", core);
		ShShader = new Shader("shader//SH.comp", core);
		hermiteShader = new Shader("shader//Hermite.comp",core);
		prtShader = new Shader("shader//prtVertex.vert", "shader//prtFragment.frag", core);
		zhToShShader = new Shader("shader//zhToSh.comp", core);
		emitShader = new Shader("shader//emit.vert", "shader//emit.frag", core);




		hermiteGlossyShader = new Shader("shader//HermiteGlossy.comp", core);
		ShGlossyShader = new Shader("shader//SHGlossy.comp", core);
		zhToShGlossyShader = new Shader("shader//zhToShGlossy.comp", core);
		
		hermiteSecondGlossyShader=new Shader("shader//HermiteSecondGlossy.comp", core);

		core->glGenFramebuffers(1, &depthMapFBO);
		const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		GLint m_oldFBO;
		core->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_oldFBO);
		
		core->glGenTextures(1, &IshadowMap);
		core->glBindTexture(GL_TEXTURE_2D, IshadowMap);
		core->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



		core->glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		core->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, IshadowMap, 0);
		core->glDrawBuffer(GL_NONE);
		core->glReadBuffer(GL_NONE);


		//core->glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		//core->glGenTextures(1, &IshadowMap);
		//core->glBindTexture(GL_TEXTURE_2D, IshadowMap);
		//core->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 900, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		//core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//core->glBindTexture(GL_TEXTURE_2D, 0);

		//// 将它附加到当前绑定的帧缓冲对象
		//core->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, IshadowMap, 0);

		//
		//core->glGenRenderbuffers(1, &rbo);
		//core->glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		//core->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 900, 600);
		//core->glBindRenderbuffer(GL_RENDERBUFFER, 0);
		//core->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		//if (core->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		//	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		core->glBindFramebuffer(GL_FRAMEBUFFER,m_oldFBO);

		prtZh.precompute(*ShGlossyShader, *zhToShGlossyShader, *hermiteGlossyShader, *hermiteSecondGlossyShader, mylight, this->ourModel, cameraControl.cameraPos);
		//prtZh.precompute(*ShShader, *zhToShShader, *hermiteShader, mylight, this->ourModel);
		timer = timeCtrl();
	}
	float Fps=0.0;
	void ImportLight(QString path);
	void openFile(QString path);
	void setCameraPos(int front, int back, int left, int right);
	void setCameraFront(QPointF firstPoint, QPointF nextPoint);

	//灯光
	PolygonalLight mylight;
	//light mylight;
	timeCtrl timer;
private:
	QOpenGLFunctions_4_3_Core *core;
	Shader* simpleDepthShader;
	Shader* debugDepthQuad;
	Shader* pbrPcssShader;
	Shader* ShShader;
	Shader* hermiteShader;
	Shader* prtShader;
	Shader* zhToShGlossyShader;
	Shader* hermiteGlossyShader;
	Shader* ShGlossyShader;
	Shader* zhToShShader;
	Shader* emitShader;
	Shader*  hermiteSecondGlossyShader;
	GLuint depthMapFBO;
	GLuint depthCubemap;
	GLuint depthMap;
	GLuint outputTexID;
	GLuint intermediateTexID;
	GLuint inputTexID;
	
	Prt_Zh prtZh;
	camera cameraControl;
	unsigned int rbo;
	unsigned int quadVAO;
	unsigned int quadVBO;
	model ourModel;
	void shadowMap();
	void prtViewer(Shader shader);
	void DrawScene(Shader shader);
	void DrawPolLight(Shader shader);
	void renderQuad();
	void genShadowMap(Shader shader);
	void DrawAsianDragon();
	void DrawDragonAndBunny();
	void animateDragonAndBunny();
	void precompute();
	unsigned int IshadowMap;
	QOpenGLFramebufferObject* m_FrameBufferObj;
	float currentTime = 0.0f;
	float lastTime = 0.0f;
	float *pfInput;
	float *pfOutput;
	
	int iDebug = 0;
	bool lightAnimate = false;
};



