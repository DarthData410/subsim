#pragma once

/// Prädeklarationen.
class Sub;

/// Mutterklasse für Standard-UI-Fenster.
class Standard_UI {

public:

    /// Rendert die UI. In jedem Renderschritt aufzurufen.
    virtual void update_and_show(const Sub* sub) = 0;

};
