#include <log.hpp>
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
    show_navigation(sub);
    ImGui::End();

    ImGui::Begin("Minimap");
    show_minimap(sub);
    ImGui::End();
}

void Nav_UI::show_minimap(const Sub* sub) const {
    const ImVec2 pos_ui = ImGui::GetCursorScreenPos();
    const auto& pos_sub = sub->get_pos();
    const float size_x = 400.f;
    const float size_y = 400.f;
    const float center_x = pos_ui.x + 0.5f*size_x;
    const float center_y = pos_ui.y + 0.5f*size_y;
    float scale = 0.1f;

    const auto world2ui = [&] (float x, float y) {
        std::array<float, 2> coords;
        coords[0] = center_x + scale * (x - pos_sub.x);
        coords[1] = center_y + scale * (y - pos_sub.y);
        return coords;
    };

    // Ozean zeichnen + Mitte markieren
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(pos_ui, {pos_ui.x + size_x, pos_ui.y + size_y},
                             ImColor(0x00, 0x50, 0xB0), 0.0f);
    draw_list->AddNgonFilled({center_x, center_y}, 4.f, ImColor(0xFF'FF'FF'FF), 4);

    // Objekte synchronisieren
    static std::vector<Objekt> objekte;
    if (static Ogre::Timer timer; timer.getMilliseconds() > SYNC_INTERVALL) {
        const std::string& objekte_raw = klient->request(Net::ALLE_OBJEKTE);
        if (!objekte_raw.empty()) {
            try { objekte = Net::deserialize<std::vector<Objekt>>(objekte_raw); }
            catch (const std::exception& e) {
                Log::err() << "Error: Konnte Antwort auf Net::ALLE_OBJEKTE nicht deserialisieren, Größe: ";
                Log::err() << objekte_raw.size() << " Fehler: " << e.what() << '\n';
            }
        }
    }
    // Objekte zeichnen
    for (const Objekt& o : objekte) {
        if (fow && o.get_team() != sub->get_team()) continue; // fremdes Team
        if (o.get_id() == sub->get_id()) continue; // eigenes Sub
        const auto pos = world2ui(o.get_pos().x, o.get_pos().z);
        const float x = pos[0];
        const float y = pos[1];
        //if (x < 0 || x > size_x || y < 0 || y > size_y) continue; // außerhalb des Bildes
        draw_list->AddNgonFilled({x, y}, 4.f, ImColor(0xFF, 0xFF, 0xFF), 4);
    }

    // Zonen zeichnen
    static std::vector<Zone> zonen = klient->get_zonen();
    if (static Ogre::Timer timer; timer.getMilliseconds() > SYNC_INTERVALL) zonen = klient->get_zonen();
    for (const Zone& zone : zonen) {
        const auto& pos = world2ui(std::get<0>(zone.get_pos()), std::get<1>(zone.get_pos()));
        const ImColor color = zone.get_team() == 0 ? ImColor(0xFF, 0xFF, 0xFF) :
                zone.get_team() == sub->get_team() ? ImColor(0x00,0xFF,0x00) : ImColor(0xFF, 0x00, 0x00);
        draw_list->AddRect({pos[0] - 0.5f * zone.get_groesse() * scale, pos[1] - 0.5f * zone.get_groesse() * scale},
                           {pos[0] + 0.5f * zone.get_groesse() * scale, pos[1] + 0.5f * zone.get_groesse() * scale},
                           color);
    }
}

void Nav_UI::show_navigation(const Sub* sub) const {
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
}
