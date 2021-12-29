#pragma once
#include "Camera.h"
class CameraFreeMode :
	public Camera
{
public:
	CameraFreeMode() ;
	//CameraFreeMode(glm::vec3 pos, glm::vec3 front, glm::vec3 up) ;

	virtual void CameraKeyboardCrtl(float front, float back, float left, float right);
	virtual void CameraMouseCrtl(QPointF &firstPoint, QPointF &nextPoint);
protected:
	//弃用
	//void createRotation(QPointF &firstPoint, QPointF &nextPoint);
	//平移更新
	void updateView();
	//旋转更新
	void updateView(glm::vec3 dir,float angle);
	glm::vec3 projectToTrackball(QPointF& point);
	virtual glm::mat4 getView();
};

//CameraFreeMode::CameraFreeMode(glm::vec3 pos, glm::vec3 front, glm::vec3 up)
//{
//	this->cameraSpeed = 0.4f;
//	this->Sensitivity = 0.1f;
//	this->cameraPos = pos;
//	this->cameraFront = front;
//	this->cameraUp = up;
//	updateView();
//}