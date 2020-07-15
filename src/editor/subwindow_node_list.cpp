#include "subwindow_node_list.h"

#include <imgui.h>

#include "shader_graph/node_type.h"

#include "enum.h"

cse::NodeListSubwindow::NodeListSubwindow()
{

}

cse::InterfaceEventArray cse::NodeListSubwindow::run() const
{
	InterfaceEventArray events;

	if (ImGui::Begin("Nodes", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		constexpr float WINDOW_WIDTH{ 180.0f };
		ImGui::PushItemWidth(WINDOW_WIDTH);
		if (ImGui::BeginCombo("", csg::NodeCategoryInfo::from(selected_category)->name())) {
			for (const auto node_category : csg::NodeCategoryList{}) {
				const auto category_info{ csg::NodeCategoryInfo::from(node_category) };
				if (category_info && category_info->allow_creation()) {
					if (ImGui::Selectable(category_info->name())) {
						events.push(InterfaceEvent{ InterfaceEventType::SELECT_NODE_CATEGORY, NodeCategoryDetails{ category_info->category() } });
					}
				}
			}
			ImGui::EndCombo();
		}
		ImGui::Separator();
		for (const csg::NodeType node_type : csg::NodeTypeList()) {
			const auto node_info{ csg::NodeTypeInfo::from(node_type) };
			if (node_info && node_info->category() == selected_category) {
				if (node_info->type() == _selected_type) {
					ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive));
					ImGui::Button(node_info->disp_name(), ImVec2{ WINDOW_WIDTH, 0.0f });
					ImGui::PopStyleColor();
				}
				else if (ImGui::Button(node_info->disp_name(), ImVec2{ WINDOW_WIDTH, 0.0f })) {
					events.push(InterfaceEvent{ InterfaceEventType::SELECT_NODE_TYPE, NodeTypeDetails{ node_info->type() } });
				}
			}
		}
		ImGui::PopItemWidth();
	}
	ImGui::End();

	return events;
}

void cse::NodeListSubwindow::do_event(const InterfaceEvent& event)
{
	if (event.target_subwindow() && event.target_subwindow() == SubwindowId::NODE_LIST) {
		// Event is targeting this subwindow
		switch (event.type()) {
			case InterfaceEventType::SELECT_NODE_CATEGORY:
			{
				if (event.details_select_node_category()) {
					selected_category = event.details_select_node_category()->value;
					_selected_type = boost::none;
				}
				break;
			}
			case InterfaceEventType::SELECT_NODE_TYPE:
			{
				if (event.details_select_node_type()) {
					_selected_type = event.details_select_node_type()->value;
				}
				break;
			}
			case InterfaceEventType::SELECT_NODE_TYPE_NONE:
			{
				_selected_type = boost::none;
				break;
			}
			default:
				// Do nothing
				break;
		}
	}
}

boost::optional<csg::NodeType> cse::NodeListSubwindow::selected_type() const
{
	return _selected_type;
}
