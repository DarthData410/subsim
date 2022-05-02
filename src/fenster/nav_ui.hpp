#pragma once

#include <vector>
#include "standard_ui.hpp"
#include "../sim/game/objekte/objekt.hpp"
#include "../sim/game/zone.hpp"
#include "../sim/game/team.hpp"

namespace gfx {
    class Grafik;
    class Multigrafik;
}

/// UI mit der Übersichtskarte und Bedienung zur Navigation.
class Nav_UI final : public Standard_UI {

public:

    Nav_UI();

    explicit Nav_UI(Klient* klient);

    virtual ~Nav_UI();

    void update_and_show(const Sub* sub) override;

    void draw_gfx(const Sub* sub, sf::RenderWindow* window) override;

    void handle(sf::Event* event) override;

    void handle_imgui_events(const Sub* sub);

    void sync(const Sub* sub);

private:

    void show_navigation(const Sub* sub) const;

    /// Zeichnet 1 Graph für sub's Signaturkurve. Optional: mark_v Markierung setzen bei gegebener Geschwindigkeit.
    void show_noise_signature(const Sub* sub, std::optional<float> mark_v = std::nullopt) const;

    /// Notauftauchen aktiv?
    bool emergency_surface = false;

    /* Grafiken */
    gfx::Grafik*      bg                            = nullptr;
    gfx::Multigrafik* schalter_vent_air             = nullptr;
    gfx::Multigrafik* schalter_emergency_surface    = nullptr;
    gfx::Multigrafik* lampe_air_available           = nullptr;
    gfx::Multigrafik* lampe_emergency_surface_aktiv = nullptr;

};
