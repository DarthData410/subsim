#pragma once

#include <vector>
#include "standard_ui.hpp"
#include "../sim/game/objekte/objekt.hpp"
#include "../sim/game/zone.hpp"
#include "../sim/game/team.hpp"

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

    void sync(bool force = false);

    void show_minimap(const Sub* sub) const;

    void show_navigation(const Sub* sub) const;

    /// Zeichnet 1 Graph für sub's Signaturkurve. Optional: mark_v Markierung setzen bei gegebener Geschwindigkeit.
    void show_noise_signature(const Sub* sub, std::optional<float> mark_v = std::nullopt) const;

    bool fow = false;

    /// SimDaten
    std::vector<std::unique_ptr<Objekt>> objekte;
    std::vector<Zone> zonen;
    std::unordered_map<uint8_t, Team> teams;

};
