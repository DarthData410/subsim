#pragma once

#include "standard_ui.hpp"
#include <vector>
#include <cstdint>
#include <memory>
#include <optional>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Clock.hpp>

/// UI zur Anzeige vom Passiven Sonar.
class Sonar_UI final : public Standard_UI {

public:

    /// Ctor. @note Erzeugt keine verwendbare UI.
    Sonar_UI();

    /// Benutzbare Sonar_UI muss hierüber erstellt werden.
    explicit Sonar_UI(Klient* klient);

    /// Veranlasst alle UI und SFML Renderings.
    void update_and_show(const Sub* sub) override;

    /// Veranlasst alle SFML-Renderings.
    void draw_gfx(const Sub* sub, sf::RenderWindow* window) override;

    /// Unbenutzt. Kann für SFML Events genutzt werden.
    void handle(sf::Event* event) override { (void)event; }

private:

    /// Aktiven Sonar zeichnen.
    void draw_as(const Sub*, sf::RenderWindow* window);

    /// Aktiven Sonar zeichnen.
    void draw_ps(const Sub*, sf::RenderWindow* window);

private:

    std::unique_ptr<sf::RenderTexture> as_tex;
    std::unique_ptr<sf::Texture> ps_tex;
    std::vector<uint8_t> ps_data;
    std::optional<sf::Clock> as_last_ping_timer;
    uint32_t as_last_ping = 0;

    /* Einstellungen */
    int   as_array_select = 1;
    float as_scale = 0.1f;
    bool  as_range_rings = true;
    int   ps_array_select = 1;
    float ps_intervall = 0.5f;

};
