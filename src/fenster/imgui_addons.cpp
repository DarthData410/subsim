#include "imgui_addons.hpp"
#include <cmath>
#include <algorithm>
#include <cstring>
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

bool ImGui::Nada::Sonar() {
    const float w = 400.f;
    const float h = 600.f;
    const char* label = "passive_sonar";
    ImGui::BeginChild(label, {w + 40.f,h + 80.f}, true,
                      ImGuiWindowFlags_NoScrollbar);
    ImGuiIO& io = ImGui::GetIO();
    const ImGuiStyle& style = ImGui::GetStyle();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    const unsigned sonar_data_size = 36;
    static std::array<std::vector<float>, 60> history;

    // Random Noise Row generieren
    const float noise = 1.f / 8.f;
    auto random_row = [noise]() {
        std::vector<float> sonar_data(sonar_data_size);
        std::generate(sonar_data.begin(), sonar_data.end(), [noise]() {
            return Zufall::f(0.f, noise); }
        );
        return sonar_data;
    };

    // Test: Random Data
    if (static bool populated = false; !populated) {
        for (auto& v : history) v = random_row();
        populated = true;
    }

    const ImVec2 start_pos = ImGui::GetCursorScreenPos();
    const float rect_h = h / history.size();
    const float rect_w = w / sonar_data_size;
    const float line_w = 8.f;

    // Vertikale Skale Beschriftung
    static std::unordered_map<unsigned, std::string> u_to_str;
    if (u_to_str.empty()) for (unsigned row = 0; row <= history.size(); ++row) u_to_str[row] = std::to_string(row);

    // Sonar-Rects
    for (unsigned row = 0; row < history.size(); ++row) {
        const float r = static_cast<float>(row);
        for (unsigned col = 0; col < history[row].size(); ++col) {
            const float c = static_cast<float>(col);
            draw_list->AddRectFilled(
                    {start_pos.x + c * rect_w, start_pos.y + r * rect_h},
                    {start_pos.x + c * rect_w + rect_w, start_pos.y + r * rect_h + rect_h},
                    ImColor(0.f, std::min(1.f, history[row][col]), 0.f, 1.0f), 0.f
            );
        }
        // Vertikale Skala
        if (row % 4 > 0) continue;
        const float line_y = start_pos.y + r * rect_h + 0.5f * rect_h;
        draw_list->AddLine({start_pos.x + w,          line_y},
                           {start_pos.x + w + line_w, line_y},
                           ImColor(1.f, 1.f, 1.f));
        draw_list->AddText({start_pos.x + w + line_w, line_y - 8.f},
                           ImColor(1.f, 1.f, 1.f),
                           u_to_str[history.size() - row].c_str());
    }

    // Tooltip °
    ImGui::InvisibleButton(label, {w, h + 8});
    if (ImGui::IsItemHovered()) {
        const ImVec2& mouse_pos = io.MousePos;
        const ImVec2& win_pos = ImGui::GetWindowPos();
        ImGui::SetNextWindowPos({mouse_pos.x, mouse_pos.y - 20.f});
        ImGui::BeginTooltip();
        const float deg = std::clamp(-188.f + 360.f * (mouse_pos.x - win_pos.x) / w,
                                     -180.f, 180.f);
        ImGui::Text("%.0f°", deg);
        ImGui::EndTooltip();
    }

    // Skala °
    ImGui::Dummy({0,0}); // für SameLine
    const unsigned tick_marks = 6;
    for (unsigned i = 0; i <= tick_marks; ++i) {
        const float x = start_pos.x + static_cast<float>(i) * (w / tick_marks);
        const float y = start_pos.y + h;
        draw_list->AddLine({x, y},
                           {x, y + line_w},
                           ImColor(1.f, 1.f, 1.f));
        ImGui::SameLine();
        ImGui::SetCursorPosX(static_cast<float>(i) * (w / tick_marks));
        ImGui::Text("%d°", -180 + i * (360 / tick_marks));
    }
    ImGui::EndChild();
    return false;
}
