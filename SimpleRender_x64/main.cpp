#include "SimpleRender_x64.h"
#include <QtWidgets/QApplication>
#include<QFile>
#include<QOpenGLShader>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	
	//QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

	QFile f(":qdarkstyle/style.qss");

	if (!f.exists()) {
		printf("Unable to set stylesheet, file not found\n");
	}
	else {
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		qApp->setStyleSheet(ts.readAll());
	}
    SimpleRender_x64 w;
    w.show();
    return a.exec();
}
