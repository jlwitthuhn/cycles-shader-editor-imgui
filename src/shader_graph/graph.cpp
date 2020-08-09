#include "graph.h"

#include <cassert>
#include <list>
#include <set>

#include <boost/optional.hpp>

#include "shader_core/vector.h"

#include "node.h"
#include "node_type.h"
#include "serialize.h"
#include "slot.h"

template <typename TSlot, typename TRaw> bool set_graph_value(
	const std::map<csg::NodeId, std::shared_ptr<csg::Node>>& nodes_by_id,
	const csg::SlotId slot_id,
	const TRaw new_value)
{
	if (nodes_by_id.count(slot_id.node_id()) == 0) {
		return false;
	}

	const std::shared_ptr<csg::Node> node{ nodes_by_id.at(slot_id.node_id()) };
	if (node.use_count() == 0) {
		return false;
	}

	const auto opt_slot{ node->slot(slot_id.index()) };
	if (opt_slot.has_value() == false) {
		return false;
	}

	csg::Slot& slot = node->slot_ref(slot_id.index());
	if (slot.value.has_value() == false) {
		return false;
	}

	const boost::optional<TSlot> opt_old_value = slot.value->as<TSlot>();
	if (opt_old_value.has_value() == false) {
		return false;
	}

	const TSlot old_value = opt_old_value.value();
	TSlot maybe_new_value{ old_value };
	maybe_new_value.set(new_value);

	if (maybe_new_value != old_value) {
		slot.value = maybe_new_value;
		return true;
	}
	else {
		return false;
	}
}

bool csg::Connection::operator<(const Connection& other) const
{
	if (_source < other._source) return true;
	if (other._source < _source) return false;

	if (_dest < other._dest) return true;
	if (other._dest < _dest) return false;

	return false;
}

boost::optional<csg::Graph> csg::Graph::from(const std::string& graph_string)
{
	return deserialize_graph(graph_string);
}

csg::Graph::Graph(const GraphType type)
{
	if (type == GraphType::MATERIAL) {
		add(NodeType::MATERIAL_OUTPUT, csc::Int2{});
	}
}

csg::Graph& csg::Graph::operator=(const Graph& other)
{
	// Destroy all shared_ptrs, the objects they point to do not belong to the this graph anymore
	_nodes.clear();
	nodes_by_id.clear();

	for (const std::shared_ptr<Node>& node : other.nodes()) {
		// Make a new shared_ptr with an equivalent but distinct object
		const std::shared_ptr<Node> new_node{ std::make_shared<Node>(*node) };
		_nodes.push_back(new_node);
		nodes_by_id[new_node->id()] = new_node;
	}

	_connections = other._connections;

	return *this;
}

std::shared_ptr<const csg::Node> csg::Graph::get(const NodeId id) const
{
	if (nodes_by_id.count(id)) {
		return std::const_pointer_cast<const Node>(nodes_by_id.at(id));
	}
	else {
		return std::shared_ptr<const csg::Node>{};
	}
}

boost::optional<csg::SlotValue> csg::Graph::get_slot_value(SlotId slot_id) const
{
	const auto opt_node{ get(slot_id.node_id()) };
	if (opt_node) {
		return opt_node->slot_value(slot_id.index());
	}
	return boost::none;
}

csg::NodeId csg::Graph::add(const NodeType type, const csc::Int2 pos)
{
	while (true) {
		const std::shared_ptr<Node> new_node{ std::make_shared<Node>(type, pos) };
		if (contains(new_node->id()) == false) {
			_nodes.push_front(new_node);
			nodes_by_id[new_node->id()] = new_node;
			return new_node->id();
		}
	}
}

bool csg::Graph::add(const NodeType type, const csc::Int2 pos, const NodeId node_id)
{
	const std::shared_ptr<Node> new_node{ std::make_shared<Node>(type, pos, node_id) };
	if (contains(new_node->id()) == false) {
		_nodes.push_front(new_node);
		nodes_by_id[new_node->id()] = new_node;
		return true;
	}
	else {
		return false;
	}
}

