/********************************************************************************
** Form generated from reading UI file 'ComponentWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPONENTWIDGET_H
#define UI_COMPONENTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ComponentWidget
{
public:
    QTreeView *ComponentTree;

    void setupUi(QWidget *ComponentWidget)
    {
        if (ComponentWidget->objectName().isEmpty())
            ComponentWidget->setObjectName(QString::fromUtf8("ComponentWidget"));
        ComponentWidget->resize(568, 520);
        ComponentTree = new QTreeView(ComponentWidget);
        ComponentTree->setObjectName(QString::fromUtf8("ComponentTree"));
        ComponentTree->setGeometry(QRect(20, 20, 521, 471));

        retranslateUi(ComponentWidget);

        QMetaObject::connectSlotsByName(ComponentWidget);
    } // setupUi

    void retranslateUi(QWidget *ComponentWidget)
    {
        ComponentWidget->setWindowTitle(QCoreApplication::translate("ComponentWidget", "ComponentWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ComponentWidget: public Ui_ComponentWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPONENTWIDGET_H
