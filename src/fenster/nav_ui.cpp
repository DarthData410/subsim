#include "nav_ui.hpp"
#include "imgui_addons.hpp"
#include "../sim/net/klient.hpp"
#include "../sim/game/objects/sub.hpp"

Nav_UI::Nav_UI() : Standard_UI(nullptr) {
    //
}

Nav_UI::Nav_UI(Klient* klient) : Standard_UI(klient) {
    //
}

void Nav_UI::update_and_show(const Sub* sub) {
    ImGui::SetNextWindowSize({300,0});
    ImGui::Begin("Nav View");

    ImGui::Text("Sub: %.1f %.1f Depth: %.1f", sub->get_pos().x, sub->get_pos().z, sub->get_pos().y);
    ImGui::Text("Pitch: %.1f", sub->get_pitch());
    ImGui::Text("Bearing: %.1f", sub->get_bearing());

    static float target_x = 0, target_z = 0;
    ImGui::InputFloat("Target_x", &target_x);
    ImGui::InputFloat("Target_z", &target_z);
    if (ImGui::Button("Set##set_target_pos")) {
        klient->kommando({Kommando::AUTO_POS, sub->get_id(),
                          std::tuple<float, float>(target_x, target_z)});
    }

    static float target_bearing = 0;
    ImGui::Nada::KnobDegree("target_bearing", &target_bearing,
                            0.f, 360.f, 1.f, 40.f, 2.f, "%.0f°", std::make_optional(sub->get_bearing()));
    if (ImGui::Button("Set##set_bearing")) {
        klient->kommando({Kommando::AUTO_KURS, sub->get_id(), (float)target_bearing});
    }
    ImGui::Separator();

    static float target_speed = 0;
    ImGui::SliderFloat("target_speed", &target_speed, -1.0f, 1.0f);
    if (ImGui::Button("Set##set_speed")) {
        klient->kommando({Kommando::MOTOR_LINEAR, sub->get_id(), (float)target_speed});
    }
    ImGui::Separator();

    if (ImGui::Button("stop")) {
        klient->kommando({Kommando::STOP, sub->get_id()});
    }
    if (ImGui::Button("rechts")) {
        klient->kommando({Kommando::MOTOR_ROT, sub->get_id(), 100.f});
    }
    if (ImGui::Button("links")) {
        klient->kommando({Kommando::MOTOR_ROT, sub->get_id(), -100.f});
    }
    ImGui::End();

    static std::unordered_map<uint8_t, Team> teams = klient->get_teams();
    static std::vector<Zone> zonen = klient->get_zonen();
    if (static Ogre::Timer timer; timer.getMilliseconds() > 500) {
        teams = klient->get_teams();
        zonen = klient->get_zonen();
    }
    ImGui::Begin("Strategic Overview");
    for (const auto& team : teams) {
        ImGui::Text("Team %u: %u Points", team.first, team.second.get_punkte());
    }
    for (const auto& zone : zonen) {
        ImGui::Text("Zone @ x=%.0f, y=%.0f, owned by Team %u",
                    std::get<0>(zone.get_pos()), std::get<1>(zone.get_pos()), zone.get_team());
    }
    ImGui::End();
}
