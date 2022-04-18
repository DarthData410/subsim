#include "sonar_ui.hpp"
#include "../sim/game/objekte/sub.hpp"
#include "imgui_addons.hpp"
#include "../gfx/grafik.hpp"
#include "../sim/net/klient.hpp"

#include <zufall.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace {
    const unsigned RGB_PX = 4; // RGBA
    const uint8_t  BG_COL = 0x40;
    const unsigned AS_RADIUS =  300;
    const unsigned AS_SIZE_X =  AS_RADIUS * 2;
    const unsigned AS_SIZE_Y =  AS_RADIUS * 2;
    const unsigned AS_POS_X  = 1230;
    const unsigned AS_POS_Y  =   82;
    const unsigned PS_SIZE_X =  432;
    const unsigned PS_SIZE_Y =  660;
    const unsigned PS_POS_X  =  112;
    const unsigned PS_POS_Y  =   80;
    const unsigned PS_LINE_WIDTH = PS_SIZE_X * RGB_PX;
    sf::CircleShape as_circ;
    sf::RectangleShape ps_rect;
    Grafik bg("data/gfx/bg_sonar.png");
}

Sonar_UI::Sonar_UI() : Standard_UI(nullptr) {}
Sonar_UI::Sonar_UI(Klient* klient) : Standard_UI(klient),
    as_data(AS_SIZE_X * AS_SIZE_Y * RGB_PX, BG_COL),
    ps_data(PS_SIZE_X * PS_SIZE_Y * RGB_PX, BG_COL),
    as_tex(new sf::Texture()),
    ps_tex(new sf::Texture())
{
    // Setup Active Sonar Texture + Render-Circle
    as_tex->create(AS_SIZE_X, AS_SIZE_Y);
    as_tex->update(as_data.data());
    as_circ.setRadius(AS_RADIUS);
    as_circ.setPosition(AS_POS_X, AS_POS_Y);
    as_circ.setTexture(as_tex.get());

    // Setup Passive Sonar Texture + Render-Rect
    ps_tex->create(PS_SIZE_X, PS_SIZE_Y);
    ps_tex->update(ps_data.data());
    ps_rect.setSize({static_cast<float>(ps_tex->getSize().x), static_cast<float>(ps_tex->getSize().y)});
    ps_rect.setPosition(PS_POS_X, PS_POS_Y);
    ps_rect.setTexture(ps_tex.get());
}

void Sonar_UI::update_and_show(const Sub* sub) {
    // Passive Sonar Config
    const auto flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground;
    ImGui::SetNextWindowPos({130, 836});
    ImGui::SetNextWindowSize({405,215});
    ImGui::Begin("Sonar_Passive_Config", nullptr, flags);
    ImGui::SliderFloat("Update Intervall", &ps_intervall, 0.2f, 10.f, "%.1fs");
    ImGui::SliderInt("Array Select", &ps_array_select, 1, sub->get_sonars_passive().size(), "#%d");
    if (ImGui::Button("Clear")) {
        std::memset(ps_data.data(), BG_COL, ps_data.size());
        ps_tex->update(ps_data.data());
    }
    ImGui::End();

    // AS Select
    const Sonar_Aktiv& as = sub->get_sonars_active().at(as_array_select-1);
    ImGui::Begin("Sonar_Active_Config", nullptr);
    ImGui::SliderInt("Array Select", &as_array_select, 1, sub->get_sonars_active().size(), "#%d");
    ImGui::Text("Selected Sonar Model: %s", as.get_name().c_str());
    ImGui::Text("Ping Mode");
    Sonar_Aktiv::Mode modus = as.get_mode();
    if (ImGui::RadioButton("OFF", as.get_mode() == Sonar_Aktiv::Mode::OFF)) modus = Sonar_Aktiv::Mode::OFF;
    ImGui::SameLine();
    if (ImGui::RadioButton("SINGLE", as.get_mode() == Sonar_Aktiv::Mode::SINGLE)) modus = Sonar_Aktiv::Mode::SINGLE;
    ImGui::SameLine();
    if (ImGui::RadioButton("ON", as.get_mode() == Sonar_Aktiv::Mode::ON)) modus = Sonar_Aktiv::Mode::ON;
    if (modus != as.get_mode()) klient->kommando(Kommando(Kommando::SONAR_A_MODE, sub->get_id(),
                                                          std::tuple<uint8_t, Sonar_Aktiv::Mode>(as_array_select-1, modus)));
    if (ImGui::Button("Clear")) {
        std::memset(as_data.data(), BG_COL, as_data.size());
        as_tex->update(as_data.data());
    }
    // TODO:
    // Ping Intervall
    // ImGui::Text("Range Rings");
    ImGui::End();
}

void Sonar_UI::draw_gfx(const Sub* sub, sf::RenderWindow* window) {
    bg.draw(window);
    draw_as(sub, window);
    draw_ps(sub, window);
}

void Sonar_UI::draw_ps(const Sub* sub, sf::RenderWindow* window) {
    /// 1 Line Update
    if (static sf::Clock timer; timer.getElapsedTime().asSeconds() > ps_intervall) {
        timer.restart();
        const auto& sonar = sub->get_sonars_passive().at(ps_array_select-1);

        /// Konvertiert 0°-360°-Kurs-Koordinaten zu x-Koordinaten der Sonar-Anzeige
        auto convert = [](float degree) {
            return static_cast<unsigned>(std::round(((degree * static_cast<float>(PS_SIZE_X)) / 360.f)));
        };
        const unsigned res = std::max(1u, convert(sonar.get_aufloesung()));

        /// Rauschen eintragen
        std::vector<uint8_t> v(PS_SIZE_X);
        for (unsigned i = 0; i < v.size(); i += res) {
            const uint8_t rnd = Zufall::ui(0, 0x10);
            for (unsigned k = 0; k < res && i+k < v.size(); ++k) v[i+k] = rnd;
        }

        /// Detektionen zu x-Koordinaten konvertieren / eintragen
        for (const auto& d : sonar.get_detektionen()) {
            const unsigned pos = convert(d.bearing);
            const uint8_t val = std::clamp(static_cast<int>(v.at(pos) + (d.gain * 255.f)), 0, 0xFF);
            for (unsigned i = pos; i < pos + res && i < v.size(); ++i) {
                v[i] = val;
            }
        }

        /// Neue Zeile eintragen
        for (unsigned i = 0; i < PS_LINE_WIDTH; i+=RGB_PX) {
            const auto pos = (PS_LINE_WIDTH * (PS_SIZE_Y-1)) + i;
            ps_data[pos]   = 0x00; // R
            ps_data[pos+1] = v.at(i/RGB_PX); // G
            ps_data[pos+2] = 0x00; // B
            ps_data[pos+3] = 0xFF; // A
        }
        /// Neue Zeile nach oben schieben
        for (unsigned i = 0; i < res; ++i) {
            std::memmove(ps_data.data(), ps_data.data() + PS_LINE_WIDTH, ps_data.size() - PS_LINE_WIDTH);
        }
        ps_tex->update(ps_data.data()); // In Textur übernehmen
    }
    window->draw(ps_rect);
}

void Sonar_UI::draw_as(const Sub*, sf::RenderWindow* window) {
    if (static sf::Clock timer; timer.getElapsedTime().asSeconds() > 3) {
        timer.restart();
        for (auto& x : as_data) x = Zufall::ui(0, 0xFF);
        as_tex->update(as_data.data());
    }
    window->draw(as_circ);
}
