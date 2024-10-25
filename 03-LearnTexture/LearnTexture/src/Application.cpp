#include<iostream>

#include<glad\glad.h>
#include<GLFW\glfw3.h>

#include"vender\std_image\stb_image.h"
#include"shader.h"


void processInput(GLFWwindow* window,Shader shader);

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);

float mixval = 0.0f;


int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1080, 840, "LearnShader", nullptr, nullptr);
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

	glViewport(0, 0, 1080, 840);

	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	int max_vertex_attribute_num = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attribute_num);
	std::cout << "max vertex attribute num : " << max_vertex_attribute_num << std::endl;

	glClearColor(0.2f, 0.5f, 1.0f, 1.0f);

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	float positions[] = {
		-0.5f,-0.5f, 0.0f,   1.0f,0.0f,0.0f,  0.0f,0.0f,
		 0.5f,-0.5f, 0.0f,   0.0f,1.0f,0.0f,  1.0f,0.0f,
		-0.5f, 0.5f, 0.0f,   0.0f,0.0f,1.0f,  0.0f,1.0f,
		 0.5f, 0.5f, 0.0f,   1.0f,1.0f,0.0f,  1.0f,1.0f
	};

	glGenBuffers(1, &VBO);
	glActiveTexture(GL_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	unsigned int indices[] = {
		0,1,2,
		1,2,3
	};

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	Shader shader("res/Shaders/shader.vs", "res/Shaders/shader.fs");



	unsigned int texture1,texture2;
	int width, height, nrChannels;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("res/Textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	data = stbi_load("res/Textures/awesomeface.png", &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);

	

	while (!glfwWindowShouldClose(window)) {
		
		processInput(window,shader);

		//float timeVal = (float)glfwGetTime();
		//float greenVal = (sin(timeVal) / 2.0f) + 0.5f;
		//int m_color = glGetUniformLocation(program, "m_color");
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shader.setInt("texture1", 0);
		shader.setInt("texture2", 1);
		shader.setFloat("mixval", mixval);
		
		//glUniform4f(m_color, 0.2f, greenVal, 0.3f, 1.0f);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}


void processInput(GLFWwindow* window,Shader shader) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, false);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mixval = std::min(1.0f, mixval + 0.0004f);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mixval = std::max(0.0f, mixval - 0.0004f);
	}

}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

