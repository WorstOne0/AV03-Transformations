#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

class Shape {
private:
	std::vector<float> verticies;
	glm::mat4 transformationMatrix = glm::mat4(1.0f);
	float centerX = 0, centerY = 0;

	GLenum type;
public:
	// Get Fuctions
	std::vector<float> getVerticies();
	glm::mat4 getTransformations();
	GLenum getType();

	// Set Functions
	void addVerticies(float x, float y, float z, float r, float g, float b);
	void setVerticie(int index, float value);
	void calcCenter();

	void setMatrix(glm::mat4 matrix);
	void translate(float x, float y);
	void rotate(float angle);
	void scale(float xScale, float yScale);
	void reflection(bool xAxis);
	void shear(float angle);

	void setType(GLenum type);
	void clearData();
};

#endif