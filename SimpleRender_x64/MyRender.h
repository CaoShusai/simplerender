#pragma once
#include<QOpenGLShaderProgram>
#include<QOpenGLShader>
#include"timeCtrl.h"
#include"stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"CameraFreeMode.h"
#include"CameraCenterMode.h"
#include"model.h"
#include<QOpenGLFunctions_4_3_Core>
#include"shader.h"
#include<QOpenGLFramebufferObject>
#include<QOpenGLTexture>
#include "PolygonalLight.h"
#include"Prt_Zh.h"
#include"ObjectBase.h"
#include"ModelVetexSplit.h"
#include"SkyBox.h"
#include"PointLight.h"
#include<random>
#include"SceneRenderStatic.h"
using namespace std;
class MyRender
{
public:
	void Draw();
	MyRender() {};
	MyRender(QOpenGLFunctions_4_3_Core *core)
	{
		//初始化相机
		//默认是2模式，中心浏览模式
		this->camera_ptr=std::make_shared<CameraCenterMode>();
		CameraModeCur = 2;

		//初始化场景
		this->scene_ptr = std::make_shared<SceneRenderStatic>();


		
		this->core = core;








		//这些初始化模型要变成动态加入
		model ourModel(core);
		ObjectBase* object = &ourModel;
		object->loadModel("dragon_and_bunny/dragon_and_bunny.obj");
		//object->loadModel("dragon_and_bunny/111.obj");
		this->ourModel = ourModel;

		



		//prt内容先注释了
		




		ModelVetexSplit ourModelVetexSplit(core);
		ObjectBase* objectx = &ourModelVetexSplit;
		objectx->loadModel("dragon_and_bunny/dragon_and_bunny.obj");
		this->ourModelVetexSplit = ourModelVetexSplit;

		

		Prt_Zh tmpPrt(core);
		this->prtZh = tmpPrt;

		
		PolygonalLight TmpLight(core);
		object = &TmpLight;
		object->loadModel("dragon_and_bunny/light.obj");
		this->mylight = TmpLight;
		
		

		simpleDepthShader = new Shader("shader//simpleDepthShader.vert", "shader//simpleDepthShader.frag",core);
		pbrPcssShader = new Shader("shader//PbrVertex.vert", "shader//PbrFragment.frag",core);
		debugDepthQuad = new Shader("shader//Debug.vert", "shader//Debug.frag", core);

		ShShader = new Shader("shader//SH.comp", core);
		hermiteShader = new Shader("shader//Hermite.comp",core);
		prtShader = new Shader("shader//prtVertex.vert", "shader//prtFragment.frag", core);
		zhToShShader = new Shader("shader//zhToSh.comp", core);
		emitShader = new Shader("shader//emit.vert", "shader//emit.frag", core);

		simpleCubeDepthShader = new Shader("shader//shadowCube.vert", "shader//shadowCube.frag", core,"shader//shadowCube.geom");

		finalPbrPcssShader = new Shader("shader//finalPbrPcss.vert", "shader//finalPbrPcss.frag", core);

		hermiteGlossyShader = new Shader("shader//HermiteGlossy.comp", core);
		ShGlossyShader = new Shader("shader//SHGlossy.comp", core);
		zhToShGlossyShader = new Shader("shader//zhToShGlossy.comp", core);
		
		hermiteSecondGlossyShader=new Shader("shader//HermiteSecondGlossy.comp", core);
		TcoeffShader = new Shader("shader//Tcoeff.comp", core);

		skyBoxShader = new Shader("shader//skyBox.vert", "shader//skyBox.frag", core);

		GBufferShader = new Shader("shader//Gbuffer.vert", "shader//Gbuffer.frag", core);
		SSAOShader = new Shader("shader//SSAO.vert", "shader//SSAO.frag", core);
		computeSSAOShader = new Shader("shader//computeSSAO.vert", "shader//computeSSAO.frag", core);
		SimpleBlurShader= new Shader("shader//SimpleBlur.vert", "shader//SimpleBlur.frag", core);

		GLint m_oldFBO;
		core->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_oldFBO);

		core->glGenFramebuffers(1, &depthMapFBO);
		core->glGenFramebuffers(1, &depthCubeMapFBO);
		const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;


		
		core->glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		for (GLuint i = 0; i < 6; ++i)
			core->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
				SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		core->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		core->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		core->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		core->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		core->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		



		//SSAO
		int SCR_WIDTH = 1440;
		int SCR_HEIGHT = 960;
		core->glGenFramebuffers(1, &gBuffer);
		core->glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		

