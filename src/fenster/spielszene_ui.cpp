#include "spielszene.hpp"
#include "imgui_addons.hpp"
//#include "../gfx/image.hpp"

#include <zufall.hpp>

void Spielszene::render_nav() {
    if (!player_sub) return;
    ImGui::SetNextWindowSize({300,0});
    ImGui::Begin("Nav View");

    ImGui::Text("Sub: %.1f %.1f Depth: %.1f", player_sub->get_pos().x, player_sub->get_pos().z, player_sub->get_pos().y);
    ImGui::Text("Pitch: %.1f", player_sub->get_pitch());
    ImGui::Text("Bearing: %.1f", player_sub->get_bearing());

    static float target_x = 0, target_z = 0;
    ImGui::InputFloat("Target_x", &target_x);
    ImGui::InputFloat("Target_z", &target_z);
    if (ImGui::Button("Set##set_target_pos")) {
        klient->kommando({Kommando::AUTO_POS, player_sub->get_id(),
                          std::tuple<float, float>(target_x, target_z)});
    }

    static float target_bearing = 0;
    ImGui::Nada::KnobDegree("target_bearing", &target_bearing);
    if (ImGui::Button("Set##set_bearing")) {
        klient->kommando({Kommando::AUTO_KURS, player_sub->get_id(), (float)target_bearing});
    }
    ImGui::Separator();

    static float target_speed = 0;
    ImGui::SliderFloat("target_speed", &target_speed, -1.0f, 1.0f);
    if (ImGui::Button("Set##set_speed")) {
        klient->kommando({Kommando::MOTOR_LINEAR, player_sub->get_id(), (float)target_speed});
    }
    ImGui::Separator();

    if (ImGui::Button("stop")) {
        klient->kommando({Kommando::STOP, player_sub->get_id()});
    }
    if (ImGui::Button("rechts")) {
        klient->kommando({Kommando::MOTOR_ROT, player_sub->get_id(), 100.f});
    }
    if (ImGui::Button("links")) {
        klient->kommando({Kommando::MOTOR_ROT, player_sub->get_id(), -100.f});
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

void Spielszene::render_sonar() {
    ImGui::Begin("Sonar View");

    sonar_ui.update_and_show(&player_sub.value());

    ImGui::End();
}

void Spielszene::render_weapons() {

}

void Spielszene::render_3d() {

}

void Spielszene::render_menu() {

}
