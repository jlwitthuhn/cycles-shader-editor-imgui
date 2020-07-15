#pragma once

/**
 * @file
 * @brief Defines functions that allow a MainWindow to be hooked up with glfw callbacks.
 */

struct GLFWwindow;

namespace cse {

	class MainWindow;

	void register_window_pair_for_callbacks(GLFWwindow* glfw_window, MainWindow* main_window);

}
