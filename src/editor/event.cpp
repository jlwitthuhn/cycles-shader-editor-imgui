#include "event.h"

#include <cassert>

size_t cse::InputEvent::sizeof_details()
{
	return sizeof(InputEventDetails);
}

cse::InputEvent::InputEvent(const InputCharacterDetails& input_character_details) :
	_type{ InputEventType::CHARACTER },
	details{ input_character_details }
{

}

cse::InputEvent::InputEvent(const InputKeyDetails& input_key_details) :
	_type{ InputEventType::KEY },
	details{ input_key_details }
{

}

cse::InputEvent::InputEvent(const InputMouseButtonDetails& input_mouse_button_details) :
	_type{ InputEventType::MOUSE_BUTTON },
	details{ input_mouse_button_details }
{

}

cse::InputEvent::InputEvent(const InputScrollDetails& input_scroll_details) :
	_type{ InputEventType::SCROLL },
	details{ input_scroll_details }
{

}

boost::optional<cse::InputCharacterDetails> cse::InputEvent::details_character() const
{
	if (_type == InputEventType::CHARACTER) {
		return details.character;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::InputKeyDetails> cse::InputEvent::details_key() const
{
	if (_type == InputEventType::KEY) {
		return details.key;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::InputMouseButtonDetails> cse::InputEvent::details_mouse_button() const
{
	if (_type == InputEventType::MOUSE_BUTTON) {
		return details.mouse_button;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::InputScrollDetails> cse::InputEvent::details_scroll() const
{
	if (_type == InputEventType::SCROLL) {
		return details.scroll;
	}
	else {
		return boost::none;
	}
}

size_t cse::InterfaceEvent::sizeof_details()
{
	return sizeof(InterfaceEventDetails);
}

cse::InterfaceEvent::InterfaceEvent(
	InterfaceEventType type,
	boost::optional<SubwindowId> target,
	boost::optional<std::string> message) :
	_type{ type },
	_target_subwindow{ target },
	_message{ message }
{

}

cse::InterfaceEvent::InterfaceEvent(const InterfaceEventType type, const CurveEditorModeDetails& curve_mode_details) :
	_type{ type },
	_target_subwindow{ SubwindowId::CURVE_EDITOR },
	details{ curve_mode_details }
{
	assert(curve_mode_details.matches_event(type));
}

cse::InterfaceEvent::InterfaceEvent(const InterfaceEventType type, const CurveEditorTabDetails& curve_tab_details) :
	_type{ type },
	_target_subwindow{ SubwindowId::CURVE_EDITOR },
	details{ curve_tab_details }
{
	assert(curve_tab_details.matches_event(type));
}

cse::InterfaceEvent::InterfaceEvent(const InterfaceEventType type, const SelectModeDetails& select_mode_details, const boost::optional<SubwindowId> target) :
	_type{ type },
	_target_subwindow{ target },
	details{ select_mode_details }
{
	assert(select_mode_details.matches_event(type));
}

cse::InterfaceEvent::InterfaceEvent(const InterfaceEventType type, const SubwindowIdDetails& subwindow_id_details, const boost::optional<SubwindowId> target) :
	_type{ type },
	_target_subwindow{ target },
	details{ subwindow_id_details }
{
	assert(subwindow_id_details.matches_event(type));
}

cse::InterfaceEvent::InterfaceEvent(const InterfaceEventType type, const NodeCategoryDetails& node_category_details) :
	_type{ type },
	_target_subwindow{ SubwindowId::NODE_LIST },
	details{ node_category_details }
{
	assert(node_category_details.matches_event(type));
}

cse::InterfaceEvent::InterfaceEvent(const InterfaceEventType type, const NodeTypeDetails& node_type_details) :
	_type{ type },
	_target_subwindow{ SubwindowId::NODE_LIST },
	details{ node_type_details }
{
	assert(node_type_details.matches_event(type));
}

cse::InterfaceEvent::InterfaceEvent(const InterfaceEventType type, const SlotIdDetails& slot_id_details, const boost::optional<SubwindowId> target) :
	_type{ type },
	_target_subwindow{ target },
	details{ slot_id_details }
{
	assert(slot_id_details.matches_event(type));
}

cse::InterfaceEvent::InterfaceEvent(const InterfaceEventType type, const Float3Details& float3_details, const boost::optional<SubwindowId> target) :
	_type{ type },
	_target_subwindow{ target },
	details{ float3_details }
{
	assert(float3_details.matches_event(type));
}

cse::InterfaceEvent::InterfaceEvent(const InterfaceEventType type, const FloatRectDetails& float_rect_details, const boost::optional<SubwindowId> target) :
	_type{ type },
	_target_subwindow{ target },
	details{ float_rect_details }
{
	assert(float_rect_details.matches_event(type));
}

cse::InterfaceEvent::InterfaceEvent(const InterfaceEventType type, const Int2Details& int2_details) :
	_type{ type },
	_target_subwindow{ SubwindowId::GRAPH },
	details{ int2_details }
{
	assert(int2_details.matches_event(type));
}

cse::InterfaceEvent::InterfaceEvent(const CreateNodeDetails& create_node_details) :
	_type{ InterfaceEventType::CREATE_NODE },
	_target_subwindow{ SubwindowId::GRAPH },
	details{ create_node_details }
{

}

cse::InterfaceEvent::InterfaceEvent(const SelectNodeDetails& select_node_details) :
	_type{ InterfaceEventType::SELECT_NODE },
	_target_subwindow{ SubwindowId::GRAPH },
	details{ select_node_details }
{

}

cse::InterfaceEvent::InterfaceEvent(const SetSlotBoolDetails& set_slot_bool_details) :
	_type{ InterfaceEventType::SET_SLOT_BOOL },
	_target_subwindow{ boost::none },
	details{ set_slot_bool_details }
{

}

cse::InterfaceEvent::InterfaceEvent(const SetSlotColorDetails& set_slot_color_details) :
	_type{ InterfaceEventType::SET_SLOT_COLOR },
	_target_subwindow{ boost::none },
	details{ set_slot_color_details }
{

}

cse::InterfaceEvent::InterfaceEvent(const SetSlotEnumDetails& set_slot_enum_details) :
	_type{ InterfaceEventType::SET_SLOT_ENUM },
	_target_subwindow{ boost::none },
	details{ set_slot_enum_details }
{

}

cse::InterfaceEvent::InterfaceEvent(const SetSlotFloatDetails& set_slot_float_details) :
	_type{ InterfaceEventType::SET_SLOT_FLOAT },
	_target_subwindow{ boost::none },
	details{ set_slot_float_details }
{

}

cse::InterfaceEvent::InterfaceEvent(const SetSlotRampPosDetails& set_slot_ramp_pos_details) :
	_type{ InterfaceEventType::SET_SLOT_RAMP_POS },
	_target_subwindow{ boost::none },
	details{ set_slot_ramp_pos_details }
{

}

cse::InterfaceEvent::InterfaceEvent(const SetSlotVectorDetails& set_slot_vector_details) :
	_type{ InterfaceEventType::SET_SLOT_VECTOR },
	_target_subwindow{ boost::none },
	details{ set_slot_vector_details }
{

}

cse::InterfaceEvent::InterfaceEvent(const CurveEditorViewClickDetails& curve_edit_view_details) :
	_type{ InterfaceEventType::CURVE_EDIT_VIEW_CLICK },
	_target_subwindow{ SubwindowId::CURVE_EDITOR },
	details{ curve_edit_view_details }
{

}

cse::InterfaceEvent::InterfaceEvent(const CurveEditorPointMoveDetails& curve_edit_point_move_details) :
	_type{ InterfaceEventType::CURVE_EDIT_POINT_MOVE },
	_target_subwindow{ SubwindowId::CURVE_EDITOR },
	details{ curve_edit_point_move_details }
{

}

cse::InterfaceEvent::InterfaceEvent(const CurveEditorSetInterpDetails& curve_edit_set_interp_details) :
	_type{ InterfaceEventType::CURVE_EDIT_SET_INTERP },
	_target_subwindow{ SubwindowId::CURVE_EDITOR },
	details{ curve_edit_set_interp_details }
{

}

boost::optional<cse::CurveEditorModeDetails> cse::InterfaceEvent::details_curve_editor_set_mode() const
{
	if (_type == InterfaceEventType::CURVE_EDIT_SET_MODE && details) {
		return details->curve_editor_mode;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::CurveEditorTabDetails> cse::InterfaceEvent::details_curve_editor_set_tab() const
{
	if (_type == InterfaceEventType::CURVE_EDIT_SET_TAB && details) {
		return details->curve_editor_tab;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::SelectModeDetails> cse::InterfaceEvent::details_box_select_end() const
{
	if (_type == InterfaceEventType::BOX_SELECT_END && details) {
		return details->select_mode;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::SubwindowIdDetails> cse::InterfaceEvent::details_subwindow_is_hovered() const
{
	if (_type == InterfaceEventType::SUBWINDOW_IS_HOVERED && details) {
		return details->subwindow_id;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::NodeCategoryDetails> cse::InterfaceEvent::details_select_node_category() const
{
	if (_type == InterfaceEventType::SELECT_NODE_CATEGORY && details) {
		return details->node_category;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::NodeTypeDetails> cse::InterfaceEvent::details_select_node_type() const
{
	if (_type == InterfaceEventType::SELECT_NODE_TYPE && details) {
		return details->node_type;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::SlotIdDetails> cse::InterfaceEvent::details_connection_begin() const
{
	if (_type == InterfaceEventType::CONNECTION_BEGIN && details) {
		return details->slot_id;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::SlotIdDetails> cse::InterfaceEvent::details_connection_alter() const
{
	if (_type == InterfaceEventType::CONNECTION_ALTER && details) {
		return details->slot_id;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::SlotIdDetails> cse::InterfaceEvent::details_select_slot() const
{
	if (_type == InterfaceEventType::SELECT_SLOT && details) {
		return details->slot_id;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::Float3Details> cse::InterfaceEvent::details_param_edit_color_init() const
{
	if (_type == InterfaceEventType::PARAM_EDIT_COLOR_INIT && details) {
		return details->float3;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::Float3Details> cse::InterfaceEvent::details_param_edit_color_change() const
{
	if (_type == InterfaceEventType::PARAM_EDIT_COLOR_CHANGE && details) {
		return details->float3;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::FloatRectDetails> cse::InterfaceEvent::details_curve_edit_set_bounds() const
{
	if (_type == InterfaceEventType::CURVE_EDIT_SET_BOUNDS && details) {
		return details->float_rect;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::Int2Details> cse::InterfaceEvent::details_pan_view() const
{
	if (_type == InterfaceEventType::PAN_VIEW && details) {
		return details->int2;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::CreateNodeDetails> cse::InterfaceEvent::details_create_node() const
{
	if (_type == InterfaceEventType::CREATE_NODE && details) {
		return details->create_node;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::SelectNodeDetails> cse::InterfaceEvent::details_select_node() const
{
	if (_type == InterfaceEventType::SELECT_NODE && details) {
		return details->select_node;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::SetSlotBoolDetails> cse::InterfaceEvent::details_set_slot_bool() const
{
	if (_type == InterfaceEventType::SET_SLOT_BOOL && details) {
		return details->set_slot_bool;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::SetSlotColorDetails> cse::InterfaceEvent::details_set_slot_color() const
{
	if (_type == InterfaceEventType::SET_SLOT_COLOR && details) {
		return details->set_slot_color;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::SetSlotEnumDetails> cse::InterfaceEvent::details_set_slot_enum() const
{
	if (_type == InterfaceEventType::SET_SLOT_ENUM && details) {
		return details->set_slot_enum;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::SetSlotFloatDetails> cse::InterfaceEvent::details_set_slot_float() const
{
	if (_type == InterfaceEventType::SET_SLOT_FLOAT && details) {
		return details->set_slot_float;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::SetSlotRampPosDetails> cse::InterfaceEvent::details_set_slot_ramp_pos() const
{
	if (_type == InterfaceEventType::SET_SLOT_RAMP_POS && details) {
		return details->set_slot_ramp_pos;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::SetSlotVectorDetails> cse::InterfaceEvent::details_set_slot_vector() const
{
	if (_type == InterfaceEventType::SET_SLOT_VECTOR && details) {
		return details->set_slot_vector;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::CurveEditorViewClickDetails> cse::InterfaceEvent::details_curve_edit_view_click() const
{
	if (_type == InterfaceEventType::CURVE_EDIT_VIEW_CLICK && details) {
		return details->curve_edit_view_click;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::CurveEditorPointMoveDetails> cse::InterfaceEvent::details_curve_edit_point_move() const
{
	if (_type == InterfaceEventType::CURVE_EDIT_POINT_MOVE && details) {
		return details->curve_edit_point_move;
	}
	else {
		return boost::none;
	}
}

boost::optional<cse::CurveEditorSetInterpDetails> cse::InterfaceEvent::details_curve_edit_set_interp() const
{
	if (_type == InterfaceEventType::CURVE_EDIT_SET_INTERP && details) {
		return details->curve_edit_set_interp;
	}
	else {
		return boost::none;
	}
}

void cse::InterfaceEventArray::push(const InterfaceEvent event)
{
	if (occupied >= data.size()) {
		assert(false);
	}
	data[occupied] = event;
	occupied++;
	update_usage_info();
}

void cse::InterfaceEventArray::push(const InterfaceEventArray event_array)
{
	for (const InterfaceEvent event : event_array) {
		push(event);
	}
}

void cse::InterfaceEventArray::clear()
{
	data.fill(boost::none);
	occupied = 0;
}

std::atomic_size_t cse::InterfaceEventArray::max_used{ static_cast<size_t>(0) };

void cse::InterfaceEventArray::update_usage_info()
{
	while (occupied > max_used.load()) {
		size_t current = max_used.load();
		if (occupied > current) {
			max_used.compare_exchange_strong(current, occupied);
		}
	}
}
