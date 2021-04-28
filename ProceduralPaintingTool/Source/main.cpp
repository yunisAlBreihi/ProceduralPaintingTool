#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
//#include "stb_image.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "GUIBrushProperties.h"
#include "ObjectManager.h"
#include "BrushManager.h"
#include "IOHandler.h"
#include <stdlib.h>

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
	

	//Get global attributes from save file
	IOHandler::loadJson_attribute(globals::g_saveNameGlobals, "seed", globals::g_seed);
	IOHandler::loadJson_attribute(globals::g_saveNameGlobals, "hasGlobalsSave", globals::g_hasGlobalsSave);
	IOHandler::loadJson_attribute(globals::g_saveNameGlobals, "hasBrushSave", globals::g_hasBrushSave);
	IOHandler::loadJson_attribute(globals::g_saveNameGlobals, "hasTerrainVerticesSave", globals::g_hasTerrainVerticesSave);

	BrushManager* m_brushManager = new BrushManager();
	ObjectManager* m_objectManager = new ObjectManager(m_window, *m_brushManager);
	GUIBrushProperties m_GUIBrushProperties = GUIBrushProperties(*m_objectManager, *m_brushManager);

	//Mouse movement
	glm::vec2 m_last_mouse_position = globals::g_mouse_position;

	//Start functions here. Don't run code after this point
	m_brushManager->start();
	m_objectManager->start();

	while (glfwWindowShouldClose(m_window) == GLFW_FALSE) {
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Early update
		globals::g_mouse_delta = globals::g_mouse_position - m_last_mouse_position;
		m_last_mouse_position = globals::g_mouse_position;
		m_GUIBrushProperties.update();
		m_GUIBrushProperties.render(m_window);

		//Update
		m_objectManager->update();

		//render
		//m_objectManager->render();

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	//Quit functions go here
	m_objectManager->quit();
	m_brushManager->quit();

	//Save global attributes to save file
	json* m_saveFile = new json();
	IOHandler::saveJson_attribute(*m_saveFile, globals::g_saveNameGlobals, "seed", globals::g_seed);
	IOHandler::saveJson_attribute(*m_saveFile, globals::g_saveNameGlobals, "hasGlobalsSave", globals::g_hasGlobalsSave);
	IOHandler::saveJson_attribute(*m_saveFile, globals::g_saveNameGlobals, "hasBrushSave", globals::g_hasBrushSave);
	IOHandler::saveJson_attribute(*m_saveFile, globals::g_saveNameGlobals, "hasTerrainVerticesSave", globals::g_hasTerrainVerticesSave);

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_window);
	glfwTerminate();

	return 0;
}