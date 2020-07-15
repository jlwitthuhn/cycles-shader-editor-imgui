#pragma once

/**
 * @file
 * @brief Defines classes used to keep track of events.
 */

#include <array>
#include <atomic>
#include <cstddef>
#include <string>

#include <boost/optional.hpp>

#include "shader_core/config.h"
#include "shader_core/rect.h"
#include "shader_core/util_enum.h"
#include "shader_core/vector.h"
#include "shader_graph/curves.h"
#include "shader_graph/node_id.h"
#include "shader_graph/node_type.h"
#include "shader_graph/slot_id.h"

#include "enum.h"

namespace cse {

	struct InputCharacterDetails {
		InputCharacterDetails(unsigned int codepoint) : codepoint{ codepoint } {}

		unsigned int codepoint;
	};

	struct InputKeyDetails {
		InputKeyDetails(const int key, const int scancode, const int action, const int mods) :
			key{ key }, scancode{ scancode }, action{ action }, mods{ mods } {}

		int key;
		int scancode;
		int action;
		int mods;
	};

	struct InputMouseButtonDetails {
		InputMouseButtonDetails(const int button, const int action, const int mods, const csc::Float2 pos) :
			button{ button }, action{ action }, mods{ mods }, pos{ pos } {}

		int button;
		int action;
		int mods;

		csc::Float2 pos;
	};

	struct InputScrollDetails {
		InputScrollDetails(const double xoffset, const double yoffset) : xoffset{ xoffset }, yoffset{ yoffset } {}
		
		double xoffset;
		double yoffset;
	};

	/**
	 * @brief A class representing some raw user input. Instances of this class require additional processing to be turned into some number of InterfaceEvents.
	 */
	class InputEvent {
	public:
		static size_t sizeof_details();

		InputEvent(const InputCharacterDetails& input_character_details);
		InputEvent(const InputKeyDetails& input_key_details);
		InputEvent(const InputMouseButtonDetails& input_mouse_button_details);
		InputEvent(const InputScrollDetails& input_scroll_details);
		
		inline InputEventType type() const { return _type; }

		boost::optional<InputCharacterDetails> details_character() const;
		boost::optional<InputKeyDetails> details_key() const;
		boost::optional<InputMouseButtonDetails> details_mouse_button() const;
		boost::optional<InputScrollDetails> details_scroll() const;

	private:
		union InputEventDetails {
			InputEventDetails(const InputCharacterDetails& details) : character{ details } {}
			InputEventDetails(const InputKeyDetails& details) : key{ details } {}
			InputEventDetails(const InputMouseButtonDetails& details) : mouse_button{ details } {}
			InputEventDetails(const InputScrollDetails& details) : scroll{ details } {}

			InputCharacterDetails character;
			InputKeyDetails key;
			InputMouseButtonDetails mouse_button;
			InputScrollDetails scroll;
		};

		InputEventType _type;
		InputEventDetails details;
	};
	
	/**
	 * @brief Template class that can be used to store a single object as details.
	 */
	template <typename T, InterfaceEventType... types>
	struct SimpleDetails {
		SimpleDetails(T value) : value{ value } {}

		bool matches_event(InterfaceEventType type) const { return matcher.matches(type); }

		T value;

	private:
		csc::EnumMatcher<InterfaceEventType, types...> matcher;
	};

