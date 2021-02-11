#include "imgui_addons.hpp"
#include <cmath>
#include <algorithm>
#include <zufall.hpp>

bool ImGui::Nada::KnobDegree(const char* label, float* p_value, float v_min, float v_max, float v_step, float radius, float thickness, const char* fmt) {
    /// Modified from Source: https://github.com/Flix01/imgui
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    const float ANGLE_MIN = 1.5f * M_PI;
    const float ANGLE_MAX = 3.5f * M_PI;
    const float radius_outer = radius;
    const ImVec2 pos = ImGui::GetCursorScreenPos();
    const ImVec2 center = ImVec2(pos.x + radius_outer, pos.y + radius_outer);
    const float line_height = ImGui::GetTextLineHeight();

    // Value manipulation
    ImGui::InvisibleButton(label, ImVec2(radius_outer*2, radius_outer*2 + line_height + style.ItemInnerSpacing.y));
    bool value_changed = true;
    const bool is_active = ImGui::IsItemActive();
    const bool is_hovered = ImGui::IsItemHovered();
    if (is_active && io.MouseDelta.x != 0.0f) *p_value += io.MouseDelta.x * v_step;
    else if (is_hovered && (io.MouseDoubleClicked[0] || io.MouseClicked[2])) *p_value = v_min;  // reset value
    else if (is_hovered && (std::abs(io.MouseWheel) > 0)) *p_value += io.MouseWheel * v_step;
    else value_changed = false;

    // Clamping
    if (*p_value > v_max) *p_value = std::fmod(*p_value, v_max);
    else while (*p_value < v_min) *p_value = v_max - std::abs(*p_value);

    const float t = (*p_value - v_min) / (v_max - v_min);
    const float angle = ANGLE_MIN + (ANGLE_MAX - ANGLE_MIN) * t;
    const float angle_cos = cosf(angle), angle_sin = sinf(angle);
    const float radius_inner = radius_outer * 0.40f;

    // Draw primitives
    draw_list->AddCircleFilled(center, radius_outer, ImGui::GetColorU32(ImGuiCol_FrameBg), 16);
    draw_list->AddLine(ImVec2(center.x + angle_cos*radius_inner, center.y + angle_sin*radius_inner),
                       ImVec2(center.x + angle_cos*(radius_outer-2), center.y + angle_sin*(radius_outer-2)),
                       ImGui::GetColorU32(ImGuiCol_SliderGrabActive), thickness);
    draw_list->AddCircleFilled(center, radius_inner, ImGui::GetColorU32(is_active ? ImGuiCol_FrameBgActive : is_hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), 16);
    draw_list->AddText(ImVec2(pos.x, pos.y + radius_outer * 2 + style.ItemInnerSpacing.y), ImGui::GetColorU32(ImGuiCol_Text), label);

    // Tooltip
    if (is_active || is_hovered)    {
        ImGui::SetNextWindowPos(ImVec2(pos.x - style.WindowPadding.x, pos.y - line_height - style.ItemInnerSpacing.y - style.WindowPadding.y));
        ImGui::BeginTooltip();
        ImGui::Text(fmt, *p_value);
        ImGui::EndTooltip();
    }
    return value_changed;
}

void ImGui::Nada::Tooltip(const char* text) {
    if (ImGui::IsItemHovered()) ImGui::SetTooltip(text);
}