void csg::Graph::remove(const std::set<NodeId>& ids)
{
	for (auto iter{ _nodes.begin() }; iter != _nodes.end(); ) {
		const auto this_node{ *iter };
		const bool is_deletable{ csg::NodeTypeInfo::from(this_node->type())->category() != csg::NodeCategory::OUTPUT };
		if (is_deletable && ids.count(this_node->id())) {
			nodes_by_id.erase(this_node->id());
			iter = _nodes.erase(iter);
		}
		else {
			++iter;
		}
	}
}

boost::optional<csg::NodeId> csg::Graph::duplicate(const NodeId node_id)
{
	const csc::Int2 duplicate_offset{ 20, 20 };

	if (nodes_by_id.count(node_id) == 0) {
		// node_id is invalid, do nothing
		return boost::none;
	}

	const std::shared_ptr<Node> old_node{ nodes_by_id[node_id] };
	const boost::optional<NodeTypeInfo> old_type_info{ NodeTypeInfo::from(old_node->type()) };
	assert(old_type_info.has_value());
	if (old_type_info->allow_creation() == false) {
		// This node cannot be duplicated
		return boost::none;
	}

	const NodeId new_node_id{ add(old_node->type(), old_node->position + duplicate_offset) };
	const std::shared_ptr<Node> new_node{ nodes_by_id[new_node_id] };
	new_node->copy_from(*old_node);
	return new_node_id;
}

bool csg::Graph::add_connection(const SlotId source, const SlotId dest)
{
	if (source.node_id() == dest.node_id()) {
		return false;
	}
	const std::shared_ptr<const Node> source_node = get(source.node_id());
	const std::shared_ptr<const Node> dest_node = get(dest.node_id());
	if (source_node.use_count() == 0 || dest_node.use_count() == 0) {
		return false;
	}
	if (source_node->has_pin(source.index(), SlotDirection::OUTPUT) == false) {
		return false;
	}
	if (dest_node->has_pin(dest.index(), SlotDirection::INPUT) == false) {
		return false;
	}

	// Add new connection
	boost::optional<Connection> removed_connection{ remove_connection(dest) };
	_connections.push_back(Connection{ source, dest });

	// If this change made the graph contain a cycle, roll back graph to previous state
	if (contains_cycle()) {
		_connections.pop_back();
		if (removed_connection) {
			_connections.push_back(*removed_connection);
		}
		return false;
	}
	else {
		return true;
	}
}

boost::optional<csg::Connection> csg::Graph::remove_connection(const SlotId dest)
{
	for (auto conn_iter{ _connections.begin() }; conn_iter != _connections.end(); conn_iter++) {
		if (conn_iter->dest() == dest) {
			Connection result = *conn_iter;
			_connections.erase(conn_iter);
			return result;
		}
	}
	return boost::none;
}

bool csg::Graph::set_bool(const SlotId slot_id, const bool new_value)
{
	return set_graph_value<BoolSlotValue>(nodes_by_id, slot_id, new_value);
}

bool csg::Graph::set_color(const SlotId slot_id, const csc::Float3 new_value)
{
	return set_graph_value<ColorSlotValue>(nodes_by_id, slot_id, new_value);
}

bool csg::Graph::set_enum(const SlotId slot_id, const size_t new_value)
{
	return set_graph_value<EnumSlotValue>(nodes_by_id, slot_id, new_value);
}

bool csg::Graph::set_float(const SlotId slot_id, const float new_value)
{
	return set_graph_value<FloatSlotValue>(nodes_by_id, slot_id, new_value);
}

bool csg::Graph::set_int(const SlotId slot_id, const int new_value)
{
	return set_graph_value<IntSlotValue>(nodes_by_id, slot_id, new_value);
}

bool csg::Graph::set_vector(const SlotId slot_id, const csc::Float3 new_value)
{
	return set_graph_value<VectorSlotValue>(nodes_by_id, slot_id, new_value);
}

