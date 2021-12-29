#include "glWidget.h"
#include <QMessageBox>
#include<Qmath.h>
#include<Qdebug>
#include<FileSystem>
glWidget::glWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	myrender = NULL;
}

glWidget::~glWidget()
{
}

void glWidget::initializeGL()
{
	core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
	myrender = new MyRender(core);//MyRender default load the sphere1.obj file
	emit  mySignalAddList(myrender->mylight.verticesNum/3);
	//myrender->openFile("easymodel/sphere.obj");
	drawFlag = true;
}

void glWidget::paintGL()
{
	if (drawFlag==false)
	{
		return;
	}
	if (openFlag==true)
	{
		myrender->openFile(fileName);
		openFlag = false;
	}
	myrender->Draw();
	emit mySignalShowFps(myrender->timer.getFps());
	update();
}

void glWidget::resizeGL(int w, int h)
{
	
}


void glWidget::mySlot2(int front, int back,int left,int right)
{
	myrender->setCameraPos(front, back, left, right);
}

void glWidget::mySlotMosue(QPointF firstPoint, QPointF nextPoint)
{
	myrender->setCameraFront(firstPoint, nextPoint);
}

void glWidget::mySlotOpenFile()
{
	
	drawFlag = false;
	//myrender = new MyRender(core);
	fileName = QFileDialog::getOpenFileName(NULL, "标题", ".");

	/*if (fileName!="")
	{
		myrender->openFile(fileName);
	}
	core->glClearColor(0.6f, 0.2f, 0.2f, 1.0f);*/
	openFlag = true;
	drawFlag = true;
	//this->doneCurrent();
	//update();
}
void glWidget::mySlotImportLight()
{
	int a = 5;
	lightFileName = QFileDialog::getOpenFileName(NULL, "标题", ".");
	//myrender->ImportLight();
}

void glWidget::mySlotClickList(int index)
{
	myrender->mylight.currentLightIndex = index;

}

//保存场景文件
void glWidget::mySlotSaveFile()
{

}

//加载资源
void glWidget::mySlotImport()
{

}

void glWidget::mySlotCameraFreeMode()
{
	myrender->setCameraMode(1);
}

void glWidget::mySlotCameraCenterMode()
{
	myrender->setCameraMode(2);
}