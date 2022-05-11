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

void Shape::calcCenter() {
	float centerX = 0, centerY = 0;

	for (int i = 0; i < this->verticies.size() / 6; i++) {
		centerX += this->verticies[(6 * i)];
		centerY += this->verticies[(6 * i) + 1];
	}

	centerX /= (this->verticies.size() / 6);
	centerY /= (this->verticies.size() / 6);

	this->centerX = centerX;
	this->centerY = centerY;
}

void Shape::translate(float x, float y) {
	this->transformationMatrix = glm::translate(this->transformationMatrix, glm::vec3(x, y, 0.0f));
}

// For rotation you have to translate de object to the origin, rotate than translate back
// But it matrix multiplication the order is revesed so Matrix = translateBack * rotate * translateOrigin
void Shape::rotate(float angle) {
	// Translate back to where it was
	this->transformationMatrix = glm::translate(this->transformationMatrix, glm::vec3(this->centerX, this->centerY, 0.0f));
	// Rotate
	this->transformationMatrix = glm::rotate(this->transformationMatrix, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
	// Translate to the origin
	this->transformationMatrix = glm::translate(this->transformationMatrix, glm::vec3((-1) * this->centerX, (-1) * this->centerY, 0.0f));
	
	// Matrix = translateBack * rotate * translateOrigin
}

void Shape::scale(float xScale, float yScale) {
	this->transformationMatrix = glm::scale(this->transformationMatrix, glm::vec3(xScale, yScale, 0.5));
}

void Shape::reflection(bool xAxis) {
	xAxis ? this->transformationMatrix = glm::scale(this->transformationMatrix, glm::vec3(1, -1, 1)) : this->transformationMatrix = glm::scale(this->transformationMatrix, glm::vec3(-1, 1, 1));
}

void Shape::shear(float angle) {
	this->transformationMatrix = glm::rotate(this->transformationMatrix, glm::radians(-angle / 2), glm::vec3(0.0, 0.0, 1.0));

	this->transformationMatrix = glm::scale(this->transformationMatrix, glm::vec3(sin(angle / 2), cos(angle / 2), 0.5));

	this->transformationMatrix = glm::rotate(this->transformationMatrix, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));

	this->transformationMatrix = glm::scale(this->transformationMatrix, glm::vec3(sqrt(2) / sin(angle), sqrt(2) / cos(angle), 0.5));
}

void Shape::setType(GLenum type) {
	this->type = type;

	this->calcCenter();
}

void Shape::clearData() {
	this->verticies.clear();
};