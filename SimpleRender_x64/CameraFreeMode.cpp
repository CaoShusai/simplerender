#include "CameraFreeMode.h"

CameraFreeMode::CameraFreeMode()
{
	this->cameraSpeed = 0.4f;
	this->Sensitivity = 0.1f;
	this->cameraPos = glm::vec3(-0.04, 1.0848, 3.5918);
	this->cameraFront = glm::vec3(0, 0, -1);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	updateView();
}



glm::mat4 CameraFreeMode::getView()
{
	return this->view;
}

/*
void CameraFreeMode::translate(float front, float back, float left, float right)
{
	if (front == 1)
	{
		cameraPos += cameraSpeed * cameraFront;
		//view = glm::scale(view, glm::vec3(1.1, 1.1, 1.1));
	}
	else if (back == 1)
	{
		cameraPos -= cameraSpeed * cameraFront;
		//view = glm::scale(view, glm::vec3(0.8, 0.8, 0.8));
	}
	else if (left == 1)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	else if (right == 1)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	updateView(0);
}


void CameraFreeMode::updateView(int flag)
{
	if (flag == 1)
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, angle, dir);

		//this->getView() = glm::lookAt(cameraPos, cameraTarget, cameraUp);
		//view = glm::rotate(view, angle, dir);

		glm::vec4 tmpCameraFront(cameraFront, 1.0);
		tmpCameraFront = trans * tmpCameraFront;
		cameraFront.x = tmpCameraFront.x;
		cameraFront.y = tmpCameraFront.y;
		cameraFront.z = tmpCameraFront.z;
		glm::vec4 tmpCameraUp(cameraUp, 1.0);
		tmpCameraUp = trans * tmpCameraUp;
		cameraUp.x = tmpCameraUp.x;
		cameraUp.y = tmpCameraUp.y;
		cameraUp.z = tmpCameraUp.z;

		this->getView() = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		angle = 0.0;
	}
	else;
	this->getView() = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	//dir = glm::vec3();
}
*/


glm::vec3 CameraFreeMode::projectToTrackball(QPointF& point)
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

//void CameraFreeMode::createRotation(QPointF &firstPoint, QPointF &nextPoint)
//{
//	glm::vec3 lastPos3D = projectToTrackball(firstPoint);
//	lastPos3D = glm::normalize(lastPos3D);
//	glm::vec3 currentPos3D = projectToTrackball(nextPoint);
//	currentPos3D = glm::normalize(currentPos3D);
//	dir = glm::cross(currentPos3D, lastPos3D);
//	angle = acos(glm::clamp(glm::dot(currentPos3D, lastPos3D), 0.0f, 1.0f));
//
//	updateView(1);
//}

void CameraFreeMode::CameraMouseCrtl(QPointF &firstPoint, QPointF &nextPoint)
{
	glm::vec3 lastPos3D = projectToTrackball(firstPoint);
	lastPos3D = glm::normalize(lastPos3D);
	glm::vec3 currentPos3D = projectToTrackball(nextPoint);
	currentPos3D = glm::normalize(currentPos3D);
	glm::vec3 dir = glm::cross(currentPos3D, lastPos3D);
	float angle = acos(glm::clamp(glm::dot(currentPos3D, lastPos3D), 0.0f, 1.0f));

	updateView(dir,angle);
}


void CameraFreeMode::CameraKeyboardCrtl(float front, float back, float left, float right)
{
	if (front == 1)
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	else if (back == 1)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	else if (left == 1)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	else if (right == 1)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	updateView();
}

void CameraFreeMode::updateView(glm::vec3 dir, float angle)
{
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, angle, dir);

	glm::vec4 tmpCameraFront(cameraFront, 1.0);
	tmpCameraFront = trans * tmpCameraFront;
	cameraFront.x = tmpCameraFront.x;
	cameraFront.y = tmpCameraFront.y;
	cameraFront.z = tmpCameraFront.z;
	glm::vec4 tmpCameraUp(cameraUp, 1.0);
	tmpCameraUp = trans * tmpCameraUp;
	cameraUp.x = tmpCameraUp.x;
	cameraUp.y = tmpCameraUp.y;
	cameraUp.z = tmpCameraUp.z;

	this->view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void CameraFreeMode::updateView()
{
	this->view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}