bool csg::Graph::set_color_ramp(const SlotId slot_id, const ColorRampSlotValue& new_value)
{
	return set_graph_value<ColorRampSlotValue>(nodes_by_id, slot_id, new_value);
}

bool csg::Graph::set_curve_rgb(const SlotId slot_id, const RGBCurveSlotValue& new_value)
{
	return set_graph_value<RGBCurveSlotValue>(nodes_by_id, slot_id, new_value);
}

bool csg::Graph::set_curve_vec(const SlotId slot_id, const VectorCurveSlotValue& new_value)
{
	return set_graph_value<VectorCurveSlotValue>(nodes_by_id, slot_id, new_value);
}

void csg::Graph::move(const std::set<NodeId>& ids, const csc::Float2 delta)
{
	for (const NodeId id : ids) {
		if (nodes_by_id.count(id)) {
			const auto ptr{ nodes_by_id[id] };
			const csc::Float2 current_pos{ ptr->position };
			const csc::Float2 new_pos{ current_pos + delta };
			ptr->position = csc::Int2{ new_pos };
		}
	}
}

void csg::Graph::raise(const NodeId id)
{
	if (contains(id) == false) {
		return;
	}
	std::shared_ptr<Node> the_node;
	for (auto iter{ _nodes.begin() }; iter != _nodes.end(); iter++) {
		const NodeId this_id{ (*iter)->id() };
		if (this_id == id) {
			the_node = *iter;
			_nodes.erase(iter);
			break;
		}
	}
	if (the_node) {
		_nodes.push_front(the_node);
	}
}

bool csg::Graph::contains(const NodeId id) const
{
	return (nodes_by_id.count(id) > 0);
}

std::string csg::Graph::serialize() const
{
	return csg::serialize_graph(*this);
}

bool csg::Graph::operator==(const Graph& other) const
{
	const bool size_match_nodes{ _nodes.size() == other._nodes.size() };
	const bool size_match_conns{ _connections.size() == other._connections.size() };
	if (!size_match_nodes || !size_match_conns) {
		return false;
	}

	// Check that all connections match (order does not matter)
	{
		const std::set<Connection> source_connections{ _connections.cbegin(), _connections.cend() };
		// For each connection in other, make sure it exists in the source set
		for (const Connection this_conn : other._connections) {
			if (source_connections.count(this_conn) == 0) {
				return false;
			}
		}
	}

	// Check that all nodes match (order does not matter)
	{
		for (const std::shared_ptr<Node>& this_node : _nodes) {
			const std::shared_ptr<const Node> other_node{ other.get(this_node->id()) };
			if (other_node.use_count() == 0) {
				return false;
			}
			if (*this_node != *other_node) {
				return false;
			}
		}
	}

	return true;
}

bool csg::Graph::contains_cycle() const
{
	// For each node, follow the graph and record visited nodes
	// If a visited node reappears, then a cycle exists
	std::set<NodeId> good_nodes;
	for (const std::shared_ptr<Node> root_node : _nodes) {
		std::set<NodeId> visited_nodes;
		std::list<NodeId> pending_nodes;
		pending_nodes.push_back(root_node->id());
		while (pending_nodes.size() > 0) {
			const NodeId this_node_id{ pending_nodes.front() };
			pending_nodes.pop_front();
			if (good_nodes.count(this_node_id)) {
				// This node is known to not be part of a cycle
				// No need to search down this path again
				continue;
			}
			if (visited_nodes.count(this_node_id)) {
				// This node has been visited before, a cycle exists
				return true;
			}
			for (const Connection this_conn : _connections) {
				if (this_conn.source().node_id() == this_node_id) {
					pending_nodes.push_back(this_conn.dest().node_id());
				}
			}
			visited_nodes.insert(this_node_id);
		}

		// The test has succeeded for this root node, mark every visited node as good
		good_nodes.insert(visited_nodes.begin(), visited_nodes.end());
	}

	return false;
}
