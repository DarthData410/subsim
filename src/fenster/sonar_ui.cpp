#include "sonar_ui.hpp"
#include "../sim/game/objekte/sub.hpp"
#include "imgui_addons.hpp"

#include <zufall.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

Sonar_UI::Sonar_UI() : Standard_UI(nullptr) {
    //
}

Sonar_UI::Sonar_UI(const Sub* sub) : Standard_UI(nullptr) {

}

void Sonar_UI::update_and_show(const Sub* sub) {
    ImGui::Begin("Sonar Config");

    ImGui::End();
}

void Sonar_UI::draw_gfx(const Sub* sub, sf::RenderWindow* window) {
    draw_ps(sub, window);
}

void Sonar_UI::draw_ps(const Sub* sub, sf::RenderWindow* window) {
    // Einstellungen
    constexpr unsigned PS_SIZE_X = 255;
    constexpr unsigned PS_SIZE_Y = 511;

    //std::unique_ptr<sf::Texture> - ps_pic
    // .create(w,h) .update(uint8* RGBA) loadFromMemory (const void * data, size_t size)
    //if sf timer
        // Pixel 1 Reihe nach oben schieben
        // Unterste Reihe: Aktuelle Detektionen zeichnen
        // textur .update
    // Aktuelles ps_pic anzeigen
}
