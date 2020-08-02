#include "shared_state.h"

bool cse::SharedState::input_updated()
{
	std::lock_guard<std::mutex> lock(input_mutex);
	const bool result{ _input_updated };
	return result;
}

bool cse::SharedState::output_updated()
{
	std::lock_guard<std::mutex> lock(output_mutex);
	const bool result{ _output_updated };
	return result;
}

std::string cse::SharedState::get_input_graph()
{
	std::lock_guard<std::mutex> lock(input_mutex);
	const std::string result{ input_graph };
	_input_updated = false;
	return result;
}

void cse::SharedState::set_input_graph(const std::string& new_graph)
{
	std::lock_guard<std::mutex> lock(input_mutex);
	input_graph = new_graph;
	_input_updated = true;
}

std::string cse::SharedState::get_output_graph()
{
	std::lock_guard<std::mutex> lock(output_mutex);
	const std::string result{ output_graph };
	_output_updated = false;
	return result;
}

void cse::SharedState::set_output_graph(const std::string& new_graph)
{
	std::lock_guard<std::mutex> lock(output_mutex);
	output_graph = new_graph;
	_output_updated = true;
}
