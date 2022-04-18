#include "sonar_ui.hpp"
#include "../sim/game/objekte/sub.hpp"
#include "imgui_addons.hpp"
#include "../gfx/grafik.hpp"

#include <zufall.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace {
    const unsigned RGB_PX = 4; // RGBA
    const uint8_t  BG_COL = 0x40;
    const unsigned PS_SIZE_X = 432;
    const unsigned PS_SIZE_Y = 660;
    const unsigned PS_POS_X  = 112;
    const unsigned PS_POS_Y  =  80;
    const unsigned PS_LINE_WIDTH = PS_SIZE_X * RGB_PX;
    sf::RectangleShape ps_rect;
    Grafik bg("data/gfx/bg_sonar.png");
}

Sonar_UI::Sonar_UI() : Standard_UI(nullptr),
    ps_data(PS_SIZE_X * PS_SIZE_Y * RGB_PX, BG_COL),
    ps_tex(new sf::Texture())
{
    // Setup Passive Sonar Texture + Render-Rect
    ps_tex->create(PS_SIZE_X, PS_SIZE_Y);
    ps_tex->update(ps_data.data());
    ps_rect.setSize({static_cast<float>(ps_tex->getSize().x), static_cast<float>(ps_tex->getSize().y)});
    ps_rect.setPosition(PS_POS_X, PS_POS_Y);
    ps_rect.setTexture(ps_tex.get(), true);

}

Sonar_UI::Sonar_UI(const Sub* sub) : Sonar_UI() {

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
        ps_data.assign(PS_SIZE_X * PS_SIZE_Y * RGB_PX, BG_COL);
        ps_tex->update(ps_data.data());
    }
    ImGui::End();

    // AS Select
    // AS Mode (off - single - on)
    // AS Single Ping Fire
}

void Sonar_UI::draw_gfx(const Sub* sub, sf::RenderWindow* window) {
    bg.draw(window);
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
