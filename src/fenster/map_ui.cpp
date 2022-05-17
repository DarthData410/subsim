#include "map_ui.hpp"
#include "gfx/ui.hpp"
#include "../sim/net/net.hpp"
#include "../sim/net/klient.hpp"
#include "../sim/game/karte.hpp"
#include "../sim/physik.hpp"

#include <nada/log.hpp>
#include <SimplexNoise.h>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>

using nada::Log;

Map_UI::Map_UI(Klient* klient) : Standard_UI(klient) {
    //
}

void Map_UI::update_and_show(const Sub* sub) {
    ui::Font f(ui::FONT::MONO_16);
    sync(objekte.empty());
    handle_imgui_events();
    show_minimap(sub);
}

void Map_UI::sync(bool force) {
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

void Map_UI::handle(sf::Event* event) {
    (void)event;
}

void Map_UI::handle_imgui_events() {
    const auto& io = ImGui::GetIO();
    /// Karte verschieben
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
        const auto& delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle);
        shift_x += 0.5f * delta.x;
        shift_y += 0.5f * delta.y;
        ImGui::ResetMouseDragDelta(ImGuiMouseButton_Middle);
    }
    /// Zoom via Mausrad
    if (!ImGui::IsAnyItemHovered() && std::abs(io.MouseWheel) > 0) {
        scale += (0.25f * scale * io.MouseWheel);
        scale = std::clamp(scale, 0.005f, 1.f);
    }
}

void Map_UI::draw_gfx(const Sub* sub, sf::RenderWindow* window) {
    const auto& pos_sub = sub->get_pos();
    const float size_x = window->getSize().x;
    const float size_y = window->getSize().y;
    const float center_x = 0.5f * size_x;
    const float center_y = 0.5f * size_y;

    /// Konvertiert Welt zu Fenster-Koordinaten (UI).
    const auto world2ui = [&] (float x, float y) {
        return sf::Vector2<float> {
                static_cast<float>(center_x + shift_x + (scale * (x - pos_sub.x()))),
                static_cast<float>(center_y + shift_y - (scale * (y - pos_sub.y())))
        };
    };
    /// Konvertiert von Fenster-Koordinaten zu Welt-Koordinaten
    const auto ui2world = [&] (float x, float y) { return std::tuple<float,float> {
            static_cast<float>(((x - center_x - shift_x) / scale) + pos_sub.x()),
            static_cast<float>(((y - center_y - shift_y) / scale) + pos_sub.y()) };
    };

    // Kartenrenderer
    static Karte karte = klient->get_karte();
    if (sf::Clock sync_karte; sync_karte.getElapsedTime().asSeconds() > 3) { karte = klient->get_karte(); sync_karte.restart(); }
    const auto& [mouse_world_x, mouse_world_y] = ui2world(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
    height_at_mouse = karte.get_height_at(mouse_world_x, mouse_world_y); // TODO nur in Debug (?)

    const unsigned vsize = 16; // Vertex-Größe
    for (unsigned x = 0; x <= size_x; x += vsize) for (unsigned y = 0; y <= size_y; y += vsize) {
            sf::VertexArray va(sf::Quads, 4);
            std::array<uint8_t, 4> rgba;
            const auto [x0, y0] = ui2world(x, y);
            const auto [x1, y1] = ui2world(x+vsize, y+vsize);
            rgba = karte.get_rgba_at(x0, y1);
            va[0] = sf::Vertex({static_cast<float>(x), static_cast<float>(y+vsize)},
                               {rgba[0], rgba[1], rgba[2], rgba[3]});
            rgba = karte.get_rgba_at(x0, y0);
            va[1] = sf::Vertex({static_cast<float>(x), static_cast<float>(y)},
                               {rgba[0], rgba[1], rgba[2], rgba[3]});
            rgba = karte.get_rgba_at(x1, y0);
            va[2] = sf::Vertex({static_cast<float>(x+vsize), static_cast<float>(y)},
                               {rgba[0], rgba[1], rgba[2], rgba[3]});
            rgba = karte.get_rgba_at(x1, y1);
            va[3] = sf::Vertex({static_cast<float>(x+vsize), static_cast<float>(y+vsize)},
                               {rgba[0], rgba[1], rgba[2], rgba[3]});
            window->draw(va);
    }

    // Skala
    const int skala_size_real = std::max(1000.f, Physik::round(200.f / scale, 1000.f));
    sf::RectangleShape skala_rect;
    skala_rect.setPosition(20.f, size_y - 40.f);
    skala_rect.setSize({skala_size_real * scale, 16.f});
    sf::Text skala_txt(std::to_string(skala_size_real/1000) + " km", *ui::get_font(), 16);
    skala_txt.setPosition(skala_rect.getPosition().x, skala_rect.getPosition().y - skala_rect.getSize().y - skala_txt.getCharacterSize() - 8);
    window->draw(skala_txt);
    window->draw(skala_rect);

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
        if      (o->get_id()   == sub->get_id())        color = sf::Color(0xFF, 0xFF, 0xFF); // eigenes Sub
        else if (o->get_team() != sub->get_team())      color = sf::Color(0xFF, 0, 0); // Feindliches Objekt
        else if (o->get_typ()  == Objekt::Typ::TORPEDO) color = sf::Color(0xFF, 0xFF, 0); // Torpedo
        else if (o->get_typ()  == Objekt::Typ::PING)    color = sf::Color(0, 0, 0xFF); // Sonar Ping
        else if (o->get_team() == sub->get_team())      color = sf::Color(0, 0xFF, 0); // freundliches Objekt
        sf::CircleShape shape(8.f, 4);
        const auto& o_map_pos = world2ui(o->get_pos().x(), o->get_pos().y());
        shape.setPosition(o_map_pos);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(color);
        shape.setOutlineThickness(2);
        window->draw(shape);
    }
}

void Map_UI::show_minimap(const Sub* sub) const {
    (void) sub;
    ImGui::Begin("Map Settings");
    if (ui::Button("Center on Sub")) { shift_x = 0; shift_y = 0; }
    ui::Text("Height @ Mouse = %.1f", height_at_mouse); // TODO nur in Debug (?)
    ui::SliderFloat("Scale", &scale, 0.001, 0.5);

    for (const auto& team : teams) {
        ui::Text("Team %u: %u Points", team.first, team.second.get_punkte());
    }
    for (const auto& zone : zonen) {
        ui::Text("Zone @ x=%.0f, y=%.0f, owned by Team %u",
                 std::get<0>(zone.get_pos()), std::get<1>(zone.get_pos()), zone.get_team());
    }
    ImGui::End();
}
