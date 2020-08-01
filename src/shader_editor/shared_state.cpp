#include "shared_state.h"

bool cse::SharedState::updated()
{
	std::lock_guard<std::mutex> lock(graph_mutex);
	const bool result{ graph_updated };
	return result;
}

std::string cse::SharedState::get_graph()
{
	std::lock_guard<std::mutex> lock(graph_mutex);
	const std::string result{ graph };
	graph_updated = false;
	return result;
}

void cse::SharedState::set_new_graph(const std::string& new_graph)
{
	std::lock_guard<std::mutex> lock(graph_mutex);
	graph = new_graph;
	graph_updated = true;
}

void cse::SharedState::request_stop()
{
	stop.store(true);
}

bool cse::SharedState::should_stop()
{
	return stop.load();
}
