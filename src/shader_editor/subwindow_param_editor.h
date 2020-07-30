#pragma once

#include <memory>

#include <boost/optional.hpp>

#include "event.h"

namespace csc {
	class Float3;
}

namespace csg {
	class BoolSlotValue;
	class ColorRampSlotValue;
	class ColorSlotValue;
	class EnumSlotValue;
	class FloatSlotValue;
	class VectorSlotValue;
	class Graph;
	class SlotId;
}

namespace cse {
	class ParamEditorSubwindow {
	public:
		ParamEditorSubwindow(std::shared_ptr<csg::Graph> the_graph) : the_graph{ the_graph } {}

		InterfaceEventArray run() const;
		InterfaceEventArray run_bool(csg::SlotId slot_id, csg::BoolSlotValue slot_value) const;
		InterfaceEventArray run_color(csg::SlotId slot_id, csg::ColorSlotValue slot_value) const;
		InterfaceEventArray run_enum(csg::SlotId slot_id, csg::EnumSlotValue slot_value) const;
		InterfaceEventArray run_float(csg::SlotId slot_id, csg::FloatSlotValue slot_value) const;
		InterfaceEventArray run_vector(csg::SlotId slot_id, csg::VectorSlotValue slot_value) const;
		InterfaceEventArray run_color_ramp(csg::SlotId slot_id, csg::ColorRampSlotValue slot_value) const;

		void do_event(const InterfaceEvent& event);

	private:
		void clear_state();

		std::shared_ptr<csg::Graph> the_graph;
		
		boost::optional<csg::SlotId> selected_slot;

		// State for the color editor
		boost::optional<csc::Float3> edit_color_color;
	};
}
