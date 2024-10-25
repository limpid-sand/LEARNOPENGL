#include<iostream>

#include<glad\glad.h>
#include<GLFW\glfw3.h>

#include"shader.h"


void processInput(GLFWwindow* window);

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);


int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1840, 1080, "LearnShader", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Create Window Failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD INITIALIZE FAILED" << std::endl;
		return -1;
	}

	glViewport(0, 0, 1840, 1080);

	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	int max_vertex_attribute_num = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attribute_num);
	std::cout << "max vertex attribute num : " << max_vertex_attribute_num << std::endl;

	glClearColor(0.2f, 0.5f, 1.0f, 1.0f);

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	

	float positions[] = {
		-0.5f,-0.5f, 0.0f, 1.0f,0.0f,0.0f,
		 0.5f,-0.5f, 0.0f, 0.0f,1.0f,0.0f,
		 0.0f, 0.5f, 0.0f, 0.0f,0.0f,1.0f
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	Shader shader("res/Shaders/shader.vs", "res/Shaders/shader.fs");


	while (!glfwWindowShouldClose(window)) {
		
		processInput(window);

		//float timeVal = (float)glfwGetTime();
		//float greenVal = (sin(timeVal) / 2.0f) + 0.5f;
		//int m_color = glGetUniformLocation(program, "m_color");

		shader.use();
		//glUniform4f(m_color, 0.2f, greenVal, 0.3f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, false);
	}
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

