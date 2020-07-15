#pragma once

/**
 * @file
 * @brief Defines ShaderGraphEditorImpl.
 */

#include <memory>
#include <string>
#include <thread>

namespace cse {

	class SharedState;

	/**
	 * @brief Implementation class for ShaderGraphEditor to hide internal types.
	 */
	class ShaderGraphEditorImpl {
	public:
		ShaderGraphEditorImpl();
		~ShaderGraphEditorImpl();

		bool running() const;

		bool open_window();
		void wait();

		bool has_new_data();
		std::string get_serialized_graph();

	private:
		std::shared_ptr<SharedState> shared_state;

		std::thread window_thread;
		bool _running{ false };

		bool initialized{ false };
	};
}
