#pragma once

/**
 * @file
 * @brief Defines SharedState. 
 */

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

	private:
		bool _updated{ false };
		std::string graph;

		std::mutex mutex;
	};
}
