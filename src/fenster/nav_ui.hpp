#pragma once

#include <vector>
#include "standard_ui.hpp"
#include "../sim/game/objekte/objekt.hpp"
#include "../sim/game/zone.hpp"
#include "../sim/game/team.hpp"

/// UI mit der Übersichtskarte und Bedienung zur Navigation.
class Nav_UI final : public Standard_UI {

public:

    Nav_UI();

    explicit Nav_UI(Klient* klient);

    void update_and_show(const Sub* sub) override;

    void draw_gfx(const Sub* sub, sf::RenderWindow* window) override;

    void handle(sf::Event* event) override { (void)event; }

private:

    void show_navigation(const Sub* sub) const;

    /// Zeichnet 1 Graph für sub's Signaturkurve. Optional: mark_v Markierung setzen bei gegebener Geschwindigkeit.
    void show_noise_signature(const Sub* sub, std::optional<float> mark_v = std::nullopt) const;

};
