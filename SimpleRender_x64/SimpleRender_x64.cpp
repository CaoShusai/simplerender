#include "SimpleRender_x64.h"

SimpleRender_x64::SimpleRender_x64(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	connect(this, SIGNAL(mySignalParam(int, int, int, int)), ui.openGLWidget, SLOT(mySlot2(int, int, int, int)));
	connect(this, SIGNAL(mySignalMouse(QPointF, QPointF)), ui.openGLWidget, SLOT(mySlotMosue(QPointF, QPointF)));

	connect(ui.openGLWidget, SIGNAL(mySignalAddList(int)), this, SLOT(mySlotAddList(int)));
	connect(ui.openGLWidget, SIGNAL(mySignalShowFps(float)), this, SLOT(mySlotShowFps(float)));
	//读写
	connect(ui.actionOpen, SIGNAL(triggered()), ui.openGLWidget, SLOT(mySlotOpenFile()));
	connect(ui.actionSave, SIGNAL(triggered()), ui.openGLWidget, SLOT(mySlotSaveFile()));
	connect(ui.actionImport, SIGNAL(triggered()), ui.openGLWidget, SLOT(mySlotImport()));
	//相机模式
	connect(ui.actionFreeMode, SIGNAL(triggered()), ui.openGLWidget, SLOT(mySlotCameraFreeMode()));
	connect(ui.actionCenterMode, SIGNAL(triggered()), ui.openGLWidget, SLOT(mySlotCameraCenterMode()));

	connect(ui.actionImportPolLight, SIGNAL(triggered()), ui.openGLWidget, SLOT(mySlotImportLight()));
	connect(ui.listWidget,SIGNAL(triggered()),this,SLOT(on_listWidget_clicked(const QModelIndex &index)));
	connect(this, SIGNAL(mySignalListIndex(int)), ui.openGLWidget, SLOT(mySlotClickList(int)));


	
	glWidth = ui.openGLWidget->width();
	glHeight = ui.openGLWidget->height();
	int left = ui.openGLWidget->rect().x();
	int top = ui.openGLWidget->rect().top();
	int right = ui.openGLWidget->rect().right();
	
	
	

	leftMouseButtonActive = false;
}

void SimpleRender_x64::mySlotShowFps(float FPS)
{
	string str = to_string(FPS);
	ui.lineEdit->setText(QString::fromStdString(str));
}

void SimpleRender_x64::keyPressEvent(QKeyEvent *event) {
	float front, back, left, right;
	switch (event->key())
	{
	case Qt::Key_W:
		qDebug() << "W";
		emit mySignalParam(1, 0, 0, 0);
		break;
	case Qt::Key_S:
		qDebug() << "S";
		emit mySignalParam(0, 1, 0, 0);
		break;
	case Qt::Key_A:
		qDebug() << "A";
		emit mySignalParam(0, 0, 1, 0);
		break;
	case Qt::Key_D:
		qDebug() << "D";
		emit mySignalParam(0, 0, 0, 1);
		break;
	}
}

void SimpleRender_x64::mouseMoveEvent(QMouseEvent* event)
{

	this->firstPoint = this->nextPoint;
	this->nextPoint.setX((event->pos().x() - 100) / glWidth - 0.5);
	this->nextPoint.setY((glHeight - event->pos().y() + 20) / glHeight - 0.5);
	//this->nextPoint.x= ;
	//this->nextPoint.y = (event->pos().y - 20)/glHeight-0.5;
	if (!leftMouseButtonActive)
	{
		leftMouseButtonActive = true;
		return;
	}
	emit mySignalMouse(firstPoint, nextPoint);
}

void SimpleRender_x64::mouseReleaseEvent(QMouseEvent * event)
{
	leftMouseButtonActive = false;
	//firstPoint.setX(0);
	//firstPoint.setY(0);
}

void SimpleRender_x64::mySlotAddList(int lightNum)
{
	for (int i = 0; i < lightNum; i++)
	{
		string numStr = to_string(i);
		
		QStringList list;
		list << tr(numStr.c_str());//将内容"设备2"赋值给列表第一行。后面还可以赋值给列表的第二行，例如：list<<tr("设备2")<<tr("设备3")
		ui.listWidget->insertItems(i+1, list);//在ListWidget控件的第1行插入一个列表list内容
	}
	
}

void SimpleRender_x64::on_listWidget_clicked(const QModelIndex &index)
{
	componentwidget.show();
	int currenRow = ui.listWidget->currentRow();
	emit mySignalListIndex(currenRow);
}
