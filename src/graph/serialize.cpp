#include "serialize.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <iomanip>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <utility>
#include <vector>

#include <boost/tokenizer.hpp>

#include "shader_core/config.h"
#include "shader_core/vector.h"

#include "ext_base64.h"
#include "graph.h"
#include "node.h"
#include "node_enums.h"
#include "node_id.h"
#include "node_type.h"
#include "slot.h"
#include "slot_id.h"

static const char* const MAGIC_WORD{ "cycles_shader" };
static const char* const VERSION_INPUT{ "1" };
static const char* const VERSION_OUTPUT{ "1" };

static const char* const SECTION_CONNECTIONS{ "section_connections" };
static const char* const SECTION_NODES      { "section_nodes" };

static const char* const NODE_END{ "node_end" };

static const char* const NODE_ID_PREFIX{ "_nodeid_" };

static std::string name_from_node_id(const csg::NodeId node_id)
{
	const size_t size_src{ sizeof(csg::NodeId) };
	const size_t size_dest{ ext::base64::encoded_size(size_src) };
	std::array<char, 24> result_array;
	assert(size_dest < result_array.size());
	result_array.fill('\0');
	ext::base64::encode(result_array.data(), &node_id, size_src);
	return std::string{ NODE_ID_PREFIX } + result_array.data();
}

static std::string serialize_curve(const csg::Curve& curve)
{
	constexpr char SEPARATOR{ ',' };

	const auto interp_as_char = [](const csg::CurveInterp interp) -> char
	{
		switch (interp) {
		case csg::CurveInterp::LINEAR:
			return 'l';
		case csg::CurveInterp::CUBIC_HERMITE:
			return 'h';
		default:
			return '?';
		}
	};

	std::stringstream sstream;
	sstream << std::fixed << std::setprecision(SERIALIZED_GRAPH_PRECISION);

	bool write_opening_separator{ false };
	for (const csg::CurvePoint point : curve.control_points()) {
		if (write_opening_separator) {
			sstream << SEPARATOR;
		}
		sstream << point.pos.x << SEPARATOR << point.pos.y << SEPARATOR << interp_as_char(point.interp);
		write_opening_separator = true;
	}
	return sstream.str();
}

static std::string serialize_slot_value(const csg::SlotValue& slot_value)
{
	switch (slot_value.type()) {
	case csg::SlotType::BOOL:
		if (slot_value.as<csg::BoolSlotValue>().has_value()) {
			const csg::BoolSlotValue bool_slot_value{ slot_value.as<csg::BoolSlotValue>().value() };
			if (bool_slot_value.get()) {
				return "1";
			}
			else {
				return "0";
			}
		}
		break;
	case csg::SlotType::COLOR:
		if (slot_value.as<csg::ColorSlotValue>().has_value()) {
			const csg::ColorSlotValue color_slot_value{ slot_value.as<csg::ColorSlotValue>().value() };
			const csc::Float3 color{ color_slot_value.get() };
			std::stringstream sstream;
			sstream << std::fixed << std::setprecision(SERIALIZED_GRAPH_PRECISION) << color.x << ',' << color.y << ',' << color.z;
			return sstream.str();
		}
		break;
	case csg::SlotType::ENUM:
		if (slot_value.as<csg::EnumSlotValue>().has_value()) {
			const csg::EnumSlotValue enum_slot_value{ slot_value.as<csg::EnumSlotValue>().value() };
			return std::string{ enum_slot_value.internal_name() };
		}
		break;
	case csg::SlotType::FLOAT:
		if (slot_value.as<csg::FloatSlotValue>().has_value()) {
			const csg::FloatSlotValue float_slot_value{ slot_value.as<csg::FloatSlotValue>().value() };
			std::stringstream sstream;
			sstream << std::fixed << std::setprecision(SERIALIZED_GRAPH_PRECISION) << float_slot_value.get();
			return sstream.str();
		}
		break;
	case csg::SlotType::VECTOR:
		if (slot_value.as<csg::VectorSlotValue>().has_value()) {
			const csg::VectorSlotValue vec_slot_value{ slot_value.as<csg::VectorSlotValue>().value() };
			const csc::Float3 color{ vec_slot_value.get() };
			std::stringstream sstream;
			sstream << std::fixed << std::setprecision(SERIALIZED_GRAPH_PRECISION) << color.x << ',' << color.y << ',' << color.z;
			return sstream.str();
		}
		break;
	case csg::SlotType::CURVE_VECTOR:
		if (slot_value.as<csg::VectorCurveSlotValue>().has_value()) {
			constexpr char CURVE_SEPARATOR{ '/' };
			const csg::VectorCurveSlotValue curve_slot_value{ slot_value.as<csg::VectorCurveSlotValue>().value() };
			std::stringstream sstream;
			sstream << std::fixed << std::setprecision(SERIALIZED_GRAPH_PRECISION);
			sstream << "curve_vec_00" << CURVE_SEPARATOR << "00" << CURVE_SEPARATOR;
			sstream << curve_slot_value.get_min().x << "," << curve_slot_value.get_min().y << CURVE_SEPARATOR;
			sstream << curve_slot_value.get_max().x << "," << curve_slot_value.get_max().y << CURVE_SEPARATOR;
			sstream << serialize_curve(curve_slot_value.get_x()) << CURVE_SEPARATOR;
			sstream << serialize_curve(curve_slot_value.get_y()) << CURVE_SEPARATOR;
			sstream << serialize_curve(curve_slot_value.get_z());
			return sstream.str();
		}
		break;
	default:
		assert(false);
		break;
	}

	return "ERROR";
}

