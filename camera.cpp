#include "camera.h"
#include<math.h>
void camera::translate(float front, float back, float left, float right) 
{
	if (front == 1)
	{
		cameraPos += cameraSpeed * cameraFront;
		//view = glm::scale(view, glm::vec3(1.1, 1.1, 1.1));
	}
	else if(back==1)
	{ 
		cameraPos -= cameraSpeed * cameraFront;
		//view = glm::scale(view, glm::vec3(0.8, 0.8, 0.8));
	}
	else if (left==1)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	else if (right==1)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	updateView(0);
}

void camera::updateView(int flag)
{
	if (flag==1)
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, angle, dir);
		
		
		//this->view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
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

		this->view = glm::lookAt(cameraPos, cameraPos+ cameraFront, cameraUp);

		angle = 0.0;
	}
	else;
		this->view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	
	//dir = glm::vec3();
}

glm::vec3 camera::projectToTrackball(QPointF& point)
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

void camera::createRotation( QPointF &firstPoint, QPointF &nextPoint)
{
	glm::vec3 lastPos3D = projectToTrackball(firstPoint);
	lastPos3D=glm::normalize(lastPos3D);
	glm::vec3 currentPos3D = projectToTrackball(nextPoint);
	currentPos3D = glm::normalize(currentPos3D);
	dir = glm::cross(currentPos3D, lastPos3D);
	angle = acos(glm::clamp(glm::dot(currentPos3D, lastPos3D),0.0f,1.0f));
	
	updateView(1);
}


