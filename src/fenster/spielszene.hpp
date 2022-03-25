#pragma once

#include "../sim/game/objects/sub.hpp"
#include "../sim/net/klient.hpp"
#include "nav_ui.hpp"
#include "sonar_ui.hpp"
#include "waffen_ui.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Spielszene final {

public:

    /// Root-Ctor, wird von anderen Ctors aufgerufen zur Verbindungserstellung.
    explicit Spielszene(sf::RenderWindow* window, const std::string& ip = "127.0.0.1");

    void show();

private:

    void key_pressed(const sf::Keyboard::Key& key);

    void sync();

    /// Zeigt das Hauptmenü.
    void draw_menu();

    void draw_imgui();

    void draw_gfx();

private:

    enum Tab : uint8_t {
        MAINMENU,   // TODO
        NAV,
        SONAR,      // TODO
        WEAPONS,    // TODO
        THREE_D
    };

private:

    /// Anzuzeigendes Fenster.
    Tab tab = NAV;

    /// Netzwerkklient
    std::unique_ptr<Klient> klient;

    /// Simulation
    std::optional<Sub> player_sub = std::nullopt;
    Nav_UI nav_ui;
    Sonar_UI sonar_ui;
    Waffen_UI waffen_ui;
    sf::RenderWindow* window;

};