	typedef SimpleDetails<CurveEditorMode,   InterfaceEventType::CURVE_EDIT_SET_MODE>     CurveEditorModeDetails;
	typedef SimpleDetails<CurveEditorTab,    InterfaceEventType::CURVE_EDIT_SET_TAB>      CurveEditorTabDetails;
	typedef SimpleDetails<SelectMode,        InterfaceEventType::BOX_SELECT_END>          SelectModeDetails;
	typedef SimpleDetails<SubwindowId,       InterfaceEventType::SUBWINDOW_IS_HOVERED>    SubwindowIdDetails;
	typedef SimpleDetails<csg::NodeCategory, InterfaceEventType::SELECT_NODE_CATEGORY>    NodeCategoryDetails;
	typedef SimpleDetails<csg::NodeType,     InterfaceEventType::SELECT_NODE_TYPE>        NodeTypeDetails;
	typedef SimpleDetails<csg::SlotId,
		InterfaceEventType::CONNECTION_BEGIN,
		InterfaceEventType::CONNECTION_ALTER,
		InterfaceEventType::SELECT_SLOT,
		InterfaceEventType::MODAL_CURVE_EDITOR_SHOW
		> SlotIdDetails;
	typedef SimpleDetails<csc::Float3,
		InterfaceEventType::PARAM_EDIT_COLOR_INIT,
		InterfaceEventType::PARAM_EDIT_COLOR_CHANGE
	> Float3Details;
	typedef SimpleDetails<csc::FloatRect,
		InterfaceEventType::CURVE_EDIT_SET_BOUNDS
	> FloatRectDetails;
	typedef SimpleDetails<csc::Int2, InterfaceEventType::PAN_VIEW> Int2Details;

	struct CreateNodeDetails {
		CreateNodeDetails(csg::NodeType type, csc::Float2 screen_pos) : type{ type }, screen_pos{ screen_pos } {}

		csg::NodeType type;
		csc::Float2 screen_pos;
	};

	struct SelectNodeDetails {
		SelectNodeDetails(SelectMode mode, csg::NodeId id) : mode{ mode }, id{ id } {}

		SelectMode mode;
		csg::NodeId id;
	};

	struct SetSlotBoolDetails {
		SetSlotBoolDetails(csg::SlotId slot_id, bool new_value) : slot_id{ slot_id }, new_value{ new_value } {}

		csg::SlotId slot_id;
		bool new_value;
	};

	struct SetSlotColorDetails {
		SetSlotColorDetails(csg::SlotId slot_id, csc::Float3 new_value) : slot_id{ slot_id }, new_value{ new_value } {}

		csg::SlotId slot_id;
		csc::Float3 new_value;
	};

	struct SetSlotEnumDetails {
		SetSlotEnumDetails(csg::SlotId slot_id, size_t new_value) : slot_id{ slot_id }, new_value{ new_value } {}

		csg::SlotId slot_id;
		size_t new_value;
	};

	struct SetSlotFloatDetails {
		SetSlotFloatDetails(csg::SlotId slot_id, float new_value) : slot_id{ slot_id }, new_value{ new_value } {}

		csg::SlotId slot_id;
		float new_value;
	};

	struct SetSlotVectorDetails {
		SetSlotVectorDetails(csg::SlotId slot_id, csc::Float3 new_value) : slot_id{ slot_id }, new_value{ new_value } {}

		csg::SlotId slot_id;
		csc::Float3 new_value;
	};

	struct CurveEditorViewClickDetails {
		CurveEditorViewClickDetails(csc::Float2 pos, CurveEditorMode mode) : pos{ pos }, mode{ mode } {}

		csc::Float2 pos;
		CurveEditorMode mode;
	};

	struct CurveEditorPointMoveDetails {
		CurveEditorPointMoveDetails(size_t index, csc::Float2 new_pos) : index{ index }, new_pos{ new_pos } {}

		size_t index;
		csc::Float2 new_pos;
	};

	struct CurveEditorSetInterpDetails {
		CurveEditorSetInterpDetails(size_t index, csg::CurveInterp interp) : index{ index }, interp{ interp } {}

		size_t index;
		csg::CurveInterp interp;
	};

	/**
	 * @brief A class representing some action that has been taken in the interface. An instance of this class reprensents a clear logical action within the program.
	 */
	class InterfaceEvent {
	public:
		static size_t sizeof_details();

