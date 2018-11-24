#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

void reshapeCallback(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow* window);

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

const char * vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"void main() {\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}";

const char * fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}";

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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

	/* set up an area to store a vertex array */
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	/* send triangle vertices to graphics card */
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* compile vertex shader */
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	/* determine if vertex shader was successful in compiling */
	int v_success;
	char v_infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &v_success);
	if (!v_success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, v_infoLog);
		std::cout << "error in vertex shader, compilation failed: " << v_infoLog << std::endl;
		return -1;
	}

	/* compile fragment shader */
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	/* determine if fragment shader was successful in compiling */
	int f_success;
	char f_infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &f_success);
	if (!f_success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, f_infoLog);
		std::cout << "error in fragment shader, compilation failed: " << f_infoLog << std::endl;
		return -1;
	}

	/* make the shader program */
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	/* check that the shaderProgram was successful */
	int p_success;
	char p_infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &p_success);
	if (!p_success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, p_infoLog);
		std::cout << "error in shaderProgram: " << p_infoLog << std::endl;
		return -1;
	}

	/* use the shaderProgram we just set up */
	glUseProgram(shaderProgram);

	/* shaderProgram handles the data from these so we don't need them anymore */
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/* set which vertex attribute to use for upcoming operations */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* main program loop */
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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
