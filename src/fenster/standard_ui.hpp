#pragma once

/// Prädeklarationen.
class Sub;
class Klient;

/// Mutterklasse für Standard-UI-Fenster.
class Standard_UI {

public:

    Standard_UI() = delete;

    /// Ctor. Initialisiert den Klienten für Netzwerkzugriffe.
    explicit Standard_UI(Klient* klient) : klient(klient) {};

    /// Rendert die UI. In jedem Renderschritt aufzurufen.
    virtual void update_and_show(const Sub* sub) = 0;

protected:

    Klient* klient;

};
