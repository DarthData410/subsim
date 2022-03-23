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

    void key_pressed(const sf::Keyboard::Key& key);

    void sync();

private:

    enum Tab : uint8_t {
        MAINMENU,   // TODO
        NAV,
        SONAR,      // TODO
        WEAPONS,    // TODO
        THREE_D
    };

    /// Zeigt das Hauptmenü.
    void render_menu();

    /// Zeigt die Navigation inkl. Strategie - (Teams, Zonen, Karte).
    void render_nav();

    /// Zeigt Aktives, Passives und Externes Sonar.
    void render_sonar();

    /// Zeigt Zielsteuerung & Waffenkontrolle.
    void render_weapons();

    /// Zeigt die 3D-Ansicht.
    void render_3d();

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
