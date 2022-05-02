#include "Shape.h"

// Form Class
std::vector<float> Shape::getVerticies() {
	return this->verticies;
};

glm::mat4 Shape::getTransformations() {
	return this->transformationMatrix;
}

GLenum Shape::getType() {
	return this->type;
}

void Shape::addVerticies(float x, float y, float z, float r, float g, float b) {
	// Verticies
	this->verticies.push_back(x);
	this->verticies.push_back(y);
	this->verticies.push_back(z);

	// Colors
	this->verticies.push_back(r);
	this->verticies.push_back(g);
	this->verticies.push_back(b);
};

void Shape::setVerticie(int index, float value) {
	this->verticies[index] = value;
}

void Shape::setMatrix(glm::mat4 matrix) {
	this->transformationMatrix = matrix;
}

void Shape::translate(float x, float y) {
	this->transformationMatrix = glm::translate(this->transformationMatrix, glm::vec3(x, y, 0.0f));
}

void Shape::rotate(float angle) {
	this->transformationMatrix = glm::rotate(this->transformationMatrix, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
}

void Shape::scale(float xScale, float yScale) {
	this->transformationMatrix = glm::scale(this->transformationMatrix, glm::vec3(xScale, yScale, 0.5));
}

void Shape::setType(GLenum type) {
	this->type = type;
}

void Shape::clearData() {
	this->verticies.clear();
};