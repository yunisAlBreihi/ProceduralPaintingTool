#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "stb_image.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "GUIWindow.h"
#include "ObjectManager.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	globals::g_window_size = glm::vec2(width, height);
}

void handle_key_event(GLFWwindow* window, int key, int scancode, int action, int modifiers)
{
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, true);
		}
	}
}

void handle_mouse_event(GLFWwindow* window, int button, int action, int modifiers)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS) {
			globals::g_LMB_hold = true;
		}
		else if (action == GLFW_RELEASE) {
			globals::g_LMB_hold = false;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			globals::g_cam_control = true;
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			globals::g_cam_control = false;
		}
	}
}

void handle_mouse_pos(GLFWwindow* window, double mouse_x, double mouse_y)
{
	globals::g_mouse_position = glm::vec2(mouse_x, mouse_y);
	float y = (2.0f * globals::g_mouse_position.y) / globals::g_window_size.y - 1;

	//printf("MousePosY: %0.1f\n", globals::g_mouse_position.y);
	//printf("normalized MouseY: %0.1f\n", y);
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

	globals::g_window_size = glm::vec2(1000);
	GLFWwindow* m_window = glfwCreateWindow(globals::g_window_size.x, globals::g_window_size.y, "Editor Window", NULL, NULL);
	if (m_window == NULL) {
		printf("Failed to create GLFW window!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(m_window);

	glfwSetKeyCallback(m_window, handle_key_event);
	glfwSetMouseButtonCallback(m_window, handle_mouse_event);
	glfwSetCursorPosCallback(m_window, handle_mouse_pos);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == GLFW_FALSE) {
		printf("Failed to initialize GLAD!\n");
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		//Enable Keyboard controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			//Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;			//Enable Multi-ViewPort / platform windows

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	ObjectManager* m_objectManager = new ObjectManager(m_window);
	GUIWindow m_transformWindow = GUIWindow(Transform(), *m_objectManager);

	//Mouse movement
	glm::vec2 m_last_mouse_position = globals::g_mouse_position;

	while (glfwWindowShouldClose(m_window) == GLFW_FALSE) {
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Early update
		globals::g_mouse_delta = globals::g_mouse_position - m_last_mouse_position;
		m_last_mouse_position = globals::g_mouse_position;
		m_transformWindow.update();

		//Update
		m_objectManager->update();

		//render
		//m_objectManager->render();
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