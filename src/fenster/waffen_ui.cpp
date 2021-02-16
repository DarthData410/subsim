#include "waffen_ui.hpp"
#include "imgui_addons.hpp"
#include "../sim/game/objects/sub.hpp"
#include "../sim/net/klient.hpp"

Waffen_UI::Waffen_UI() : Standard_UI(nullptr) {
    //
}

Waffen_UI::Waffen_UI(Klient* klient) : Standard_UI(klient) {
    //
}

void Waffen_UI::update_and_show(const Sub* sub) {
    ImGui::Begin("Weapons View");
    if (sub->get_torpedos().empty()) { ImGui::End(); return; } // Gar keine Torpedos? Tschüs.

    // Torpedoauswahl
    static int torpedo_index = 0;
    for (int i = 0; i < sub->get_torpedos().size(); ++i) {
        auto it = sub->get_torpedos().begin();
        std::advance(it, i);
        ImGui::RadioButton(it->first.get_name().c_str(), &torpedo_index, i);
    }

    // Ausgewähltes Torpedo bestimmen
    if (torpedo_index > sub->get_torpedos().size()) torpedo_index = 0;
    auto it = sub->get_torpedos().begin();
    std::advance(it, torpedo_index);
    const Torpedo& torp = it->first;
    const unsigned ammo = it->second;

    ImGui::Text("%u Torpedos Left", ammo);
    ImGui::Text("Range: %.0f", torp.get_range());

    ImGui::NewLine();

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

    if (ImGui::Button("Launch") && temp && ammo > 0) {
        // Launch Torpedo
        const Torpedo t(torp, sub, static_cast<float>(distance_to_activate), target_bearing, static_cast<float>(target_depth));
        const Kommando cmd(Kommando::TORP_LAUNCH, sub->get_id(), t);
        klient->kommando(cmd);
    }

    ImGui::End();
}
