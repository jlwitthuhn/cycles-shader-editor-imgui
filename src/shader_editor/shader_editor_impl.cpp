#include "shader_editor_impl.h"

#include <memory>
#include <string>

#include <GLFW/glfw3.h>
#include <imgui.h>

#include "main_window.h"
#include "shared_state.h"

static void thread_func(const std::shared_ptr<cse::SharedState> shared_state, bool* const running)
{
	auto main_window{ std::make_unique<cse::MainWindow>(shared_state) };
	while (main_window->should_close() == false && shared_state->should_stop() == false) {
		if (shared_state->input_updated()) {
			main_window->load_graph(shared_state->get_input_graph());
		}
		main_window->event_loop_iteration();
	}
	*running = false;
}

cse::ShaderGraphEditorImpl::ShaderGraphEditorImpl() :
	shared_state{ std::make_shared<SharedState>() }
{
	if (IMGUI_CHECKVERSION() == false) {
		return;
	}
	if (glfwInit() == GLFW_FALSE) {
		return;
	}
	initialized = true;
}

cse::ShaderGraphEditorImpl::~ShaderGraphEditorImpl()
{
	if (window_thread.joinable()) {
		window_thread.join();
	}
}

void cse::ShaderGraphEditorImpl::load_graph(const std::string graph)
{
	shared_state->set_input_graph(graph);
}

bool cse::ShaderGraphEditorImpl::running() const
{
	return _running;
}

bool cse::ShaderGraphEditorImpl::open_window()
{
	if (initialized) {
		_running = true;
		window_thread = std::thread{ thread_func, shared_state, &_running };
		return true;
	}
	else {
		return false;
	}
}

void cse::ShaderGraphEditorImpl::wait()
{
	if (window_thread.joinable()) {
		window_thread.join();
	}
}

bool cse::ShaderGraphEditorImpl::has_new_data()
{
	return shared_state->output_updated();
}

std::string cse::ShaderGraphEditorImpl::get_serialized_graph()
{
	return shared_state->get_output_graph();
}

void cse::ShaderGraphEditorImpl::force_close()
{
	shared_state->request_stop();
}
