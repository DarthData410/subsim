#include "waffen_ui.hpp"
#include "imgui_addons.hpp"
#include "../sim/game/objekte/sub.hpp"
#include "../sim/net/klient.hpp"

Waffen_UI::Waffen_UI() : Standard_UI(nullptr) {
    //
}

Waffen_UI::Waffen_UI(Klient* klient) : Standard_UI(klient) {
    //
}

void Waffen_UI::update_and_show(const Sub* sub) {
    show_weapons(sub);
    show_intelligence();
}

void Waffen_UI::show_weapons(const Sub* sub) const {
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
    ImGui::InputInt("Distance to Activate", &distance_to_activate, 50, 100);
    ImGui::Nada::Tooltip("Hold CTRL for fast increase/decrease");
    distance_to_activate = std::clamp(distance_to_activate, 150, static_cast<int>(torp.get_range()));

    static int distance_to_explode = 50;
    ImGui::InputInt("Proximity Fuse in m", &distance_to_explode, 1, 2);
    distance_to_explode = std::clamp(distance_to_explode, 1, 100);

    if (ImGui::Button("Launch") && temp && ammo > 0) {
        // Launch Torpedo
        const Torpedo t(torp, sub, static_cast<float>(distance_to_activate), target_bearing,
                        static_cast<float>(target_depth), static_cast<float>(distance_to_explode));
        const Kommando cmd(Kommando::TORP_LAUNCH, sub->get_id(), t);
        klient->kommando(cmd);
    }

    ImGui::End();
}

void Waffen_UI::show_intelligence() const {
    ImGui::Begin("Intelligence");

    /// Übersicht: Submarine Designs
    if (ImGui::BeginTable("Submarine Designs", 7)) {
        ImGui::TableSetupColumn("Team");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Max Speed (in m/s)");
        ImGui::TableSetupColumn("Noise Level");
        ImGui::TableSetupColumn("Active Sonar Range");
        ImGui::TableSetupColumn("Passive Sonar Sensitivity");
        ImGui::TableSetupColumn("Sonar Resolution (in °)");
        ImGui::TableHeadersRow();
        for (const auto&[team_id, team]: klient->get_teams()) {
            for (const auto& subdesign : team.get_sub_designs()) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%d", (int)team_id);
                ImGui::TableSetColumnIndex(1); ImGui::TextUnformatted(subdesign.get_name().c_str());
                ImGui::TableSetColumnIndex(2); ImGui::Text("%.1f", subdesign.get_speed_max());
                ImGui::TableSetColumnIndex(3); ImGui::Text("%.1f", subdesign.get_noise());
                ImGui::TableSetColumnIndex(4); ImGui::Text("%.0f", subdesign.get_max_reichweite_as());
                ImGui::TableSetColumnIndex(5); ImGui::Text("%.0f", subdesign.get_beste_empfindlichkeit_ps());
                ImGui::TableSetColumnIndex(6); ImGui::Text("%.1f", subdesign.get_best_aufloesung_sonar());
            }
        }
        ImGui::EndTable();
    }

    /// Torpedos
    if (ImGui::BeginTable("Torpedoes", 3)) {
        ImGui::TableSetupColumn("One");
        ImGui::TableSetupColumn("Two");
        ImGui::TableSetupColumn("Three");
        ImGui::TableHeadersRow();
        for (const auto&[team_id, team]: klient->get_teams()) {
            ImGui::TableNextRow();
            ImGui::NextColumn();
        }
        ImGui::EndTable();
    }

    ImGui::End();
}
