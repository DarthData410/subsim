#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <memory>

namespace sf { class RenderWindow; }
class Klient;
class Sub;
class Map_UI;
class Nav_UI;
class Sonar_UI;
class Waffen_UI;

/// Verwaltet das UI. Besitzt alle UIs als Instanzen.
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

    /// Anzuzeigender Tab.
    enum Tab {
        MAINMENU,   // TODO
        MAP,
        NAV,
        SONAR,
        WEAPONS,
        THREE_D
    };

private:

    /// Anzuzeigendes Fenster.
    Tab tab = NAV;

    /// Netzwerkklient
    std::unique_ptr<Klient> klient;

    /// Simulation
    std::unique_ptr<Sub> player_sub;

    /// UI
    sf::RenderWindow* window; // Diese Klasse hier ist nicht Besitzer.
    std::unique_ptr<Map_UI> map_ui;
    std::unique_ptr<Nav_UI> nav_ui;
    std::unique_ptr<Sonar_UI> sonar_ui;
    std::unique_ptr<Waffen_UI> waffen_ui;

};
