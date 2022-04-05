#include "nav_ui.hpp"
#include "imgui_addons.hpp"
#include "../sim/net/klient.hpp"
#include "../sim/physik.hpp"

#include <log.hpp>
#include <SFML/System/Clock.hpp>
#include <implot.h>
#include <cmath>
#include <cereal/types/memory.hpp>

Nav_UI::Nav_UI() : Standard_UI(nullptr) {

}

Nav_UI::Nav_UI(Klient* klient) : Standard_UI(klient) {

}

void Nav_UI::sync(bool force) {
    if (static sf::Clock timer; timer.getElapsedTime().asMilliseconds() > SYNC_INTERVALL || force) {
        timer.restart();
        const std::string& objekte_raw = klient->request(Net::ALLE_OBJEKTE);
        zonen = klient->get_zonen();
        teams = klient->get_teams();
        if (!objekte_raw.empty()) {
            try { objekte = Net::deserialize<std::vector<std::unique_ptr<Objekt>>>(objekte_raw); }
            catch (const std::exception& e) {
                Log::err() << "Error: Konnte Antwort auf Net::ALLE_OBJEKTE nicht deserialisieren, Größe: ";
                Log::err() << objekte_raw.size() << " Fehler: " << e.what() << '\n';
            }
        }
    }
}

void Nav_UI::update_and_show(const Sub* sub) {
    sync(objekte.empty());
    show_navigation(sub);
    show_minimap(sub);

    ImGui::Begin("Noise signature");
    show_noise_signature(sub, sub->get_speed());
    ImGui::End();
}

void Nav_UI::show_minimap(const Sub* sub) const {
    ImGui::Begin("Minimap");
    const ImVec2 pos_ui = ImGui::GetCursorScreenPos();
    const auto& pos_sub = sub->get_pos();
    const float size_x = 400.f;
    const float size_y = 400.f;
    const float center_x = pos_ui.x + 0.5f*size_x;
    const float center_y = pos_ui.y + 0.5f*size_y;
    static float scale = 0.1f;

    const auto world2ui = [&] (float x, float y) {
        return std::array<float, 2> {
                static_cast<float>(center_x + scale * x),
                static_cast<float>(center_y - scale * y)
        };
    };

    // Ozean zeichnen
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(pos_ui, {pos_ui.x + size_x, pos_ui.y + size_y},
                             ImColor(0x00, 0x50, 0xB0), 0.0f);

    // Objekte zeichnen
    for (const auto& o : objekte) {
        //if (fow && o.get_team() != sub->get_team()) continue; // fremdes Team TODO
        //if (x < 0 || x > size_x || y < 0 || y > size_y) continue; // außerhalb des Bildes
        const auto pos = world2ui(o->get_pos().x(), o->get_pos().y());
        auto color = ImColor(0xFF, 0xFF, 0xFF);
        if      (o->get_id()   == sub->get_id())        color = ImColor(0, 0xFF, 0); // eigenes Sub
        else if (o->get_team() != sub->get_team())      color = ImColor(0xFF, 0, 0); // Feindliches Objekt
        else if (o->get_typ()  == Objekt::Typ::TORPEDO) color = ImColor(0xFF, 0xFF, 0); // Torpedo
        draw_list->AddNgonFilled({pos[0], pos[1]}, 4.f, color, 4);
    }

    // Zonen zeichnen
    for (const Zone& zone : zonen) {
        const auto& pos = world2ui(std::get<0>(zone.get_pos()), std::get<1>(zone.get_pos()));
        const ImColor color = zone.get_team() == 0 ? ImColor(0xFF, 0xFF, 0xFF) :
                zone.get_team() == sub->get_team() ? ImColor(0x00,0xFF,0x00) : ImColor(0xFF, 0x00, 0x00);
        draw_list->AddRect({pos[0] - 0.5f * zone.get_groesse() * scale, pos[1] - 0.5f * zone.get_groesse() * scale},
                           {pos[0] + 0.5f * zone.get_groesse() * scale, pos[1] + 0.5f * zone.get_groesse() * scale},
                           color);
    }

    // Karteneinstellungen
    ImGui::SetCursorPosY(size_y + 32);
    ImGui::SliderFloat("Scale", &scale, 0.01, 0.2);

    for (const auto& team : teams) {
        ImGui::Text("Team %u: %u Points", team.first, team.second.get_punkte());
    }
    for (const auto& zone : zonen) {
        ImGui::Text("Zone @ x=%.0f, y=%.0f, owned by Team %u",
                    std::get<0>(zone.get_pos()), std::get<1>(zone.get_pos()), zone.get_team());
    }

    ImGui::End();
}

