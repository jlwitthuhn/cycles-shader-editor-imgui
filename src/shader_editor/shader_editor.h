#pragma once

/**
 * @file
 * @brief Defines the main class to be used by consumers of this library.
 */

#include <memory>
#include <string>

namespace cse {

	class ShaderGraphEditorImpl;

	/**
	 * @brief Used to create and manage a Shader Graph Editor window.
	 */
	class ShaderGraphEditor {
	public:
		ShaderGraphEditor();
		~ShaderGraphEditor();

		void load_graph(std::string graph);

		bool running() const;

		bool open_window();
		void wait();

		bool has_new_data();
		std::string get_serialized_graph();

		void force_close();

	private:
		std::unique_ptr<ShaderGraphEditorImpl> impl;
	};
}
