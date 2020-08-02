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

		bool input_updated();
		bool output_updated();

		std::string get_input_graph();
		void set_input_graph(const std::string& new_graph);

		std::string get_output_graph();
		void set_output_graph(const std::string& new_graph);

		void request_stop() { return stop.store(true); }
		bool should_stop() { return stop.load(); }

	private:

		std::mutex input_mutex;
		std::string input_graph;
		bool _input_updated{ false };

		std::mutex output_mutex;
		std::string output_graph;
		bool _output_updated{ false };

		std::atomic<bool> stop{ false };
	};
}
