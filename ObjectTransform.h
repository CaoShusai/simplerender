#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class ObjectTransform
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	void translate(float front, float back, float left, float right);
	//void createRotation(QPointF &firstPoint, QPointF &nextPoint);
	//void 
};