std::string csg::serialize_graph(const Graph& graph)
{
	// Make a local sorted copy of all nodes and connections
	const auto& graph_connections = graph.connections();
	std::vector<Connection> connections{ graph_connections.begin(), graph_connections.end() };
	std::sort(connections.begin(), connections.end());

	std::vector<Node> nodes;
	for (const auto& node : graph.nodes()) {
		nodes.push_back(*node);
	}
	std::sort(nodes.begin(), nodes.end(),
		[](const Node& a, const Node& b) {
			return a.id() < b.id();
		}
	);

	std::stringstream result_stream;

	// Header
	result_stream << MAGIC_WORD << "|" << VERSION_OUTPUT << "|";

	// Node section
	result_stream << SECTION_NODES << "|";
	std::map<NodeId, std::string> names_by_id;
	for (const Node& node : nodes) {
		const std::string node_name{ name_from_node_id(node.id()) };
		names_by_id[node.id()] = node_name;
		const boost::optional<NodeTypeInfo> info{ NodeTypeInfo::from(node.type()) };
		if (info.has_value()) {
			result_stream << info->name() << "|" << node_name << "|";
			result_stream << node.position.x << "|" << node.position.y << "|";
			for (const Slot& slot : node.slots()) {
				if (slot.dir() == SlotDirection::INPUT && slot.value.has_value()) {
					result_stream << slot.name() << "|" << serialize_slot_value(slot.value.value()) << "|";
				}
			}
			result_stream << "node_end|";
		}
		else {
			continue;
		}
	}

	// Connection section
	result_stream << SECTION_CONNECTIONS << "|";

	for (const Connection& connection : connections) {
		const csg::NodeId id_src{ connection.source().node_id() };
		const csg::NodeId id_dest{ connection.dest().node_id() };
		if (names_by_id.count(id_src) == 0 || names_by_id.count(id_dest) == 0) {
			// Either source or dest node does not exist, ignore this connection
			continue;
		}
		const std::string name_src{ names_by_id[id_src] };
		const std::string name_dest{ names_by_id[id_dest] };
		const auto opt_node_src{ graph.get(id_src) };
		const auto opt_node_dest{ graph.get(id_dest) };

		// We already checked that these nodes exist, so the optionals here should always hold a value
		assert(opt_node_src.has_value());
		assert(opt_node_dest.has_value());

		// Now we need to get the slots to find the slot names
		const auto opt_slot_src{ (*opt_node_src)->slot(connection.source().index()) };
		const auto opt_slot_dest{ (*opt_node_dest)->slot(connection.dest().index()) };
		if (opt_slot_src.has_value() == false || opt_slot_dest.has_value() == false) {
			// One of the slots is not real, ignore this connection
			continue;
		}

		result_stream << name_src << "|" << opt_slot_src->disp_name() << "|";
		result_stream << name_dest << "|" << opt_slot_dest->disp_name() << "|";
	}

	return result_stream.str();
}

// Check whether iter is able to successfully increment count times without becoming equal to end
template<typename T>
static bool iter_has_contents(T iter, const T end, size_t count)
{
	while (count > 0) {
		if (iter == end) {
			return false;
		}
		count--;
		iter++;
	}
	return true;
}

// Normal stoi/stof will use system locale which may be a problem, this way uses default locale

static int my_stoi(const std::string& input)
{
	std::stringstream stream{ input };
	int result{ 0 };
	stream >> result;
	return result;
}

