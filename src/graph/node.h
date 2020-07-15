#pragma once

/**
 * @file
 * @brief Defines Node.
 */

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include "shader_core/vector.h"

#include "node_id.h"
#include "node_type.h"
#include "slot.h"

namespace csg {

	/**
	 * @brief A single node in a shader graph.
	 */
	class Node {
	public:
		Node(NodeType type, csc::Int2 position);
		Node(NodeType type, csc::Int2 position, NodeId id);

		NodeId id() const { return _id; }
		NodeType type() const { return _type; }
		const std::vector<Slot>& slots() const { return _slots; }
		
		boost::optional<size_t> slot_index(SlotDirection dir, const std::string& slot_name) const;
		boost::optional<Slot> slot(size_t index) const;
		Slot& slot_ref(size_t index) { return _slots[index]; }

		void copy_from(const Node& other);

		bool has_pin(size_t index, SlotDirection direction) const { return index < _slots.size() && _slots[index].dir() == direction; }

		bool operator==(const Node& other) const;
		bool operator!=(const Node& other) const { return operator==(other) == false; }

		csc::Int2 position;

	private:
		NodeId roll_id();

		NodeId _id;
		NodeType _type;
		std::vector<Slot> _slots;
	};
}