		// - 位置颜色缓冲
		core->glGenTextures(1, &gPosition);
		core->glBindTexture(GL_TEXTURE_2D, gPosition);
		core->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		core->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);


		
		
			// - 法线颜色缓冲
		core->glGenTextures(1, &gNormal);
		core->glBindTexture(GL_TEXTURE_2D, gNormal);
		core->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		core->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

		// - 颜色 + 镜面颜色缓冲
		core->glGenTextures(1, &gAlbedoSpec);
		core->glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		core->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		core->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

		// - 告诉OpenGL我们将要使用(帧缓冲的)哪种颜色附件来进行渲染
		GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		core->glDrawBuffers(3, attachments);

		
		core->glGenRenderbuffers(1, &rboDepth);
		core->glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		core->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
		core->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
		// - Finally check if framebuffer is complete
		if (core->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			qDebug() << "Framebuffer not complete!" << endl;



	







		
		
		core->glGenTextures(1, &IshadowMap);
		core->glBindTexture(GL_TEXTURE_2D, IshadowMap);
		core->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



		core->glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFBO);
		core->glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
		core->glDrawBuffer(GL_NONE);
		core->glReadBuffer(GL_NONE);
		if (core->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			qDebug() << "Framebuffer not complete!" << endl;
		

		core->glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		core->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, IshadowMap, 0);
		core->glDrawBuffer(GL_NONE);
		core->glReadBuffer(GL_NONE);



		
		core->glGenFramebuffers(1, &ssaoFBO);
		core->glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
		

		core->glGenTextures(1, &ssaoColorBuffer);
		core->glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		core->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		core->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

		
		core->glGenFramebuffers(1, &ssaoBlurFBO);
		core->glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
		core->glGenTextures(1, &ssaoColorBufferBlur);
		core->glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
		core->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		core->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		core->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);

		/*core->glDrawBuffer(GL_NONE);
		core->glReadBuffer(GL_NONE);*/


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





		prtZh.precompute(*ShGlossyShader, *zhToShGlossyShader, *hermiteGlossyShader, *hermiteSecondGlossyShader,*TcoeffShader, mylight, this->ourModel,this->ourModelVetexSplit, camera_ptr->cameraPos);


		timer = timeCtrl();
	}
	float Fps=0.0;
	void ImportLight(QString path);
	void Import(string path);

	void openFile(QString path);
	void setCameraPos(int front, int back, int left, int right);
	void setCameraFront(QPointF firstPoint, QPointF nextPoint);
	void setCameraMode(int flag);
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
	Shader* finalPbrPcssShader;
	Shader* emitShader;
	Shader* TcoeffShader;
	Shader* hermiteSecondGlossyShader;
	Shader* simpleCubeDepthShader;
	Shader* skyBoxShader;
	Shader* GBufferShader;
	Shader* computeSSAOShader;
	Shader* SSAOShader;
	Shader* SimpleBlurShader;


	GLuint ssaoBlurFBO, ssaoColorBufferBlur;
	GLuint ssaoFBO;
	GLuint ssaoColorBuffer;
	GLuint rboDepth;
	GLuint gBuffer;
	GLuint gPosition, gNormal, gColorSpec, gAlbedoSpec;
	GLuint depthMapFBO;
	GLuint depthCubemap;
	GLuint depthMap;
	GLuint outputTexID;
	GLuint intermediateTexID;
	//GLuint inputTexID;
	
	GLuint depthCubeMapFBO;

	Prt_Zh prtZh;
	//Camera* cameraControl;
	shared_ptr<Camera> camera_ptr;
	shared_ptr<SceneBase> scene_ptr;

	unsigned int rbo;
	unsigned int quadVAO;
	unsigned int quadVBO;
	unsigned int screenVAO;
	unsigned int screenVBO;
	model ourModel;
	ModelVetexSplit ourModelVetexSplit;
	void debugTexture(GLuint inputTexID);
	void prtViewer(Shader shader);
	void DrawScene(Shader shader);
	void DrawPolLight(Shader shader);
	void genShadowMap(Shader shader);

	void animateDragonAndBunny();
	void Blur(Shader shader,GLuint inputTexID);
	void precompute();
	void genShadowCubeMap(Shader shader);
	void DrawScene2(Shader shader);
	void DrawSkyBox(Shader shader);
	void preSSAO(Shader shader,  Shader computeSSAOShader);
	void excSSAO(Shader shader);
	void debugCubeTexture(Shader shader);
	
	unsigned int IshadowMap;
	QOpenGLFramebufferObject* m_FrameBufferObj;
	float currentTime = 0.0f;
	float lastTime = 0.0f;
	float *pfInput;
	float *pfOutput;
	
	int CameraModeCur;
	int iDebug = 0;
	bool lightAnimate = false;
};



