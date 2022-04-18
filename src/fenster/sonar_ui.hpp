#pragma once

#include "standard_ui.hpp"
#include <vector>
#include <cstdint>
#include <memory>
#include <SFML/Graphics/RenderTexture.hpp>

/// UI zur Anzeige vom Passiven Sonar.
class Sonar_UI final : public Standard_UI {

public:

    Sonar_UI();

    explicit Sonar_UI(Klient* klient);

    void update_and_show(const Sub* sub) override;

    void draw_gfx(const Sub* sub, sf::RenderWindow* window) override;

private:

    void draw_as(const Sub*, sf::RenderWindow* window);
    void draw_ps(const Sub*, sf::RenderWindow* window);

private:

    std::unique_ptr<sf::RenderTexture> as_tex;
    std::unique_ptr<sf::Texture> ps_tex;
    std::vector<uint8_t> ps_data;

    /* Einstellungen */
    float as_intervall = 15.f;
    int   as_array_select = 1;
    float as_scale = 0.1f;
    float ps_intervall = 0.5f;
    int   ps_array_select = 1;

};
