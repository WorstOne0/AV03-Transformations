#include "State.h"

// Program Class
std::vector<Shape> State::getShapes() {
	return this->Shapes;
};

void State::addShapeToVAO(GLenum type) {
	// Type to be drawn
	this->newShape.setType(type);

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(this->newShape.getVerticies().data(), this->newShape.getVerticies().size() * sizeof(float));

	// Links VBO to VAO
	// Vertex
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	// Color
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// Adds the VAO to the list
	this->shapeVAO.push_back(VAO1);

	// Unbind all to prevent accidentally modifying them
	VBO1.Unbind();
	VAO1.Unbind();
	VBO1.Delete();

	// Adds the shape to the array
	this->Shapes.push_back(this->newShape);

	// Resets the new shape
	this->newShape.clearData();
};

void State::addVerticieToShape(float x, float y, float z, float r, float g, float b) {
	this->newShape.addVerticies(x, y, z, r, g, b);
};

void State::deleteVAOs() {
	for (int i = 0; i < this->shapeVAO.size(); i++) {
		this->shapeVAO[i].Delete();
	}
};

// Keyboard and Mouse Input

// Configure the window to be possible to use class functions as callbacks
void State::configureWindow(GLFWwindow* window) {
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	glfwSetWindowUserPointer(window, this);
}

void State::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	State* ptrState = static_cast<State*>(glfwGetWindowUserPointer(window));

	// Swap between mouse and keyboard input
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		std::string frase = ptrState->isMouse ? "Teclado. Tecle I para ativar a digitalizacao no console \n" : "Mouse \n";

		// Swap
		ptrState->isMouse = !ptrState->isMouse;
		std::cout << "O Modo de input agora e pelo " << frase << std::endl;
	}

	if (!ptrState->isMouse) {
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			ptrState->isConsole = true;

			std::cout << "A tela esta travada enquanto o modo de digitacao pelo console esta ativo, selecione o console" << std::endl;
		}
	}

	// Toggle the points
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		std::string frase = ptrState->drawPoints ? "desativados" : "ativados";

		// Swap
		ptrState->drawPoints = !ptrState->drawPoints;
		std::cout << "Os pontos foram " << frase << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		//ptrState->Shapes[0].translate(0.1f, 0.0f);
		ptrState->Shapes[0].scale(0.5f, 0.5f);

		std::cout << "Translation" << std::endl;
	}

	// Submits the points
	if (!ptrState->isConsole) {
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			// No points
			if (ptrState->newShape.getVerticies().size() == 0) return;

			ptrState->addShapeToVAO(GL_TRIANGLE_FAN);
		}
	}
		
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void State::getFromConsole() {
	std::string a;
	std::cout << "Digite o " << (this->newShape.getVerticies().size() / 6) + 1 << " verticie" << std::endl;

	std::cout << "X: ";
	std::cin >> a;
	float x = std::stof(a);

	std::cout << "Y: ";
	std::cin >> a;
	float y = std::stof(a);

	// NDC
	if ((x > 1.0f || y > 1.0f) || (x < -1.0f || y < -1.0f)) {
		std::cout << "O Ponto adiconado em (" << x << ", " << y << ") e invalido e sera descartado" << std::endl;
		return;
	}

	// No repeated points for the same curve
	for (int i = 0; i < this->newShape.getVerticies().size() / 6; i++) {
		if (this->newShape.getVerticies()[(6 * i)] == x && this->newShape.getVerticies()[(6 * i) + 1] == y) {
			std::cout << "O Ponto adiconado em (" << x << ", " << y << ") e repetido e sera descartado" << std::endl;
			return;
		}
	}

	// Add the verticies with a color to the shape
	this->addVerticieToShape(x, y, 0.0f, this->colors[(3 * this->Shapes.size())], this->colors[(3 * this->Shapes.size()) + 1], this->colors[(3 * this->Shapes.size()) + 2]);

	std::cout << "Digite no console 'q' para sair. Digite 'i' pra continuar" << std::endl;
	std::cin >> a;

	// Max 9 points or user finished inserting
	if (a == "q" || (this->newShape.getVerticies().size() / 6) >= 9) {
		this->isConsole = false;

		// **** Add To The VAO ****

		std::cout << "O modo de input pelo console terminou, selecione novamente a tela" << std::endl;
	}
}

