#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "stb_image.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Mesh.h"
#include "GUIWindow.h"
#include "Camera.h"
#include "Timer.h"
#include "ObjectManager.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void handle_key_event(GLFWwindow* window, int key, int scancode, int action, int modifiers)
{
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE) {
			//glfwDestroyWindow(window);
			glfwSetWindowShouldClose(window, true);
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

	GLFWwindow* m_window = glfwCreateWindow(800, 800, "Editor Window", NULL, NULL);
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

	//glEnable(GL_DEPTH_TEST);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		//Enable Keyboard controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			//Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;			//Enable Multi-ViewPort / platform windows

	ImGui::StyleColorsDark();

	const std::vector m_squareVertices = {
		//Position
		 -0.5f,  -0.5f,  0.0f,					// Bot Left
		  0.5f,  -0.5f,  0.0f,					// Bot Right
		  0.5f,   0.5f,  0.0f,					// Top Right
		 -0.5f,   0.5f,  0.0f,					// Top Left
	};

	const std::vector m_squareIndices = {
		0, 1, 2,			//0.0
		0, 2, 3,			//0.1
	};

	ObjectManager* m_objectManager = new ObjectManager();

	std::vector<Shader> m_shaders;

	Shader m_shaderDefault("Shaders/default.vs", "Shaders/default.fs");
	m_objectManager->addShader(m_shaderDefault);

	Mesh m_monkey("Assets/Monkey.obj", Transform(), m_shaderDefault);
	m_objectManager->addMesh(m_monkey);

	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	Transform& currentTransform = m_monkey.getTransform();

	GUIWindow m_transformWindow = GUIWindow(currentTransform, *m_objectManager);

	Camera m_camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), m_window, *m_objectManager);
	Timer m_timer;

	while (glfwWindowShouldClose(m_window) == GLFW_FALSE) {
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//before update
		m_transformWindow.update();
		m_timer.update();

		//During update
		m_camera.update(m_timer.deltaTime);
		m_transformWindow.updateSliders(m_monkey.getTransform());
		m_objectManager->update();

		//render
		m_objectManager->render();
		m_transformWindow.render(m_window);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_window);
	glfwTerminate();

	return 0;
}