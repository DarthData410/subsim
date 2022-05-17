#include "sonar_ui.hpp"
#include "../sim/game/objekte/sub.hpp"
#include "../sim/physik.hpp"
#include "gfx/grafik.hpp"
#include "gfx/ui.hpp"
#include "../sim/net/klient.hpp"

#include <nada/random.hpp>
#include <selbaward/Ring.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace {
    const unsigned RGB_PX = 4; // RGBA
    const uint8_t  BG_COL = 0x40;
    const unsigned AS_RADIUS  =  300;
    const unsigned AS_SIZE_X  =  AS_RADIUS * 2;
    const unsigned AS_SIZE_Y  =  AS_RADIUS * 2;
    const unsigned AS_POS_X   = 1230;
    const unsigned AS_POS_Y   =   82;
    const unsigned AS_MITTE_X     = AS_SIZE_X / 2;
    const unsigned AS_MITTE_Y     = AS_SIZE_Y / 2;
    const unsigned AS_MITTE_X_ABS = AS_POS_X + AS_MITTE_X;
    const unsigned AS_MITTE_Y_ABS = AS_POS_Y + AS_MITTE_Y;
    const unsigned PS_SIZE_X =  432;
    const unsigned PS_SIZE_Y =  660;
    const unsigned PS_POS_X  =  112;
    const unsigned PS_POS_Y  =   80;
    const unsigned PS_LINE_WIDTH = PS_SIZE_X * RGB_PX;
    sf::CircleShape    as_circ;
    sw::Ring           as_ring; // Aufdeckend
    sf::RectangleShape ps_rect;
    gfx::Grafik bg("data/gfx/bg_sonar.png");
    sf::Color SOFT_GREEN(0, 0xFF, 0, 0x80);
}

Sonar_UI::Sonar_UI() : Standard_UI(nullptr) {}
Sonar_UI::Sonar_UI(Klient* klient) : Standard_UI(klient),
    as_tex(new sf::RenderTexture()),
    ps_tex(new sf::Texture()),
    ps_data(PS_SIZE_X * PS_SIZE_Y * RGB_PX, BG_COL)
{
    // Setup Active Sonar Texture + Render-Circle
    as_tex->create(AS_SIZE_X, AS_SIZE_Y);
    as_tex->clear({BG_COL, BG_COL, BG_COL, BG_COL});
    as_circ.setRadius(AS_RADIUS);
    as_circ.setPosition(AS_POS_X, AS_POS_Y);
    as_circ.setTexture(&as_tex->getTexture());
    as_ring.setRadius(AS_RADIUS);
    as_ring.setPosition(AS_POS_X, AS_POS_Y);
    as_ring.setColor(sf::Color::Black);
    as_ring.setHole(0);

    // Setup Passive Sonar Texture + Render-Rect
    ps_tex->create(PS_SIZE_X, PS_SIZE_Y);
    ps_tex->update(ps_data.data());
    ps_rect.setSize({static_cast<float>(ps_tex->getSize().x), static_cast<float>(ps_tex->getSize().y)});
    ps_rect.setPosition(PS_POS_X, PS_POS_Y);
    ps_rect.setTexture(ps_tex.get());
}

