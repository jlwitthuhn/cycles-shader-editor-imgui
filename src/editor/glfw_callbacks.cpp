#include "glfw_callbacks.h"

#include <GLFW/glfw3.h>

#include "main_window.h"

thread_local cse::MainWindow* threadlocal_main_window{ nullptr };

static void callback_character(GLFWwindow*, const unsigned int codepoint)
{
	if (threadlocal_main_window != nullptr) {
		threadlocal_main_window->callback_character(codepoint);
	}
}

static void callback_cursor_pos(GLFWwindow*, const double x, const double y)
{
	if (threadlocal_main_window != nullptr) {
		threadlocal_main_window->callback_cursor_pos(x, y);
	}
}

static void callback_key(GLFWwindow*, const int key, const int scancode, const int action, const int mods)
{
	if (threadlocal_main_window != nullptr) {
		threadlocal_main_window->callback_key(key, scancode, action, mods);
	}
}

static void callback_mouse_button(GLFWwindow*, const int button, const int action, const int mods)
{
	if (threadlocal_main_window != nullptr) {
		threadlocal_main_window->callback_mouse_button(button, action, mods);
	}
}

static void callback_scroll(GLFWwindow*, const double xoffset, const double yoffset)
{
	if (threadlocal_main_window != nullptr) {
		threadlocal_main_window->callback_scroll(xoffset, yoffset);
	}
}

void cse::register_window_pair_for_callbacks(GLFWwindow* const glfw_window, MainWindow* const main_window)
{
	threadlocal_main_window = main_window;
	glfwSetCharCallback(glfw_window, callback_character);
	glfwSetCursorPosCallback(glfw_window, callback_cursor_pos);
	glfwSetKeyCallback(glfw_window, callback_key);
	glfwSetMouseButtonCallback(glfw_window, callback_mouse_button);
	glfwSetScrollCallback(glfw_window, callback_scroll);
}
