#include "sonar_ui.hpp"
#include "../sim/game/objekte/sub.hpp"
#include "imgui_addons.hpp"

#include <zufall.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace {
    const unsigned RGB_PX = 4; // RGBA
    const unsigned PS_SIZE_Y = 800;
    const unsigned PS_SIZE_X = 500;
    const unsigned PS_LINE_WIDTH = PS_SIZE_X * RGB_PX;
}

Sonar_UI::Sonar_UI() : Standard_UI(nullptr),
    ps_data(PS_SIZE_X * PS_SIZE_Y * RGB_PX, 0x40), // TODO set to 0 (?)
    ps_tex(new sf::Texture())
{
    ps_tex->create(PS_SIZE_X, PS_SIZE_Y);
    ps_tex->update(ps_data.data());
}

Sonar_UI::Sonar_UI(const Sub* sub) : Sonar_UI() {

}

void Sonar_UI::update_and_show(const Sub* sub) {
    // ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize ImGuiWindowFlags_AlwaysAutoResize
    const auto flags = 0; // ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground;
    ImGui::Begin("Sonar Config", nullptr, flags);
    ImGui::Button("Blub");
    static float ps_intervall = 1.0f;
    ImGui::SliderFloat("Intervall", &ps_intervall, 0.2f, 10.f, "%.1f");
    ImGui::End();
}

void Sonar_UI::draw_gfx(const Sub* sub, sf::RenderWindow* window) {
    draw_ps(sub, window);
}

void Sonar_UI::draw_ps(const Sub* sub, sf::RenderWindow* window) {
    static sf::RectangleShape r;
    r.setSize({static_cast<float>(ps_tex->getSize().x), static_cast<float>(ps_tex->getSize().y)});
    r.setPosition(50, 50);
    r.setTexture(ps_tex.get(), true);
    window->draw(r);

    /// 1 Line Update
    if (static sf::Clock timer; timer.getElapsedTime().asSeconds() > 0.5f) {
        timer.restart();
        const auto& sonar = sub->get_sonars_passive().at(0);

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
        // Pixel 1 Reihe nach oben schieben
        // Unterste Reihe: Aktuelle Detektionen zeichnen
        // textur .update
    // Aktuelles ps_pic anzeigen
}
