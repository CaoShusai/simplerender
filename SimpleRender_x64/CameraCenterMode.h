#pragma once
#include "Camera.h"
class CameraCenterMode :
	public Camera
{
public:
	CameraCenterMode();
	//CameraCenterMode(glm::vec3 pos, glm::vec3 front, glm::vec3 up) ;

	virtual void CameraKeyboardCrtl(float front, float back, float left, float right);
	virtual void CameraMouseCrtl(QPointF &firstPoint, QPointF &nextPoint);
protected:
	//����
	//void createRotation(QPointF &firstPoint, QPointF &nextPoint);
	//ƽ�Ƹ���
	void updateView();
	//��ת����
	void updateView(glm::vec3 dir, float angle);
	glm::vec3 projectToTrackball(QPointF& point);
	virtual glm::mat4 getView();

};

