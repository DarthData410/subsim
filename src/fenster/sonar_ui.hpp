#pragma once

#include "standard_ui.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include <cstdint>
#include <memory>

namespace sf { class Texture; }

/// UI zur Anzeige vom Passiven Sonar.
class Sonar_UI final : public Standard_UI {

public:

    /// Haupt-Ctor.
    Sonar_UI();

    explicit Sonar_UI(const Sub* sub);

    void update_and_show(const Sub* sub) override;

    void draw_gfx(const Sub* sub, sf::RenderWindow* window) override;

private:

    void draw_ps(const Sub*, sf::RenderWindow* window);

private:

    std::unique_ptr<sf::Texture> ps_tex;
    std::vector<uint8_t> ps_data;

    /* Einstellungen */
    float ps_intervall = 0.5f;
    int ps_array_select = 1;

};