static float my_stof(const std::string& input)
{
	std::stringstream stream{ input };
	float result{ 0.0f };
	stream >> result;
	return result;
}

static csc::Float2 my_stof2(const std::string& input)
{
	const boost::char_separator<char> sep{ "," };
	const boost::tokenizer<boost::char_separator<char>> tokenizer{ input, sep };

	auto token_iter = tokenizer.begin();
	if (iter_has_contents(token_iter, tokenizer.end(), 2) == false) {
		// Did not find the two expected comma-separated floats
		return csc::Float2{};
	}

	csc::Float2 result;
	result.x = my_stof(*token_iter++);
	result.y = my_stof(*token_iter++);
	return result;
}

static csc::Float3 my_stof3(const std::string& input)
{
	const boost::char_separator<char> sep{ "," };
	const boost::tokenizer<boost::char_separator<char>> tokenizer{ input, sep };

	auto token_iter = tokenizer.begin();
	if (iter_has_contents(token_iter, tokenizer.end(), 3) == false) {
		// Did not find the three expected comma-separated floats
		return csc::Float3{};
	}

	csc::Float3 result;
	result.x = my_stof(*token_iter++);
	result.y = my_stof(*token_iter++);
	result.z = my_stof(*token_iter++);
	return result;
}

static boost::optional<csg::NodeId> node_id_from_name(const std::string& node_name)
{
	if (node_name.find(NODE_ID_PREFIX) != 0) {
		// This name does not have the right prefix
		return boost::none;
	}

	const size_t prefix_size = strlen(NODE_ID_PREFIX);
	const std::string id_encoded{ node_name.substr(prefix_size, node_name.size() - prefix_size) };
	std::array<char, 32> buffer;
	buffer.fill('\0');
	const std::pair<size_t, size_t> decode_result = ext::base64::decode(buffer.data(), id_encoded.c_str(), std::min(static_cast<size_t>(16), id_encoded.size()));
	if (decode_result.first < sizeof(csg::NodeId)) {
		// ID was not long enough
		return boost::none;
	}

	const csg::NodeId result{ *( reinterpret_cast<csg::NodeId*>( buffer.data() ) ) };

	return result;
}

static boost::optional<csg::NodeType> get_type_from_name(const std::string& type_name)
{
	const boost::optional<csg::NodeTypeInfo> opt_type_info{ csg::NodeTypeInfo::from(type_name.c_str()) };
	if (opt_type_info.has_value()) {
		return opt_type_info->type();
	}
	else {
		return boost::none;
	}
}

static csg::CurveInterp get_interp(const std::string symbol)
{
	if (symbol == "l") {
		return csg::CurveInterp::LINEAR;
	}
	else {
		return csg::CurveInterp::CUBIC_HERMITE;
	}
}

static boost::optional<csg::Curve> deserialize_curve(const std::string& curve, const csc::Float2 min, const csc::Float2 max)
{
	const boost::char_separator<char> sep{ "," };
	const boost::tokenizer<boost::char_separator<char>> tokenizer{ curve, sep };

	auto token_iter = tokenizer.begin();

	std::vector<csg::CurvePoint> points;
	while (iter_has_contents(token_iter, tokenizer.end(), 3)) {
		const float x{ my_stof(*token_iter++) };
		const float y{ my_stof(*token_iter++) };
		const csg::CurveInterp interp{ get_interp(*token_iter++) };
		points.push_back(csg::CurvePoint{ csc::Float2{ x, y }, interp });
	}

	if (points.size() < 2) {
		return boost::none;
	}
	else {
		return csg::Curve{ min, max, points };
	}
}

static boost::optional<csg::VectorCurveSlotValue> deserialize_vector_curve(const std::string& vector_curve)
{
	const boost::char_separator<char> sep{ "/" };
	const boost::tokenizer<boost::char_separator<char>> tokenizer{ vector_curve, sep };

	auto token_iter = tokenizer.begin();

	if (iter_has_contents(token_iter, tokenizer.end(), 7) == false) {
		return boost::none;
	}

	const std::string curve_type{ *token_iter++ };
	const std::string curve_version{ *token_iter++ };
	const csc::Float2 min{ my_stof2(*token_iter++) };
	const csc::Float2 max{ my_stof2(*token_iter++) };

	// Check validity of min and max before proceeding
	if (min.x >= max.x || min.y >= max.y) {
		return boost::none;
	}

	const boost::optional<csg::Curve> opt_x{ deserialize_curve(*token_iter++, min, max) };
	const boost::optional<csg::Curve> opt_y{ deserialize_curve(*token_iter++, min, max) };
	const boost::optional<csg::Curve> opt_z{ deserialize_curve(*token_iter++, min, max) };

	csg::VectorCurveSlotValue result{ min, max };
	if (opt_x) {
		result.set_x(*opt_x);
	}
	if (opt_y) {
		result.set_y(*opt_y);
	}
	if (opt_z) {
		result.set_z(*opt_z);
	}

	return result;
}

