#include "nav_ui.hpp"
#include "imgui_addons.hpp"
#include "../sim/net/klient.hpp"
#include "../sim/physik.hpp"

#include <log.hpp>
#include <implot.h>
#include <cmath>
#include <cereal/types/memory.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

Nav_UI::Nav_UI() : Standard_UI(nullptr) {}

Nav_UI::Nav_UI(Klient* klient) : Standard_UI(klient) {}

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
    handle_imgui_events();
    show_navigation(sub);
    show_minimap(sub);

    ImGui::Begin("Noise signature");
    show_noise_signature(sub, sub->get_speed());
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

void Nav_UI::show_minimap(const Sub* sub) const {
    (void) sub;
    ImGui::Begin("Map Settings");
    if (ImGui::Button("Center on Sub")) { shift_x = 0; shift_y = 0; }
    ImGui::SliderFloat("Scale", &scale, 0.001, 0.5);

    for (const auto& team : teams) {
        ImGui::Text("Team %u: %u Points", team.first, team.second.get_punkte());
    }
    for (const auto& zone : zonen) {
        ImGui::Text("Zone @ x=%.0f, y=%.0f, owned by Team %u",
                    std::get<0>(zone.get_pos()), std::get<1>(zone.get_pos()), zone.get_team());
    }
    ImGui::End();
}

void Nav_UI::draw_gfx(const Sub* sub, sf::RenderWindow* window) {
    const auto& pos_sub = sub->get_pos();
    const float size_x = window->getSize().x;
    const float size_y = window->getSize().y;
    const float center_x = 0.5f * size_x;
    const float center_y = 0.5f * size_y;

    const auto world2ui = [&] (float x, float y) {
        return sf::Vector2<float> {
                static_cast<float>(center_x + shift_x + (scale * (x - pos_sub.x()))),
                static_cast<float>(center_y + shift_y - (scale * (y - pos_sub.y())))
        };
    };

    // Ozean zeichnen

    // Zonen zeichnen
    for (const Zone& zone : zonen) {
        const auto& pos = world2ui(std::get<0>(zone.get_pos()), std::get<1>(zone.get_pos()));
        const sf::Color color = zone.get_team() == 0 ?             sf::Color(0xFF, 0xFF, 0xFF) : // Kein Besitzer: Weiß
                              zone.get_team() == sub->get_team() ? sf::Color(0x00, 0xFF, 0x00) :   // Eigenes Team: Grün
                                                                   sf::Color(0xFF, 0x00, 0x00);  // Feindlich: Rot
        /*draw_list->AddRect({pos[0] - 0.5f * zone.get_groesse() * scale, pos[1] - 0.5f * zone.get_groesse() * scale},
                           {pos[0] + 0.5f * zone.get_groesse() * scale, pos[1] + 0.5f * zone.get_groesse() * scale},
                           color);*/
        const float r = 0.5f * zone.get_groesse() * scale;
        sf::RectangleShape zone_rect({2 * r, 2 * r});
        zone_rect.setPosition(pos.x - r, pos.y - r);
        zone_rect.setOutlineThickness(2);
        zone_rect.setOutlineColor(color);
        zone_rect.setFillColor(sf::Color::Transparent);
        window->draw(zone_rect);
    }

    // Objekte zeichnen
    for (const auto& o : objekte) {
        //if (fow && o.get_team() != sub->get_team()) continue; // fremdes Team TODO
        //if (x < 0 || x > size_x || y < 0 || y > size_y) continue; // außerhalb des Bildes
        sf::Color color(0xFF, 0xFF, 0xFF);
        if      (o->get_id()   == sub->get_id())        color = sf::Color(0, 0xFF, 0); // eigenes Sub
        else if (o->get_team() != sub->get_team())      color = sf::Color(0xFF, 0, 0); // Feindliches Objekt
        else if (o->get_typ()  == Objekt::Typ::TORPEDO) color = sf::Color(0xFF, 0xFF, 0); // Torpedo
        sf::CircleShape shape(8.f, 4);
        const auto& o_map_pos = world2ui(o->get_pos().x(), o->get_pos().y());
        shape.setPosition(o_map_pos);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(color);
        shape.setOutlineThickness(2);
        window->draw(shape);
    }
}

void Nav_UI::handle_imgui_events() {
    const auto& io = ImGui::GetIO();
    /// Karte verschieben
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
        const auto& delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle);
        shift_x += 0.5f * delta.x;
        shift_y += 0.5f * delta.y;
        ImGui::ResetMouseDragDelta(ImGuiMouseButton_Middle);
    }
    /// Zoom via Mausrad
    if (std::abs(io.MouseWheel) > 0) {
        scale += (0.25f * scale * io.MouseWheel);
        scale = std::clamp(scale, 0.001f, 1.f);
    }
}
