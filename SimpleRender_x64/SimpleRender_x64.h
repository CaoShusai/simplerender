#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SimpleRender_x64.h"
#include<string>
#include"ComponentWidget.h"
class SimpleRender_x64 : public QMainWindow
{
    Q_OBJECT

public:
    SimpleRender_x64(QWidget *parent = Q_NULLPTR);
	void keyPressEvent(QKeyEvent *event);
	void mouseReleaseEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent* event);
private:
    Ui::SimpleRender_x64Class ui;
	bool leftMouseButtonActive;
	QPointF firstPoint, nextPoint;
	float glWidth, glHeight;
	ComponentWidget componentwidget;
signals:
	void mySignalParam(int, int, int, int);
	void mySignalMouse(QPointF, QPointF);
	void mySignalListIndex(int);
private slots:
	void mySlotAddList(int);
	void on_listWidget_clicked(const QModelIndex &index);
	void mySlotShowFps(float);
};
