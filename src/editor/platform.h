#pragma once

/**
 * @file
 * @brief Defines an abstraction over all platform-specific functionality.
 */

#include <string>

#include <boost/optional.hpp>

namespace cse {
	namespace Platform {
		bool save_graph_dialog(std::string graph);
		boost::optional<std::string> load_graph_dialog();
	}
}
