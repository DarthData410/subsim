#include "ui.hpp"

#include <cmath>
#include <algorithm>
#include <zufall.hpp>
#include <imgui.h>

bool ui::KnobDegree(const char* label, float* p_value, float v_min, float v_max, float v_step, float radius, float thickness, const char* fmt, const std::optional<float>& value2) {
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
    if (value2) { // Optionaler 2ter Wert
        const float t2 = (value2.value() - v_min) / (v_max - v_min);
        const float angle2 = ANGLE_MIN + (ANGLE_MAX - ANGLE_MIN) * t2;
        const float angle_cos2 = cosf(angle2), angle_sin2 = sinf(angle2);
        draw_list->AddLine(ImVec2(center.x + angle_cos2*radius_inner, center.y + angle_sin2*radius_inner),
                           ImVec2(center.x + angle_cos2*(radius_outer-2), center.y + angle_sin2*(radius_outer-2)),
                           ImColor(0x80'00'00'FF), thickness);
    }

    // Tooltip
    if (is_active || is_hovered)    {
        ImGui::SetNextWindowPos(ImVec2(pos.x - style.WindowPadding.x, pos.y - line_height - style.ItemInnerSpacing.y - style.WindowPadding.y));
        ImGui::BeginTooltip();
        ImGui::Text(fmt, *p_value);
        ImGui::EndTooltip();
    }
    return value_changed;
}

void ui::Tooltip(const char* text) {
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s", text);
}

bool ui::Button(const char* label, const ImVec2& size) {
    return ImGui::Button(label, size);
}

void ui::Text(const char* fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    ImGui::TextV(fmt, argp);
    va_end(argp);
}

bool ui::SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags) {
    return ImGui::SliderFloat(label, v, v_min, v_max, format, flags);
}

bool ui::SliderInt(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags) {
    return ImGui::SliderInt(label, v, v_min, v_max, format, flags);
}

bool ui::RadioButton(const char* label, bool active) {
    return ImGui::RadioButton(label, active);
}

bool ui::RadioButton(const char* label, int* v, int v_button) {
    return ImGui::RadioButton(label, v, v_button);
}

void ui::Separator() {
    ImGui::Separator();
}

bool ui::InputInt(const char* label, int* v, int step, int step_fast, ImGuiInputTextFlags flags) {
    return ImGui::InputInt(label, v, step, step_fast, flags);
}

bool ui::Checkbox(const char* label, bool* v) {
    return ImGui::Checkbox(label, v);
}

void ui::TextUnformatted(const char* text) {
    ImGui::TextUnformatted(text);
}