void Nav_UI::show_navigation(const Sub* sub) const {
    ImGui::SetNextWindowSize({300,0});
    ImGui::Begin("Nav View");
    ImGui::Text("Sub: %.1f %.1f Depth: %.1f", sub->get_pos().x(), sub->get_pos().y(), sub->get_pos().z());
    ImGui::Text("Pitch:   %.1f", sub->get_pitch());
    ImGui::Text("Bearing: %.1f (Target: %.f)", sub->get_bearing(), sub->get_target_bearing());
    ImGui::Text("Speed:   %.1f (Target: %.f)", sub->get_speed(), sub->get_target_speed());

/*    static float target_x = 0, target_z = 0; // - ohne auto-pos mehr immersion?
    ImGui::InputFloat("Target_x", &target_x);
    ImGui::InputFloat("Target_z", &target_z);
    if (ImGui::Button("Set##set_target_pos")) {
        klient->kommando({Kommando::AUTO_POS, sub->get_id(),
                          std::tuple<float, float>(target_x, target_z)});
    }*/

    static float target_bearing = 0;
    ImGui::Nada::KnobDegree("target_bearing", &target_bearing,
                            0.f, 360.f, 1.f, 40.f, 2.f, "%.0f°", std::make_optional(sub->get_bearing()));
    if (ImGui::Button("Set##set_bearing")) {
        klient->kommando({Kommando::AUTO_KURS, sub->get_id(), target_bearing});
    }
    ImGui::Separator();

    static float target_speed = 0;
    ImGui::SliderFloat("target_speed", &target_speed, -sub->get_speed_max(), sub->get_speed_max(), "%.1f");
    if (ImGui::Button("Set##set_speed")) {
        klient->kommando({Kommando::MOTOR_LINEAR, sub->get_id(), target_speed / sub->get_speed_max()});
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
}

void Nav_UI::show_noise_signature(const Sub* sub, std::optional<float> mark_v) const {
    const auto x_max = sub->get_speed_max() + 1;
    ImPlot::SetNextAxisLimits(ImAxis_X1, 0, x_max);
    ImPlot::SetNextAxisLimits(ImAxis_Y1, 0, 1);
    if (ImPlot::BeginPlot("Noise Level", {-1,0}, ImPlotFlags_NoMenus | ImPlotFlags_Crosshairs)) {
        ImPlot::SetupAxes("v in m/s", "Noise Level", ImPlotAxisFlags_Lock, ImPlotAxisFlags_Lock);
        std::vector<float> data_x;
        std::vector<float> data_y_1;
        std::vector<float> data_y_10;
        std::vector<float> data_y_25;
        std::vector<float> data_y_50; // TODO cache those by sub
        for (int v = 0; v <= x_max; v++) {
            data_x.push_back(v);
            data_y_1.push_back(Physik::sichtbarkeit(sub->get_noise(),  static_cast<float>(v), 1000));
            data_y_10.push_back(Physik::sichtbarkeit(sub->get_noise(), static_cast<float>(v), 10000));
            data_y_25.push_back(Physik::sichtbarkeit(sub->get_noise(), static_cast<float>(v), 25000));
            data_y_50.push_back(Physik::sichtbarkeit(sub->get_noise(), static_cast<float>(v), 50000));
        }
        if (mark_v) ImPlot::TagX(mark_v.value(), ImColor(0xFF, 0xFF, 0xFF));
        ImPlot::PlotLine("@ 1km", data_x.data(), data_y_1.data(), data_x.size());
        ImPlot::PlotLine("@ 10km", data_x.data(), data_y_10.data(), data_x.size());
        ImPlot::PlotLine("@ 25km", data_x.data(), data_y_25.data(), data_x.size());
        ImPlot::PlotLine("@ 50km", data_x.data(), data_y_50.data(), data_x.size());
        ImPlot::EndPlot();
    }
}

void Nav_UI::draw_gfx() {

}
