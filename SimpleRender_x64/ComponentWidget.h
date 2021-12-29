#pragma once

#include <QWidget>
#include "ui_ComponentWidget.h"

class ComponentWidget : public QWidget
{
	Q_OBJECT

public:
	ComponentWidget(QWidget *parent = Q_NULLPTR);
	~ComponentWidget();

private:
	Ui::ComponentWidget ui;
};
