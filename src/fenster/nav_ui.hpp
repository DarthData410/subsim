#pragma once

#include "standard_ui.hpp"

class Welt;

class Nav_UI final : public Standard_UI {

    /// Netzwerk-Synchronisationsintervall in ms.
    static constexpr float SYNC_INTERVALL = 1000.f;

public:

    Nav_UI();

    explicit Nav_UI(Klient* klient);

    void update_and_show(const Sub* sub) override;

    /// Fog of War an/aus.
    void set_fow(bool fow) { Nav_UI::fow = fow; }

    void draw_gfx();

private:

    void show_minimap(const Sub* sub) const;

    void show_navigation(const Sub* sub) const;

    bool fow = false;

};
