#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"QMouseEvent"
class camera
{
public:
	glm::mat4 view;
	void getView();
	camera()
	{
		this->cameraPos = glm::vec3(-0.04,1.0848,3.5918);
		this->cameraFront = glm::vec3(0,0,-1);
		this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		this->view = glm::lookAt(cameraPos, cameraPos+ cameraFront, cameraUp);
	}
	//float 
	glm::vec3 cameraPos;
	void translate(float front, float back, float left, float right);
	void createRotation( QPointF &firstPoint,  QPointF &nextPoint);
private:
	glm::vec3 projectToTrackball(QPointF& point);
	glm::vec3 dir;//Ðý×ªÖá
	
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	float angle;//Ðý×ª½Ç¶È
	float cameraSpeed = 0.4f;
	float Sensitivity = 0.1f;
	void updateView(int flag);
};