		InterfaceEvent(
			InterfaceEventType type,
			boost::optional<SubwindowId> target = boost::none,
			boost::optional<std::string> message = boost::none
		);
		InterfaceEvent(InterfaceEventType type, const CurveEditorModeDetails& curve_mode_details);
		InterfaceEvent(InterfaceEventType type, const CurveEditorTabDetails& curve_tab_details);
		InterfaceEvent(InterfaceEventType type, const SelectModeDetails& select_mode_details, boost::optional<SubwindowId> target);
		InterfaceEvent(InterfaceEventType type, const SubwindowIdDetails& subwindow_is_hovered_details, boost::optional<SubwindowId> target);
		InterfaceEvent(InterfaceEventType type, const NodeCategoryDetails& node_category_details);
		InterfaceEvent(InterfaceEventType type, const NodeTypeDetails& node_type_details);
		InterfaceEvent(InterfaceEventType type, const SlotIdDetails& slot_id_details, boost::optional<SubwindowId> target);
		InterfaceEvent(InterfaceEventType type, const Float3Details& float3_details, boost::optional<SubwindowId> target);
		InterfaceEvent(InterfaceEventType type, const FloatRectDetails& float_rect_details, boost::optional<SubwindowId> target);
		InterfaceEvent(InterfaceEventType type, const Int2Details& int2_details);
		InterfaceEvent(const CreateNodeDetails& create_node_details);
		InterfaceEvent(const SelectNodeDetails& select_node_details);
		InterfaceEvent(const SetSlotBoolDetails& set_slot_bool_details);
		InterfaceEvent(const SetSlotColorDetails& set_slot_color_details);
		InterfaceEvent(const SetSlotEnumDetails& set_slot_enum_details);
		InterfaceEvent(const SetSlotFloatDetails& set_slot_float_details);
		InterfaceEvent(const SetSlotVectorDetails& set_slot_float_details);
		InterfaceEvent(const CurveEditorViewClickDetails& curve_edit_view_details);
		InterfaceEvent(const CurveEditorPointMoveDetails& curve_edit_point_move_details);
		InterfaceEvent(const CurveEditorSetInterpDetails& curve_edit_set_interp_details);

		inline InterfaceEventType type() const { return _type; }
		inline boost::optional<SubwindowId> target_subwindow() const { return _target_subwindow; }
		inline boost::optional<std::string> message() const { return _message; }

		boost::optional<CurveEditorModeDetails> details_curve_editor_set_mode() const;
		boost::optional<CurveEditorTabDetails> details_curve_editor_set_tab() const;
		boost::optional<SelectModeDetails> details_box_select_end() const;
		boost::optional<SubwindowIdDetails> details_subwindow_is_hovered() const;
		boost::optional<NodeCategoryDetails> details_select_node_category() const;
		boost::optional<NodeTypeDetails> details_select_node_type() const;
		boost::optional<SlotIdDetails> details_connection_begin() const;
		boost::optional<SlotIdDetails> details_connection_alter() const;
		boost::optional<SlotIdDetails> details_select_slot() const;
		boost::optional<SlotIdDetails> details_modal_show_curve_editor() const;
		boost::optional<Float3Details> details_param_edit_color_init() const;
		boost::optional<Float3Details> details_param_edit_color_change() const;
		boost::optional<FloatRectDetails> details_curve_edit_set_bounds() const;
		boost::optional<Int2Details> details_pan_view() const;
		boost::optional<CreateNodeDetails> details_create_node() const;
		boost::optional<SelectNodeDetails> details_select_node() const;
		boost::optional<SetSlotBoolDetails> details_set_slot_bool() const;
		boost::optional<SetSlotColorDetails> details_set_slot_color() const;
		boost::optional<SetSlotEnumDetails> details_set_slot_enum() const;
		boost::optional<SetSlotFloatDetails> details_set_slot_float() const;
		boost::optional<SetSlotVectorDetails> details_set_slot_vector() const;
		boost::optional<CurveEditorViewClickDetails> details_curve_edit_view_click() const;
		boost::optional<CurveEditorPointMoveDetails> details_curve_edit_point_move() const;
		boost::optional<CurveEditorSetInterpDetails> details_curve_edit_set_interp() const;

