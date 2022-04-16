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

    virtual void handle(sf::Event* event) {};

    virtual void draw_gfx(const Sub* sub, sf::RenderWindow* window) {};

protected:

    /// Für das senden von Kommandos an den Server.
    Klient* klient;

};
