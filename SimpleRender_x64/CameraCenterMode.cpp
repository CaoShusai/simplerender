#include "CameraCenterMode.h"
CameraCenterMode::CameraCenterMode()
{
	this->cameraSpeed = 0.4f;
	this->Sensitivity = 0.1f;
	this->cameraPos = glm::vec3(-0.04, 1.0848, 3.5918);
	this->cameraFront = glm::vec3(0, 0, -1);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}



glm::mat4 CameraCenterMode::getView()
{
	return this->view;
}

glm::vec3 CameraCenterMode::projectToTrackball(QPointF& point)
{
	float r2 = 1.0;
	float z = 0.0f;
	float lengthSquared = point.x()*point.x() + point.y()*point.y();
	float length = pow(lengthSquared, 0.5);
	if (lengthSquared <= r2 * 0.5f) {
		z = pow(r2 - lengthSquared, 0.5);
	}
	else {
		z = r2 * 0.5f / length;
	}
	return glm::vec3(point.x(), point.y(), z);
}

void CameraCenterMode::CameraMouseCrtl(QPointF &firstPoint, QPointF &nextPoint)
{
	glm::vec3 lastPos3D = projectToTrackball(firstPoint);
	lastPos3D = glm::normalize(lastPos3D);
	glm::vec3 currentPos3D = projectToTrackball(nextPoint);
	currentPos3D = glm::normalize(currentPos3D);
	glm::vec3 dir = glm::cross(currentPos3D, lastPos3D);
	float angle = acos(glm::clamp(glm::dot(currentPos3D, lastPos3D), 0.0f, 1.0f));

	updateView(dir, angle);
}


void CameraCenterMode::CameraKeyboardCrtl(float front, float back, float left, float right)
{
	if (front == 1)
	{
		view = glm::scale(view, glm::vec3(1.1, 1.1, 1.1));
	}
	else if (back == 1)
	{
		view = glm::scale(view, glm::vec3(0.9, 0.9, 0.9));
	}
	
	//updateView();
}

void CameraCenterMode::updateView(glm::vec3 dir, float angle)
{
	view = glm::rotate(view, angle, dir);
}

void CameraCenterMode::updateView()
{
	
}