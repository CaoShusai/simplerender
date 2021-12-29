#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"QMouseEvent"
class Camera
{
protected:
	glm::mat4 view;
	
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	float cameraSpeed ;
	float Sensitivity ;

public:
	glm::vec3 cameraPos;
	virtual glm::mat4 getView()=0;
	virtual void CameraKeyboardCrtl(float front, float back, float left, float right)=0;
	virtual void CameraMouseCrtl(QPointF &firstPoint, QPointF &nextPoint)=0;
	virtual ~Camera() {};
	
};

