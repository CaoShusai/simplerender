#pragma once
#include<QTime>


#include<QOpenGLShader>
#include<QOpenGLShaderProgram>


class timeCtrl
{
public:
	timeCtrl() {
		qtime.start();
		lastTime = qtime.elapsed();
	};
	
	float getFps() {
		float Fps = 1 / ((qtime.elapsed() - lastTime) / 1000.0);
		//qDebug() << "FPS:" <<Fps;
		//qDebug() << "T:"<< meshes[i].Tcoeff[81];
		updateTime();
		return Fps;
	};
	float getTime()
	{
		float currentTime = qtime.elapsed() / 1000.0;
		return currentTime;
	}
private:
	QTime qtime;
	float lastTime;
	void updateTime()
	{
		lastTime = qtime.elapsed();
	};
};

