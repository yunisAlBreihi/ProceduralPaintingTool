#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}

void handle_key_event(GLFWwindow* window, int key, int scancode, int action, int modifiers) 
{
	if (action == GLFW_PRESS){
		if (key == GLFW_KEY_ESCAPE) {
			glfwDestroyWindow(window);
		}
	}
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* m_window = glfwCreateWindow(1280, 720, "Editor Window", NULL, NULL);
	if (m_window == NULL) {
		printf("Failed to create GLFW window!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(m_window);

	glfwSetKeyCallback(m_window, handle_key_event);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == GLFW_FALSE) {
		printf("Failed to initialize GLAD!\n");
		return -1;
	}

	while (glfwWindowShouldClose(m_window) == GLFW_FALSE) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	return 0;
}