#pragma once

/**
 * @file
 * @brief Wrapper functions to make ImGui work with internal data types (mostly vectors).
 */

#include <imgui.h>

#include "shader_core/rect.h"
#include "shader_core/vector.h"

inline ImVec2 as_imvec(const csc::Float2& float2)
{
	return ImVec2{ float2.x, float2.y };
}

namespace ImGui {
	inline void SetNextWindowPos(const csc::Float2 pos, ImGuiCond cond = 0, const csc::Float2 pivot = csc::Float2(0.0f, 0.0f))
	{
		SetNextWindowPos(as_imvec(pos), cond, as_imvec(pivot));
	}

	inline void SetNextWindowSize(const csc::Float2 size, ImGuiCond cond = 0)
	{
		SetNextWindowSize(as_imvec(size), cond);
	}

	namespace DrawList {
		inline void PushClipRect(ImDrawList* draw_list, csc::FloatRect clip_rect, bool intersect_with_current_clip_rect = false)
		{
			draw_list->PushClipRect(as_imvec(clip_rect.begin()), as_imvec(clip_rect.end()), intersect_with_current_clip_rect);
		}

		inline void PopClipRect(ImDrawList* draw_list)
		{
			draw_list->PopClipRect();
		}

		inline void AddLine(ImDrawList* draw_list, csc::Float2 p1, csc::Float2 p2, ImU32 col, float thickness = 1.0f)
		{
			draw_list->AddLine(as_imvec(p1), as_imvec(p2), col, thickness);
		}

		inline void AddRect(ImDrawList* draw_list, csc::FloatRect rect, ImU32 col, float rounding = 0.0f, ImDrawCornerFlags rounding_corners = ImDrawCornerFlags_All, float thickness = 1.0f)
		{
			draw_list->AddRect(as_imvec(rect.begin()), as_imvec(rect.end()), col, rounding, rounding_corners, thickness);
		}

		inline void AddRectFilled(ImDrawList* draw_list, csc::FloatRect rect, ImU32 col, float rounding = 0.0f, ImDrawCornerFlags rounding_corners = ImDrawCornerFlags_All)
		{
			draw_list->AddRectFilled(as_imvec(rect.begin()), as_imvec(rect.end()), col, rounding, rounding_corners);
		}

		inline void AddCircle(ImDrawList* draw_list, csc::Float2 center, float radius, ImU32 col, int num_segments = 8, float thickness = 1.0f)
		{
			draw_list->AddCircle(as_imvec(center), radius, col, num_segments, thickness);
		}

		inline void AddCircleFilled(ImDrawList* draw_list, csc::Float2 center, float radius, ImU32 col, int num_segments = 8)
		{
			draw_list->AddCircleFilled(as_imvec(center), radius, col, num_segments);
		}

		inline void AddText(ImDrawList* const draw_list, const csc::Float2 pos, const ImU32 col, const char* const text_begin, const char* const text_end = NULL)
		{
			draw_list->AddText(as_imvec(pos), col, text_begin, text_end);
		}
	}
}
