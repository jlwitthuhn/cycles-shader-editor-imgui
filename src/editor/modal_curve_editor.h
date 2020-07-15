#pragma once

#include <array>
#include <vector>

#include <boost/optional.hpp>

#include "shader_core/rect.h"
#include "shader_graph/curves.h"
#include "shader_graph/slot.h"
#include "shader_graph/slot_id.h"

#include "enum.h"
#include "event.h"

namespace cse {

	constexpr size_t CURVE_CACHE_ARRAY_SIZE{ 256 };

	class ModalCurveEditor {
	public:
		ModalCurveEditor() { clear(); }

		void clear();
		void set_vector(const csg::VectorCurveSlotValue& value);

		void reset_curve();

		InterfaceEventArray run() const;
		void do_event(const InterfaceEvent& event);

		boost::optional<csg::VectorCurveSlotValue> take_vector();

	private:
		void draw_view(csc::FloatRect area) const;

		void select_tab(CurveEditorTab tab);
		void rebuild_curve_cache();

		void view_create_point(float x);
		void view_delete_point(csc::Float2 pos);
		void view_select_point(csc::Float2 pos);
		void view_select_move_point(csc::Float2 pos);

		boost::optional<size_t> view_get_point(csc::Float2 pos, float radius) const;

		csg::Curve get_current_curve() const;

		bool commit_curve(const csg::Curve& curve);

		CurveEditorMode mode{ CurveEditorMode::SELECT };

		boost::optional<csg::VectorCurveSlotValue> vector_curve;
		std::array<float, CURVE_CACHE_ARRAY_SIZE> cached_curve;

		CurveEditorTab selected_tab{ CurveEditorTab::DEFAULT };
		boost::optional<size_t> selected_point;
	};
}