	private:
		union InterfaceEventDetails {
			InterfaceEventDetails(const CurveEditorModeDetails& details) : curve_editor_mode{ details } {}
			InterfaceEventDetails(const CurveEditorTabDetails& details) : curve_editor_tab{ details } {}
			InterfaceEventDetails(const SelectModeDetails& details) : select_mode{ details } {}
			InterfaceEventDetails(const SubwindowIdDetails& details) : subwindow_id{ details } {}
			InterfaceEventDetails(const NodeCategoryDetails& details) : node_category{ details } {}
			InterfaceEventDetails(const NodeTypeDetails& details) : node_type{ details } {}
			InterfaceEventDetails(const SlotIdDetails& details) : slot_id{ details } {}

			InterfaceEventDetails(const Float3Details& details) : float3{ details } {}
			InterfaceEventDetails(const FloatRectDetails& details) : float_rect{ details } {}
			InterfaceEventDetails(const Int2Details& details) : int2{ details } {}

			InterfaceEventDetails(const CreateNodeDetails& details) : create_node{ details } {}
			InterfaceEventDetails(const SelectNodeDetails& details) : select_node{ details } {}
			InterfaceEventDetails(const SetSlotBoolDetails& details) : set_slot_bool{ details } {}
			InterfaceEventDetails(const SetSlotColorDetails& details) : set_slot_color{ details } {}
			InterfaceEventDetails(const SetSlotEnumDetails& details) : set_slot_enum{ details } {}
			InterfaceEventDetails(const SetSlotFloatDetails& details) : set_slot_float{ details } {}
			InterfaceEventDetails(const SetSlotVectorDetails& details) : set_slot_vector{ details } {}
			InterfaceEventDetails(const CurveEditorViewClickDetails& details) : curve_edit_view_click{ details } {}
			InterfaceEventDetails(const CurveEditorPointMoveDetails& details) : curve_edit_point_move{ details } {}
			InterfaceEventDetails(const CurveEditorSetInterpDetails& details) : curve_edit_set_interp{ details } {}

			CurveEditorModeDetails curve_editor_mode;
			CurveEditorTabDetails curve_editor_tab;
			SelectModeDetails select_mode;
			SubwindowIdDetails subwindow_id;
			NodeCategoryDetails node_category;
			NodeTypeDetails node_type;
			SlotIdDetails slot_id;

			Float3Details float3;
			FloatRectDetails float_rect;
			Int2Details int2;

			CreateNodeDetails create_node;
			SelectNodeDetails select_node;
			SetSlotBoolDetails set_slot_bool;
			SetSlotColorDetails set_slot_color;
			SetSlotEnumDetails set_slot_enum;
			SetSlotFloatDetails set_slot_float;
			SetSlotVectorDetails set_slot_vector;
			CurveEditorViewClickDetails curve_edit_view_click;
			CurveEditorPointMoveDetails curve_edit_point_move;
			CurveEditorSetInterpDetails curve_edit_set_interp;
		};

		InterfaceEventType _type;
		boost::optional<SubwindowId> _target_subwindow;
		boost::optional<std::string> _message;
		boost::optional<InterfaceEventDetails> details;
	};

	/**
	 * @brief Iterator used by InterfaceEventArray. 
	 */
	class InterfaceEventArrayIterator {
	public:
		InterfaceEventArrayIterator(const boost::optional<InterfaceEvent>* ptr) : ptr{ ptr } {}

		inline InterfaceEvent operator*() const { return **ptr; }
		inline void operator++() { ptr++; }
		inline bool operator!=(const InterfaceEventArrayIterator& other) const { return ptr != other.ptr; }

	private:
		const boost::optional<InterfaceEvent>* ptr;
	};

	/**
	 * @brief Utility class to allow easy creation of a fixed-size stack-allocated collection of InterfaceEvents
	 */
	class InterfaceEventArray {
	public:
		static std::atomic_size_t max_used;

		void push(InterfaceEvent event);
		void push(InterfaceEventArray event_array);

		void clear();

		InterfaceEventArrayIterator begin() const { return InterfaceEventArrayIterator{ data.data() }; }
		InterfaceEventArrayIterator end() const { return InterfaceEventArrayIterator{ data.data() + occupied }; }

	private:
		void update_usage_info();

		size_t occupied{ 0 };
		std::array<boost::optional<InterfaceEvent>, INTERFACE_EVENT_ARRAY_SIZE> data;
	};
}
