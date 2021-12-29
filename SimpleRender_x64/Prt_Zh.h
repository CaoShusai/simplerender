#pragma once
#include"shader.h"
#include "PolygonalLight.h"
#include"model.h"
#include"ModelVetexSplit.h"
#include"RayTracing.h"
#define DIFFUSE 0
#define GLOSSY 1
class Prt_Zh
{
public:
	Prt_Zh() {};
	Prt_Zh(QOpenGLFunctions_4_3_Core *core)
	{
		dataExist = false;
		//this->core = core;
		this->core =core;
		int legendreReso = 10000;
		legendre_2345 = new float[(legendreReso + 1) * 4];
		legendre_6789 = new float[(legendreReso + 1) * 4];

		for (int i = 0; i <= legendreReso; i++)
		{
			float X[10];
			X[1] = (float)i / legendreReso;
			for (int j = 2; j < 10; j++)
				X[j] = X[j - 1] * X[1];

			legendre_2345[i * 4 + 0] = (0.5f * (3 * X[2] - 1));
			legendre_2345[i * 4 + 1] = (0.5f * (5 * X[3] - 3 * X[1]));
			legendre_2345[i * 4 + 2] = ((35 * X[4] - 30 * X[2] + 3) / 8.0f);
			legendre_2345[i * 4 + 3] = ((63 * X[5] - 70 * X[3] + 15 * X[1]) / 8.0f);

			legendre_6789[i * 4 + 0] = ((231 * X[6] - 315 * X[4] + 105 * X[2] - 5) / 16.0f);
			legendre_6789[i * 4 + 1] = ((429 * X[7] - 693 * X[5] + 315 * X[3] - 35 * X[1]) / 16.0f);
			legendre_6789[i * 4 + 2] = ((6435 * X[8] - 12012 * X[6] + 6930 * X[4] - 1260 * X[2] + 35) / 128.0f);
			legendre_6789[i * 4 + 3] = ((12155 * X[9] - 25740 * X[7] + 18018 * X[5] - 4620 * X[3] + 315 * X[1]) / 128.0f);
		}
	}
	bool dataExist;
	void computeT(model& model, ModelVetexSplit& modelSplit, Shader TcoeffShader);
	void prepareData(model& model, ModelVetexSplit& modelSplit, PolygonalLight& light, Shader TcoeffShader);
	void precompute(Shader ShShader, Shader zhToShShader, Shader hermiteShader, Shader TcoeffShader,PolygonalLight& sceneLight, model& model, ModelVetexSplit& modelSplit);
	void precompute(Shader shShader, Shader zhToShShader, Shader hermiteShader,Shader hermiteSecondShader, Shader TcoeffShader,PolygonalLight& sceneLight, model& model, ModelVetexSplit& modelSplit,glm::vec3 cameraPos);
	void updateLightPos(float angle, glm::vec3 dir, PolygonalLight& light);
private:
	

	QOpenGLFunctions_4_3_Core *core;
	GLuint prtFbo;
	GLuint legendre_2345ID;
	GLuint legendre_6789ID;
	GLuint lightPosID;
	GLuint lightEmissiveID;
	GLuint vertexPosID;
	GLuint vertexColID;
	GLuint vertexTcoeffID;

	void addTextureReadOnly(const GLuint ID, const GLuint bind);
	void prepareBufferImage(model& model, PolygonalLight& sceneLight, int MaterialFlag);
	void setupTexture2D(const GLuint texID, structTextureParameters textureParameters, GLuint width, GLuint height);
	void createTextures2D(structTextureParameters textureParameters, GLuint* ID, GLuint attchment, GLuint width, GLuint height);
	void transferToTexture1D(float *data, GLuint texID, structTextureParameters textureParameters, GLuint size);

	void performCompute(glm::uvec3 group);
	void transferFromTexture2D(float *data, structTextureParameters textureParameters, GLuint width, GLuint height, GLuint attchment);

	void addTexture(const GLuint ID, const GLuint bind);
	void setupTexture1D(const GLuint texID, structTextureParameters textureParameters, GLuint size);
	void createTextures1D(structTextureParameters textureParameters, GLuint* ID, GLuint attachment, GLuint size);
	void transferToTexture2D(float *data, GLuint texID, structTextureParameters textureParameters, GLuint width, GLuint height);

	
	float *legendre_2345;
	float *legendre_6789;
	float *lightPos;
	float *lightEmissive;
	//float *lightNormal;
};

