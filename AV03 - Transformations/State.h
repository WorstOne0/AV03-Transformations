#ifndef STATE_H
#define STATE_H

#include "VAO.h"
#include "VBO.h"
#include "Shape.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <math.h>

class State {
private:
	// One VAO and array for all the points and other for all the curvers (so its possible to show only the curves)
	std::vector<VAO> shapeVAO;
	std::vector<Shape> Shapes;
	// The shape that is being built
	Shape newShape;
public:
	bool isMouse = true, isConsole = false;
	int consoleOptions;

	std::vector<float> colors = {
		//0.0f, 0.0f, 0.0f, // Black
		1.0f, 1.0f, 1.0f, // White
		0.482f, 0.803f, 0.729f,
		0.607f, 0.494f, 0.870f,
		0.309f, 0.615f, 0.411f,
		0.976f, 0.439f, 0.407f,
		0.819f, 0.839f, 0.274f,
		0.341f, 0.768f, 0.898f
	};

	// Get Functions
	std::vector<Shape> getShapes();

	// Set Function
	void addShapeToVAO(GLenum type);
	void addVerticieToShape(float x, float y, float z, float r, float g, float b);
	void deleteVAOs();

	// OpenGL functions
	void configureWindow(GLFWwindow* window);

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void getFromConsole();
	void newObjectFromConsole();
	void tranlationFromConsole(int index);
	void rotateFromConsole(int index);
	void scaleFromConsole(int index);
	void reflectionFromConsole(int index);

	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	void createRegularPolygon(int numOfSides, float radius);

	void drawCartesianPlane();
	void drawFromShapeVAO(GLuint uniformID);
};

/*
Cores

  0,   0,   0 - 0.0f, 0.0f, 0.0f
255, 255, 255 - 1.0f, 1.0f, 1.0f
123, 205, 186 - 0.482f, 0.803f, 0.729f
155, 126, 222 - 0.607f, 0.494f, 0.870f
156, 175, 183 - 0.611f, 0.686f, 0.717f
 79, 157, 105 - 0.309f, 0.615f, 0.411f
249, 112, 104 - 0.976f, 0.439f, 0.407f
209, 214,  70 - 0.819f, 0.839f, 0.274f
 87, 196, 229 - 0.341f, 0.768f, 0.898f
*/

#endif