boost::optional<csg::Graph> csg::deserialize_graph(const std::string& graph_string)
{
	const boost::char_separator<char> sep{ "|" };
	const boost::tokenizer<boost::char_separator<char>> tokenizer{ graph_string, sep };

	auto token_iter = tokenizer.begin();

	if (token_iter == tokenizer.end() || *token_iter != MAGIC_WORD) {
		return boost::none;
	}
	else {
		++token_iter;
	}

	if (token_iter == tokenizer.end() || *token_iter != VERSION_INPUT) {
		return boost::none;
	}
	else {
		++token_iter;
	}

	csg::Graph result{ GraphType::EMPTY };

	// Advance iterator until we find the start of the node section
	while (token_iter != tokenizer.end() && *token_iter != SECTION_NODES) {
		token_iter++;
	}

	if (token_iter == tokenizer.end()) {
		// No nodes or connection section in the input, return empty graph
		return result;
	}

	// Map to let us look up node ids by name
	// This will be needed for building connections later in this function
	std::map<std::string, NodeId> ids_by_name;

	// Advance token to the start of the first node
	token_iter++;

	// Loop adding nodes until we see the connections header
	while (token_iter != tokenizer.end() && *token_iter != SECTION_CONNECTIONS) {
		constexpr size_t NODE_MIN_TOKENS{ 5 }; // type, name, x, y, node_end
		if (iter_has_contents(token_iter, tokenizer.end(), NODE_MIN_TOKENS) == false) {
			// Not enough tokens exist to form a node, end here
			return result;
		}
		assert(token_iter != tokenizer.end());
		const std::string type_code{ *token_iter++ };
		assert(token_iter != tokenizer.end());
		const std::string node_name{ *token_iter++ };
		assert(token_iter != tokenizer.end());
		const int x{ my_stoi(*token_iter++) };
		assert(token_iter != tokenizer.end());
		const int y{ my_stoi(*token_iter++) };

		const boost::optional<NodeType> opt_node_type{ get_type_from_name(type_code) };
		if (opt_node_type.has_value() == false) {
			// We do not recognize this type code
			// Advance the iterator past this node and continue
			while (token_iter != tokenizer.end() && *token_iter != NODE_END) {
				token_iter++;
			}
			if (*token_iter == NODE_END) {
				token_iter++;
			}
			continue;
		}

		const boost::optional<NodeId> opt_node_id{ node_id_from_name(node_name) };
		NodeId node_id;
		if (opt_node_id.has_value()) {
			node_id = opt_node_id.value();
			if (result.add(opt_node_type.value(), csc::Int2{ x, y }, node_id) == false) {
				// This node was not added because it is a duplicate id
				// This can randomly happen but should be very rare (birthday problem with a 64 bit number)
				// except in malformed graphs
				return boost::none;
			}
		}
		else {
			node_id = result.add(opt_node_type.value(), csc::Int2{ x, y });
		}
		
		if (ids_by_name.count(node_name) != 0) {
			// This name has already been used
			// The graph is invalid, abort processing here
			return boost::none;
		}
		ids_by_name[node_name] = node_id;

		// Load in all input/value pairs
		while (token_iter != tokenizer.end() && *token_iter != NODE_END && iter_has_contents(token_iter, tokenizer.end(), 2)) {
			assert(token_iter != tokenizer.end());
			const std::string input_name{ *token_iter++ };
			assert(token_iter != tokenizer.end());
			const std::string input_value{ *token_iter++ };

			const auto opt_node{ result.get(node_id) };
			assert(opt_node.has_value());
			const std::shared_ptr<const Node> node{ *opt_node };

			const boost::optional<size_t> opt_slot_index{ node->slot_index(SlotDirection::INPUT, input_name) };
			if (opt_slot_index.has_value()) {
				const boost::optional<Slot> opt_slot = node->slot(*opt_slot_index);
				assert(opt_slot.has_value());
				if (opt_slot->value.has_value()) {
					const SlotId slot_id{ node->id(), *opt_slot_index };
					// Choose how we interpret 'input_value' based on the slot type
					switch (opt_slot.value().type()) {
					case SlotType::BOOL:
					{
						const bool bool_value{ static_cast<bool>(my_stoi(input_value)) };
						result.set_bool(slot_id, bool_value);
						break;
					}
					case SlotType::COLOR:
					{
						const csc::Float3 float3_value{ my_stof3(input_value) };
						result.set_color(slot_id, float3_value);
						break;
					}
					case SlotType::ENUM:
					{
						const boost::optional<EnumSlotValue> slot_value{ opt_slot->value->as<EnumSlotValue>() };
						if (slot_value) {
							const NodeMetaEnum meta_enum{ slot_value->get_meta() };
							assert(NodeEnumInfo::from(meta_enum).has_value());
							const NodeEnumInfo enum_info{ NodeEnumInfo::from(meta_enum).value() };
							// Loop though all available internal names to look for a match
							for (size_t i = 0; i < enum_info.count(); i++) {
								assert(NodeEnumOptionInfo::from(meta_enum, i).has_value());
								const NodeEnumOptionInfo option_info{ NodeEnumOptionInfo::from(meta_enum, i).value() };
								if (input_value == option_info.internal_name()) {
									result.set_enum(slot_id, i);
									break;
								}
							}
						}
						break;
					}
					case SlotType::FLOAT:
					{
						const float float_value{ my_stof(input_value) };
						result.set_float(slot_id, float_value);
						break;
					}
					case SlotType::VECTOR:
					{
						const csc::Float3 float3_value{ my_stof3(input_value) };
						result.set_vector(slot_id, float3_value);
						break;
					}
					case SlotType::CURVE_VECTOR:
					{
						const boost::optional<csg::VectorCurveSlotValue> opt_curve_value{ deserialize_vector_curve(input_value) };
						if (opt_curve_value) {
							result.set_curve_vec(slot_id, *opt_curve_value);
						}
						break;
					}
					default:
						// Not a type we know how to parse from a string, do nothing
						break;
					}
				}
			}
		}

		// Advance to one past the next NODE_END and continue loop
		while (token_iter != tokenizer.end() && *token_iter != NODE_END) {
			token_iter++;
		}
		if (*token_iter == NODE_END) {
			token_iter++;
		}
	}

	// Advance iterator until we find the start of the connection section
	while (token_iter != tokenizer.end() && *token_iter != SECTION_CONNECTIONS) {
		token_iter++;
	}

	if (token_iter == tokenizer.end()) {
		// No connections, return graph so far
		return result;
	}

	// Advance token to the start of the first connection
	token_iter++;

	while (token_iter != tokenizer.end()) {
		constexpr size_t CONNECTION_TOKENS{ 4 };
		if (iter_has_contents(token_iter, tokenizer.end(), CONNECTION_TOKENS) == false) {
			// Not enough tokens left for a full connection, end early
			break;
		}

		const std::string name_src{ *(token_iter++) };
		const std::string slot_src{ *(token_iter++) };
		const std::string name_dst{ *(token_iter++) };
		const std::string slot_dst{ *(token_iter++) };
		if (ids_by_name.count(name_src) == 0 || ids_by_name.count(name_dst) == 0) {
			// Name does not reference a real node, skip this connection
			continue;
		}
		const NodeId id_src{ ids_by_name[name_src] };
		const NodeId id_dst{ ids_by_name[name_dst] };

		// Find source slot
		const auto opt_node_src{ result.get(id_src) };
		assert(opt_node_src.has_value());
		boost::optional<size_t> slot_index_src;
		{
			size_t current_index{ 0 };
			for (const Slot this_slot : (*opt_node_src)->slots()) {
				if (slot_src == this_slot.disp_name()) {
					slot_index_src = current_index;
					break;
				}
				current_index++;
			}
		}

		// Find Dest slot
		const auto opt_node_dst{ result.get(id_dst) };
		assert(opt_node_dst.has_value());
		boost::optional<size_t> slot_index_dst;
		{
			size_t current_index{ 0 };
			for (const Slot this_slot : (*opt_node_dst)->slots()) {
				if (slot_dst == this_slot.disp_name()) {
					slot_index_dst = current_index;
					break;
				}
				current_index++;
			}
		}

		if (slot_index_src.has_value() == false || slot_index_dst.has_value() == false) {
			continue;
		}

		const SlotId src_slot_id{ id_src, slot_index_src.value() };
		const SlotId dst_slot_id{ id_dst, slot_index_dst.value() };

		result.add_connection(src_slot_id, dst_slot_id);

	}

	return result;
}
