#pragma once

/**
 * @file
 * @brief Defines SharedState. 
 */

#include <atomic>
#include <mutex>
#include <string>

/**
 * @brief Thread-safe class to allow the main window thread to send out a serialized graph to another thread
 */
namespace cse {
	class SharedState {
	public:

		bool updated();

		std::string get_graph();
		void set_new_graph(const std::string& new_graph);

		void request_stop();
		bool should_stop();

	private:
		bool graph_updated{ false };

		std::string graph;
		std::mutex graph_mutex;

		std::atomic<bool> stop{ false };
	};
}
