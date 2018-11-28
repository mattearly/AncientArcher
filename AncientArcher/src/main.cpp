#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "Shader.h"

void reshapeCallback(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow* window);

float texCoords[] = {
    0.0f, 0.0f,  // lower-left corner
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
};

int main() {
	/* init glfw and opengl */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* create window to draw in */
	GLFWwindow* window = glfwCreateWindow(800, 600, "TITLE", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "failed to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, reshapeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed to init GLAD" << std::endl;
		return -1;
	}

  Shader shader("../AncientArcher/shaders/main.vs", "../AncientArcher/shaders/main.fs");

	/* set up an area to store a vertex data */
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	/* prep to send to send vertex buffer object */
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/* verticies for drawing */
	float vertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};
	/* send vertices to graphics card */
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* prep element buffer object */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	/* send the indices to the graphics card*/
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/* set which vertex attribute to use for upcoming operations */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);   //unbind EBO

	glBindVertexArray(0);  //unbind VAO

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/* main program loop */
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		float timeValue = glfwGetTime();
		/* change color */
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    shader.use();

    shader.setVec4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		/* draw based on element routine*/
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);  // we don't really need to unbind it every time

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return  0;
}

void reshapeCallback(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

void processInput(GLFWwindow * window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