void Sonar_UI::update_and_show(const Sub* sub) {
    const Sonar_Passiv& ps = sub->get_sonars_passive().at(ps_array_select-1);
    const ui::Font font(ui::FONT::MONO_20, ImColor(0.f, 1.0f, 0.f));
    ui::BeginInvisible("Sonar_Passive_Config", {130, 836}, {405, 215});
    ui::SliderInt("Array Select", &ps_array_select, 1, sub->get_sonars_passive().size(), "#%d");
    ui::Text("Selected Sonar Model: %s", ps.get_name().c_str());
    ui::SliderFloat("Update Intervall", &ps_intervall, 0.2f, 10.f, "%.1fs");
    ui::MouseWheel(ps_intervall, 0.1f, 0.2f, 10.f);
    if (ui::Button("Clear")) {
        std::memset(ps_data.data(), BG_COL, ps_data.size());
        ps_tex->update(ps_data.data());
    }
    ImGui::End();

    // AS Select
    const Sonar_Aktiv& as = sub->get_sonars_active().at(as_array_select-1);
    auto as_clear = [&]() {
        as_tex->clear({BG_COL, BG_COL, BG_COL, BG_COL});
        as_ring.setHole(0); // komplett verdecken
        as_last_ping_timer.reset();
    };
    ui::BeginInvisible("Sonar_Active_Config", {1309, 788}, {489, 259});
    ui::SliderInt("Array Select", &as_array_select, 1, sub->get_sonars_active().size(), "#%d");
    ui::Text("Selected Sonar Model: %s (%.1fkm Range)", as.get_name().c_str(), as.get_max_range() / 1000.f);
    ui::Text("Ping Mode");
    Sonar_Aktiv::Mode modus = as.get_mode();
    ImGui::SameLine(); if (ui::RadioButton("OFF", as.get_mode() == Sonar_Aktiv::Mode::OFF)) modus = Sonar_Aktiv::Mode::OFF;
    ImGui::SameLine(); if (ui::RadioButton("SINGLE", as.get_mode() == Sonar_Aktiv::Mode::SINGLE)) modus = Sonar_Aktiv::Mode::SINGLE;
    ImGui::SameLine(); if (ui::RadioButton("ON", as.get_mode() == Sonar_Aktiv::Mode::ON)) modus = Sonar_Aktiv::Mode::ON;
    ImGui::SameLine(); if (ui::RadioButton("CLEAR", false)) as_clear();
    if (modus != as.get_mode()) klient->kommando(Kommando(Kommando::SONAR_A_MODE, sub->get_id(),
                                                          std::tuple<uint8_t, Sonar_Aktiv::Mode>(as_array_select-1, modus)));
    ui::Text("Ping Intervall: %.1fs", as.get_intervall()); // TODO Kommando zum Anpassen
    if (as_last_ping_timer.has_value()) ui::Text("Last Ping: %.1fs", as_last_ping_timer->getElapsedTime().asSeconds());
    else ui::TextUnformatted("Last Ping: -");
    ui::Text("Total Pings: %d", as.get_ping_counter());

    // AS Range Settings
    ui::Checkbox("Range Rings", &as_range_rings);
    ui::TextUnformatted("Set Range");
    if (as.get_max_range() > 1'500)  { ImGui::SameLine(); if (ui::Button("1.5k##as_range_1.5k")) { as_scale = AS_RADIUS / 1500.f;  as_clear(); }}
    if (as.get_max_range() > 3'000)  { ImGui::SameLine(); if (ui::Button("3k##as_range_3k"))   { as_scale = AS_RADIUS / 3000.f;  as_clear(); }}
    if (as.get_max_range() > 9'000)  { ImGui::SameLine(); if (ui::Button("9k##as_range_9k"))   { as_scale = AS_RADIUS / 9000.f;  as_clear(); }}
    if (as.get_max_range() > 30'000) { ImGui::SameLine(); if (ui::Button("30k##as_range_30k")) { as_scale = AS_RADIUS / 30000.f; as_clear(); }}
    ImGui::SameLine(); if (ui::Button("Max##as_range_max")) { as_scale = AS_RADIUS / as.get_max_range(); as_clear(); }
    //if (AS_RADIUS / as_scale > as.get_max_range()) as_scale = AS_RADIUS / as.get_max_range(); // Reichweite nicht überschreiten
    ImGui::End();
}

void Sonar_UI::draw_gfx(const Sub* sub, sf::RenderWindow* window) {
    bg.draw(window);
    draw_as(sub, window);
    draw_ps(sub, window);
}

void Sonar_UI::draw_ps(const Sub* sub, sf::RenderWindow* window) {
    // Konvertiert 0°-360°-Kurs-Koordinaten zu x-Koordinaten der Sonar-Anzeige
    auto convert = [](float degree) {
        if (degree > 360.f) degree -= 360.f;
        if (degree < 0.f)   degree += 360.f;
        return std::clamp(static_cast<unsigned>(std::round((degree * static_cast<float>(PS_SIZE_X)) / 360.f)),
                          0u, PS_SIZE_X - 1u);
    };

    // 1 Line Update
    if (static sf::Clock timer; timer.getElapsedTime().asSeconds() > ps_intervall) {
        timer.restart();
        const auto& sonar = sub->get_sonars_passive().at(ps_array_select-1);
        const unsigned res = std::max(1u, convert(sonar.get_aufloesung()));

        // Rauschen eintragen
        std::vector<uint8_t> v(PS_SIZE_X);
        for (unsigned i = 0; i < v.size(); i += res) {
            const uint8_t rnd = nada::random::ui(0, 0x10);
            for (unsigned k = 0; k < res && i+k < v.size(); ++k) v[i+k] = rnd;
        }

        // Detektionen zu x-Koordinaten konvertieren / eintragen
        for (const auto& d : sonar.get_detektionen()) {
            const unsigned pos = convert(d.bearing);
            const uint8_t val = std::clamp(static_cast<int>(v.at(pos) + (d.gain * 255.f)), 0, 0xFF);
            for (unsigned i = pos; i < pos + res && i < v.size(); ++i) {
                v[i] = val;
            }
        }

        // Neue Zeile eintragen
        for (unsigned i = 0; i < PS_LINE_WIDTH; i+=RGB_PX) {
            const auto pos = (PS_LINE_WIDTH * (PS_SIZE_Y-1)) + i;
            ps_data[pos]   = 0x00; // R
            ps_data[pos+1] = v.at(i/RGB_PX); // G
            ps_data[pos+2] = 0x00; // B
            ps_data[pos+3] = 0xFF; // A
        }
        // Neue Zeile nach oben schieben
        for (unsigned i = 0; i < res; ++i) {
            std::memmove(ps_data.data(), ps_data.data() + PS_LINE_WIDTH, ps_data.size() - PS_LINE_WIDTH);
        }
        ps_tex->update(ps_data.data()); // In Textur übernehmen
    }
    window->draw(ps_rect);

    // Skala eintragen
    for (unsigned i = 0; i < 360; i += 40) {
        sf::Text marker(std::to_string(i), *ui::get_font(), ui::FONT_SIZE_SFML);
        marker.setFillColor(SOFT_GREEN);
        const auto pos_x = std::min(PS_POS_X + convert(i), PS_POS_X + PS_SIZE_X - 20);
        const auto pos_y = PS_POS_Y + PS_SIZE_Y;
        marker.setPosition(pos_x, pos_y - 16.f);
        window->draw(marker);
        if (i > 0) { // kein Marker für die Ränder
            const sf::Vertex line[] = {
                    sf::Vertex({static_cast<float>(pos_x), pos_y - 16.f}, SOFT_GREEN),
                    sf::Vertex({static_cast<float>(pos_x), pos_y}, SOFT_GREEN)
            };
            window->draw(line, 2, sf::Lines);
        }
    }
}

void Sonar_UI::draw_as(const Sub* sub, sf::RenderWindow* window) {
    const Sonar_Aktiv& as = sub->get_sonars_active().at(as_array_select-1);

    if (as.get_ping_counter() != as_last_ping) { // Refresh ausführen?
        as_ring.setHole(0); // Verdecken
        as_last_ping = as.get_ping_counter();
        as_last_ping_timer = sf::Clock();
        as_tex->clear({BG_COL, BG_COL, BG_COL, BG_COL});

        // Rauschen generieren
        sf::Texture tex_rauschen;
        tex_rauschen.create(AS_SIZE_X, AS_SIZE_Y);
        std::vector<uint8_t> v_rauschen(AS_SIZE_X * AS_SIZE_Y * RGB_PX);
        for (unsigned i = 0; i < v_rauschen.size(); i += RGB_PX) {
            v_rauschen[i]   = 0x00; // R
            v_rauschen[i+1] = nada::random::i(0, 0x20); // G
            v_rauschen[i+2] = 0x00; // B
            v_rauschen[i+3] = 0xFF; // A
        }
        tex_rauschen.update(v_rauschen.data());
        sf::RectangleShape rect_rauschen({AS_SIZE_X, AS_SIZE_Y});
        rect_rauschen.setTexture(&tex_rauschen);
        as_tex->draw(rect_rauschen);

        // Detektionen eintragen //std::fmod(540.f-d.bearing, 360.f)
        for (const auto& d : as.get_detektionen()) {
            if (!d.range) throw std::runtime_error("Active Sonar detection without range is invalid.\n");
            const auto punkt = Physik::get_punkt(0, 0, d.bearing, d.range.value() * as_scale);
            sf::RectangleShape rect({2.f, 2.f});
            const uint8_t brightness = static_cast<uint8_t>(0xFF * d.gain);
            rect.setFillColor({0x00, brightness, 0x00, brightness});
            rect.setPosition(0.5f * AS_SIZE_X + punkt.first, 0.5f * AS_SIZE_Y - punkt.second);
            as_tex->draw(rect);
        }
        // Mitten-Marker
        sf::RectangleShape rect({3.f, 3.f}); // Mitte
        rect.setFillColor(sf::Color::White);
        rect.setPosition(AS_MITTE_X-1, AS_MITTE_Y-1);
        as_tex->draw(rect);
        as_tex->display();
        as_circ.setTexture(&as_tex->getTexture());
    }
    window->draw(as_circ);

    // Per Ring aufdecken
    if (static sf::Clock ring_clock; as_last_ping_timer && ring_clock.getElapsedTime().asSeconds() > 0.1f) {
        ring_clock.restart();
        const float hole_grow = (1.0f - as_ring.getHole()) * 0.0078125f * (as_scale / 0.1f); // TODO scale
        as_ring.setHole(std::min(as_ring.getHole() + hole_grow, 1.0f));
    }
    window->draw(as_ring);

    // Range Ringe
    if (as_range_rings) {
        auto put_marker = [&](int ring_m, float pos_x, float pos_y) {
            std::string str_marker(std::to_string(ring_m));
            if (ring_m > 1000) {
                str_marker = std::to_string(ring_m/1000.f);
                if (str_marker.find(".0") != std::string::npos) str_marker = str_marker.substr(0, str_marker.find('.'));
                else if (str_marker.find('.') != std::string::npos) str_marker = str_marker.substr(0, str_marker.find('.') + 2);
                str_marker += 'k';
            }
            sf::Text marker(str_marker, *ui::get_font(), ui::FONT_SIZE_SFML);
            marker.setFillColor(SOFT_GREEN);
            marker.setPosition(pos_x, pos_y);
            window->draw(marker);
        };
        for (float radius = 50.f; radius < AS_RADIUS; radius += 50.f) {
            sf::CircleShape rr(radius);
            rr.setFillColor(sf::Color::Transparent);
            rr.setOutlineThickness(1);
            rr.setOutlineColor(SOFT_GREEN);
            rr.setPosition(AS_POS_X + AS_RADIUS - rr.getRadius(), AS_POS_Y + AS_RADIUS - rr.getRadius());
            window->draw(rr);
            // Text (Entfernung vom Sub)
            const int ring_m = static_cast<int>(std::round(radius / as_scale));
            put_marker(ring_m, rr.getPosition().x + rr.getRadius(), rr.getPosition().y);
        }
        put_marker(AS_RADIUS / as_scale, AS_MITTE_X_ABS, AS_POS_Y); // Oberster Marker ohne Ring

        // Mouse Nav
        static auto maus_pos = ImGui::GetMousePos();
        static auto maus_richtung = Physik::kurs(AS_MITTE_X_ABS, AS_MITTE_Y_ABS, maus_pos.x, maus_pos.y);
        static auto line_bearing_mitte = Physik::get_punkt(AS_MITTE_X_ABS, AS_MITTE_Y_ABS, maus_richtung, 0.5f*AS_RADIUS);
        static auto line_bearing_ende  = Physik::get_punkt(AS_MITTE_X_ABS, AS_MITTE_Y_ABS, maus_richtung, AS_RADIUS);
        static sf::Text marker("", *ui::get_font(), ui::FONT_SIZE_SFML);
        static std::array<sf::Vertex, 2> line_bearing = {
                sf::Vertex({static_cast<float>(AS_MITTE_X_ABS), static_cast<float>(AS_MITTE_Y_ABS)}, SOFT_GREEN),
                sf::Vertex({static_cast<float>(line_bearing_ende.first), static_cast<float>(line_bearing_ende.second)}, SOFT_GREEN)
        };
        // Refresh?
        if (const auto maus_pos_neu = ImGui::GetMousePos(); Physik::distanz(maus_pos_neu.x, maus_pos_neu.y, AS_MITTE_X_ABS, AS_MITTE_Y_ABS) <= AS_RADIUS) {
            maus_pos = maus_pos_neu; // Maus im AS-Radius-Bereich
            maus_richtung = Physik::kurs(AS_MITTE_X_ABS, AS_MITTE_Y_ABS, maus_pos.x, maus_pos.y);
            line_bearing_mitte = Physik::get_punkt(AS_MITTE_X_ABS, AS_MITTE_Y_ABS, maus_richtung, 0.5f*AS_RADIUS);
            line_bearing_ende  = Physik::get_punkt(AS_MITTE_X_ABS, AS_MITTE_Y_ABS, maus_richtung, AS_RADIUS);
            line_bearing = {
                    sf::Vertex({static_cast<float>(AS_MITTE_X_ABS), static_cast<float>(AS_MITTE_Y_ABS)}, SOFT_GREEN),
                    sf::Vertex({static_cast<float>(line_bearing_ende.first), static_cast<float>(line_bearing_ende.second)}, SOFT_GREEN)
            };
            marker.setString(std::to_string(static_cast<int>(540-std::round(maus_richtung))%360));
            marker.setFillColor(sf::Color::Green);
            marker.setPosition(line_bearing_mitte.first, line_bearing_mitte.second);
        }
        window->draw(line_bearing.data(), 2, sf::Lines);
        window->draw(marker);
    }
}
