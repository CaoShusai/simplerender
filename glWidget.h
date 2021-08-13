#pragma once

#include <QWidget>
#include<QOpenGLFunctions_4_3_Core>
#include<QOpenGLShader>
#include<QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include<QTime>
#include"stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"camera.h"
#include"model.h"
#include<QFileDialog>
#include"MyRender.h"
#include"shader.h"
class glWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	glWidget(QWidget *parent = Q_NULLPTR);
	~glWidget();

private:
	QOpenGLFunctions_4_3_Core *core;
	QOpenGLShaderProgram shaderProgram;
	QOpenGLShaderProgram shaderProgram1;
	camera cameraControl;
	model ourModel;
	MyRender* myrender;
	bool drawFlag = false;
	bool openFlag=false;
	bool importLightFlag = false;
	QString fileName;
	QString lightFileName;
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
private slots:
	void mySlot2(int,int,int,int);
	void mySlotMosue(QPointF, QPointF);
	void mySlotOpenFile();
	void mySlotImportLight();
	void mySlotClickList(int);
signals:
	void mySignalAddList(int);
	void mySignalShowFps(float);
};
