#include <zufall.hpp>
#include "sonar_ui.hpp"
#include "../sim/game/objects/sub.hpp"
#include "imgui_addons.hpp"

Sonar_UI::Sonar_UI(const Sub* sub) {
    reset_sonar_data(sub);
}

void Sonar_UI::reset_sonar_data(const Sub* sub) {
    // Daten zurücksetzen
    sonar_data.clear();
    intervalle.clear();
    timers.clear();
    sonar_data.reserve(sub->get_sonars().size());
    intervalle.reserve(sub->get_sonars().size());
    timers.reserve(sub->get_sonars().size());

    // Für jeden Sonar...
    for (const auto& sonar : sub->get_sonars()) {
        // Sonardaten anlegen
        sonar_data.emplace_back();
        for (auto& vektor : sonar_data.back()) vektor = std::vector<float>(sonar.get_resolution(), 0.f);
        // Updateintervalle zurücksetzen
        intervalle.push_back(500);
        timers.emplace_back();
    }
}

void Sonar_UI::update_and_show(const Sub* sub) {
    for (unsigned i = 0; i < sonar_data.size(); ++i) {
        std::array<std::vector<float>, HISTORY_SIZE>& histogram = sonar_data[i];
        const Sonar_Passiv& sonar = sub->get_sonars()[i];

        // Neue Zeile lesen?
        if (Ogre::Timer& timer = timers[i]; timer.getMilliseconds() > intervalle[i]) {

            // Zeile mit Rauschen erzeugen
            std::vector<float> newline(sonar.get_resolution());
            std::generate(newline.begin(), newline.end(), [&]() { return Zufall::f(0.f, sonar.get_noise()); } );

            // Detektionen eintragen
            for (const Detection& d : sonar.get_detections()) {
                const unsigned histogram_position = std::clamp<unsigned>(std::round(
                        d.bearing * (sonar.get_resolution() / 360.f)), 0u, newline.size() - 1u);
                newline[histogram_position] += d.gain;
            }
            // Alle Zeilen 1 weiter rücken
            histogram[histogram.size()-1] = newline;
            std::rotate(histogram.rbegin(), histogram.rbegin() + 1, histogram.rend());
            timer.reset();
        }
        show(histogram); // Anzeigemethode aufrufen
    }
}

void Sonar_UI::show(const std::array<std::vector<float>, HISTORY_SIZE>& histogram) const {
    const float w = 400.f;
    const float h = 600.f;
    const char* label = "passive_sonar";
    ImGui::BeginChild(label, {w + 40.f,h + 80.f}, true,
                      ImGuiWindowFlags_NoScrollbar);
    ImGuiIO& io = ImGui::GetIO();
    const ImGuiStyle& style = ImGui::GetStyle();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const ImColor IM_WHITE = ImColor(0xFF, 0xFF, 0xFF);
    const ImColor IM_GREEN = ImColor(0x00, 0xFF, 0x00);

    const unsigned ZEILEN  = histogram.size();
    const unsigned SPALTEN = histogram[0].size();

    const ImVec2 start_pos = ImGui::GetCursorScreenPos();
    const float rect_h = h / static_cast<float>(ZEILEN);
    const float rect_w = w / static_cast<float>(SPALTEN);
    const float line_w = 8.f;

    // Vertikale Skale Beschriftung
    static std::unordered_map<unsigned, std::string> u_to_str;
    if (u_to_str.empty()) for (unsigned row = 0; row <= ZEILEN; ++row) u_to_str[row] = std::to_string(row);

    // Sonar-Rects
    for (unsigned row = 0; row < ZEILEN; ++row) {
        const float r = static_cast<float>(row);
        for (unsigned col = 0; col < SPALTEN; ++col) {
            const float c = static_cast<float>(col);
            draw_list->AddRectFilled(
                    {start_pos.x + c * rect_w, start_pos.y + r * rect_h},
                    {start_pos.x + c * rect_w + rect_w, start_pos.y + r * rect_h + rect_h},
                    ImColor(0.f, std::min(1.f, histogram[row][col]), 0.f, 1.0f), 0.f
            );
        }
        // Vertikale Skala
        if (row % 4 > 0) continue;
        const float line_y = start_pos.y + r * rect_h + 0.5f * rect_h;
        draw_list->AddLine({start_pos.x + w,          line_y},
                           {start_pos.x + w + line_w, line_y},
                           IM_GREEN);
        draw_list->AddText({start_pos.x + w + line_w, line_y - 8.f},
                           IM_GREEN,
                           u_to_str[ZEILEN - row].c_str());
    }

    // Tooltip °
    ImGui::InvisibleButton(label, {w, h + 8});
    if (ImGui::IsItemHovered()) {
        const ImVec2& mouse_pos = io.MousePos;
        const ImVec2& win_pos = ImGui::GetWindowPos();
        draw_list->AddLine({mouse_pos.x, start_pos.y}, {mouse_pos.x, start_pos.y + h}, IM_GREEN);
        draw_list->AddLine({start_pos.x, mouse_pos.y}, {start_pos.x + w, mouse_pos.y}, IM_GREEN);
        ImGui::SetNextWindowPos({mouse_pos.x - 20.f, start_pos.y + h});
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
                           IM_GREEN);
        ImGui::SameLine();
        ImGui::SetCursorPosX(static_cast<float>(i) * (w / tick_marks));
        ImGui::Text("%d°", -180 + i * (360 / tick_marks));
    }
    ImGui::EndChild();
}