void State::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	State* ptrState = static_cast<State*>(glfwGetWindowUserPointer(window));

	if (ptrState->isMouse) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			int width, height;
			double xpos = 0, ypos = 0;

			// Get mouse position
			glfwGetCursorPos(window, &xpos, &ypos);
			// Get window size
			glfwGetWindowSize(window, &width, &height);

			// Convert to NDC coordinates
			float x = -1.0f + 2 * xpos / width;
			float y = +1.0f - 2 * ypos / height;

			// No repeated points for the same curve
			for (int i = 0; i < ptrState->newShape.getVerticies().size() / 6; i++) {
				if (ptrState->newShape.getVerticies()[(6 * i)] == x && ptrState->newShape.getVerticies()[(6 * i) + 1] == y) {
					std::cout << "O Ponto adiconado em (" << x << ", " << y << ") e repetido e sera descartado" << std::endl;
					return;
				}
			}

			std::cout << "Ponto adiconado em (" << x << ", " << y << ")" << std::endl;

			// Add the verticies with a color to the shape
			ptrState->addVerticieToShape(x, y, 0.0f, ptrState->colors[(3 * ptrState->Shapes.size())], ptrState->colors[(3 * ptrState->Shapes.size()) + 1], ptrState->colors[(3 * ptrState->Shapes.size()) + 2]);
		}
	}
}

void State::createRegularPolygon(int numOfSides, float radius) {
	int numOfVerticies = numOfSides + 2;

	for (int i = 1; i < numOfVerticies; i++) {
		this->addVerticieToShape(
			this->newShape.getVerticies()[0] + (radius * cos(i * 2.0f * 3.141592 / numOfSides)), 
			this->newShape.getVerticies()[1] + (radius * sin(i * 2.0f * 3.141592 / numOfSides)),
			0.0f, 
			this->colors[(3 * this->Shapes.size())], 
			this->colors[(3 * this->Shapes.size()) + 1], 
			this->colors[(3 * this->Shapes.size()) + 2]);
	}
}
	
void State::drawCartesianPlane() {
	// Verticies
	std::vector<float> vertices = {
	//   POSITION      /       COLOR(Black)
	1.0f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f, // Positive X-axis
	-1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f, // Negative X-axis
	0.0f, 1.0f, 0.0f,     0.0f, 0.0f, 0.0f, // Positive Y-axis
	0.0f, -1.0f, 0.0f,    0.0f, 0.0f, 0.0f, // Negative Y-axis
	};

	for (float i = 0.1f; i < 1.0f; i += 0.1f) {
		// Positive X-axis point 1
		vertices.push_back(i);
		vertices.push_back(0.01f);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		// Positive X-axis point 2
		vertices.push_back(i);
		vertices.push_back(-0.01f);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		// Negative X-axis point 1
		vertices.push_back(-i);
		vertices.push_back(0.01f);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		// Negative X-axis point 2
		vertices.push_back(-i);
		vertices.push_back(-0.01f);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		// Positive Y-axis point 1
		vertices.push_back(0.01f);
		vertices.push_back(i);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		// Positive Y-axis point 2
		vertices.push_back(-0.01f);
		vertices.push_back(i);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		// Negative Y-axis point 1
		vertices.push_back(0.01f);
		vertices.push_back(-i);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		// Negative Y-axis point 2
		vertices.push_back(-0.01f);
		vertices.push_back(-i);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
	}

	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices.data(), vertices.size() * sizeof(float));

	// Links VBO to VAO
	// Vertex
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	// Color
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glDrawArrays(GL_LINES, 0, vertices.size() / 6);

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	VBO1.Delete();
}

void State::drawFromShapeVAO(GLuint uniformID) {
	// For each shape on the VAO
	for (int i = 0; i < this->shapeVAO.size(); i++) {
		// Bind the VAO to know wicth points it should to draw
		this->shapeVAO[i].Bind();
		
		// Set the transformation matrix
		glUniformMatrix4fv(uniformID, 1, GL_FALSE, glm::value_ptr(this->Shapes[i].getTransformations()));
		//glUniformMatrix4fv(uniformID, 1, GL_FALSE, glm::value_ptr(this->Shapes[i].t[this->Shapes[i].index]));

		glDrawArrays(this->Shapes[i].getType(), 0, this->Shapes[i].getVerticies().size() / 6);

		this->shapeVAO[i].Unbind();
	}
}