#include "shared_state.h"

bool cse::SharedState::updated()
{
	std::lock_guard<std::mutex> lock(mutex);
	const bool result{ _updated };
	return result;
}

std::string cse::SharedState::get_graph()
{
	std::lock_guard<std::mutex> lock(mutex);
	const std::string result{ graph };
	_updated = false;
	return result;
}

void cse::SharedState::set_new_graph(const std::string& new_graph)
{
	std::lock_guard<std::mutex> lock(mutex);
	graph = new_graph;
	_updated = true;
}
