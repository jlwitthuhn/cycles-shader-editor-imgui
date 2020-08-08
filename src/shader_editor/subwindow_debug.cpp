#include "subwindow_debug.h"

#include <atomic>
#include <cstddef>
#include <set>
#include <sstream>
#include <string>

#include <boost/optional.hpp>
#include <imgui.h>

#include "shader_core/util_enum.h"
#include "shader_graph/node.h"
#include "shader_graph/node_enums.h"
#include "shader_graph/node_type.h"
#include "shader_graph/slot.h"

#include "enum.h"
#include "event.h"

cse::DebugSubwindow::DebugSubwindow() : message("Pres butan to run validation.")
{

}

cse::InterfaceEventArray cse::DebugSubwindow::run() const
{
	InterfaceEventArray events;

	bool window_open{ true };
	if (ImGui::Begin("Debug", &window_open, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::BeginTabBar("debug_tab_bar");
		if (ImGui::BeginTabItem("Static Data")) {
			ImGui::Text("Use this to validate static data.");
			if (ImGui::Button("Pres Butan")) {
				const std::string validation_message{ run_validation() };
				events.push(InterfaceEvent{ InterfaceEventType::VALIDATE_SET_MESSAGE, SubwindowId::DEBUG, validation_message });
			}
			ImGui::Separator();
			ImGui::Text("%s", message.c_str());
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Types")) {
			ImGui::Text("Size of internal types, in bytes.");
			ImGui::Separator();
			ImGui::Text("cse::InputEvent: %ld", sizeof(InputEvent));
			ImGui::Text("   ::InputEventDetails: %ld", InputEvent::sizeof_details());
			ImGui::Text("cse::InterfaceEvent: %ld", sizeof(InterfaceEvent));
			ImGui::Text("   ::InterfaceEventDetails: %ld", InterfaceEvent::sizeof_details());
			ImGui::Text("   ::InterfaceEventArray: %ld", sizeof(InterfaceEventArray));
			ImGui::Text("csg::Node: %ld", sizeof(csg::Node));
			ImGui::Text("csg::Slot: %ld", sizeof(csg::Slot));
			ImGui::Text("   ::SlotValue: %ld", sizeof(csg::SlotValue));
			ImGui::Text("   ::SlotValueUnion: %ld", csg::SlotValue::sizeof_union());
			ImGui::Text("std::string: %ld", sizeof(std::string));
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Runtime")) {
			ImGui::Text("cse::InterfaceEventArray max size: %ld", cse::InterfaceEventArray::max_used.load());
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
	if (window_open == false) {
		events.push(InterfaceEventType::WINDOW_CLOSE_DEBUG);
	}

	return events;
}

void cse::DebugSubwindow::do_event(const InterfaceEvent& event)
{
	if (event.type() == InterfaceEventType::VALIDATE_SET_MESSAGE && event.message()) {
		message = event.message().get();
	}
}

std::string cse::DebugSubwindow::run_validation() const
{
	size_t error_count{ 0 };
	std::stringstream out_stream;
	{
		out_stream << "Checking NodeCategoryInfo for each NodeCategory..." << std::endl;
		for (const auto category : csg::NodeCategoryList()) {
			const auto info{ csg::NodeCategoryInfo::from(category) };
			if (info) {
				if (category != info->category()) {
					++error_count;
					out_stream << "Information mismatch with NodeCategoryInfo::from(" << static_cast<int>(category) << ")" << std::endl;
				}
			}
			else {
				++error_count;
				out_stream << "NodeCategoryInfo::from(" << static_cast<int>(category) << ") returns none" << std::endl;
			}
		}
	}
	{
		out_stream << "Checking uniqueness of NodeCategory names..." << std::endl;
		std::set<std::string> names;
		for (const auto category : csg::NodeCategoryList()) {
			const auto info{ csg::NodeCategoryInfo::from(category) };
			if (info) {
				std::string this_name{ info->name() };
				if (names.count(this_name)) {
					++error_count;
					out_stream << "NodeCategory " << static_cast<int>(category) << " has a non-unique name: " << this_name << std::endl;
				}
				else {
					names.insert(this_name);
				}
			}
		}
	}
	{
		out_stream << "Checking NodeTypeInfo for each NodeType..." << std::endl;
		for (const auto type : csg::NodeTypeList	()) {
			const auto info{ csg::NodeTypeInfo::from(type) };
			if (info) {
				if (type != info->type()) {
					++error_count;
					out_stream << "Information mismatch with NodeTypeInfo::from(" << static_cast<int>(type) << ")" << std::endl;
				}
			}
			else {
				++error_count;
				out_stream << "NodeTypeInfo::from(" << static_cast<int>(type) << ") returns none" << std::endl;
			}
		}
	}
	{
		out_stream << "Checking uniqueness of NodeType names..." << std::endl;
		std::set<std::string> names;
		for (const auto type : csg::NodeTypeList()) {
			const auto info{ csg::NodeTypeInfo::from(type) };
			if (info) {
				std::string this_name{ info->disp_name() };
				if (names.count(this_name)) {
					++error_count;
					out_stream << "NodeType " << static_cast<int>(type) << " has a non-unique name: " << this_name << std::endl;
				}
				else {
					names.insert(this_name);
				}
			}
		}
	}
	{
		out_stream << "Checking NodeEnumInfo and NodeEnumOptionInfo for each NodeMetaEnum..." << std::endl;
		const csc::EnumList<csg::NodeMetaEnum, csg::NodeMetaEnum::COUNT> enum_list;
		for (const csg::NodeMetaEnum this_enum : enum_list) {
			boost::optional<csg::NodeEnumInfo> enum_info{ csg::NodeEnumInfo::from(this_enum) };
			std::set<std::string> names;
			if (enum_info) {
				for (size_t i = 0; i < enum_info->count(); i++) {
					boost::optional<csg::NodeEnumOptionInfo> enum_option_info{ csg::NodeEnumOptionInfo::from(this_enum, i) };
					if (enum_option_info) {
						if (names.count(enum_option_info->internal_name())) {
							++error_count;
							out_stream << "NodeEnumOptionInfo::from(" << static_cast<int>(this_enum) << ", " << i << ") has repeated name" << std::endl;
						}
						else if (std::string{ "ERROR" } == enum_option_info->internal_name()) {
							++error_count;
							out_stream << "NodeEnumOptionInfo::from(" << static_cast<int>(this_enum) << ", " << i << ") has ERROR string" << std::endl;
						}
						else {
							names.insert(enum_option_info->internal_name());
						}
					}
					else {
						++error_count;
						out_stream << "NodeEnumOptionInfo::from(" << static_cast<int>(this_enum) << ", " << i << ") returns none" << std::endl;
					}
				}
			}
			else {
				++error_count;
				out_stream << "NodeEnumInfo::from(" << static_cast<int>(this_enum) << ") returns none" << std::endl;
			}
		}
	}
	if (error_count > 0) {
		out_stream << "Validation failed with " << error_count << " error(s)." << std::endl;
	}
	else {
		out_stream << "Validation suceeded." << std::endl;
	}
	return out_stream.str();
}
