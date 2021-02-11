#include "waffen_ui.hpp"

#include "imgui_addons.hpp"

Waffen_UI::Waffen_UI(const Sub* sub) {
    (void) sub;
}

void Waffen_UI::update_and_show(const Sub* sub) {
    ImGui::Begin("Weapons View");

    static bool temp = false;
    ImGui::Checkbox("Open Torpedobay Door", &temp);

    static float target_bearing = 0.f;
    ImGui::Nada::KnobDegree("Target Bearing", &target_bearing);

    static int target_depth = -50;
    ImGui::InputInt("Target Depth", &target_depth, 1, 10);
    ImGui::Nada::Tooltip("Hold CTRL for fast increase/decrease");

    static int distance_to_activate = 100;
    ImGui::InputInt("Distance to Activate", &distance_to_activate, 100, 1000);
    ImGui::Nada::Tooltip("Hold CTRL for fast increase/decrease");
    if (distance_to_activate < 100) distance_to_activate = 100;

    // distance_to_deactivate

    ImGui::Button("Launch");

    ImGui::End();
}
