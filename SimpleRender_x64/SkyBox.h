#pragma once
#include<QOpenGLFunctions_4_3_Core>
#include<QOpenGLShader>
#include<QOpenGLShaderProgram>
#include"shader.h"
class SkyBox
{
public:
	void createSkyBox(QOpenGLFunctions_4_3_Core *core);
	void Draw(Shader shader);
	float* vertices;
	unsigned int VAO, VBO, EBO;
	QOpenGLFunctions_4_3_Core *core;

};

