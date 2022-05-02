#pragma once

/// Prädeklarationen.
class Sub;
class Klient;
namespace sf {
    class Event;
    class RenderWindow;
}

/// Mutterklasse für Standard-UI-Fenster.
class Standard_UI {

public:

    Standard_UI() = delete;

    /// Ctor. Initialisiert den Klienten für Netzwerkzugriffe.
    explicit Standard_UI(Klient* klient) : klient(klient) {};

    /// Rendert die UI. In jedem Renderschritt aufzurufen.
    virtual void update_and_show(const Sub* sub) = 0;

    /// SFML Eingabe-Event verwenden.
    virtual void handle(sf::Event* event) = 0;

    /// An diese Stelle kommen SFML-Renderings.
    virtual void draw_gfx(const Sub* sub, sf::RenderWindow* window) = 0;

protected:

    /// Für das senden von Kommandos an den Server.
    Klient* klient;

};
