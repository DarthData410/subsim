#pragma once

#include <vector>
#include "standard_ui.hpp"
#include "../sim/game/objekte/objekt.hpp"
#include "../sim/game/zone.hpp"
#include "../sim/game/team.hpp"

class Welt;
namespace sf { class RenderWindow; }

/// UI mit der Übersichtskarte und Bedienung zur Navigation.
class Nav_UI final : public Standard_UI {

    /// Netzwerk-Synchronisationsintervall in ms.
    static constexpr float SYNC_INTERVALL = 1000.f;

public:

    Nav_UI();

    explicit Nav_UI(Klient* klient);

    void update_and_show(const Sub* sub) override;

    void draw_gfx(const Sub* sub, sf::RenderWindow* window);

    /// Fog of War an/aus.
    void set_fow(bool fow) { Nav_UI::fow = fow; }

private:

    void sync(bool force = false);

    void handle_imgui_events();

    void show_minimap(const Sub* sub) const;

    void show_navigation(const Sub* sub) const;

    /// Zeichnet 1 Graph für sub's Signaturkurve. Optional: mark_v Markierung setzen bei gegebener Geschwindigkeit.
    void show_noise_signature(const Sub* sub, std::optional<float> mark_v = std::nullopt) const;

    /// SimDaten
    bool fow = false;
    std::vector<std::unique_ptr<Objekt>> objekte;
    std::vector<Zone> zonen;
    std::unordered_map<uint8_t, Team> teams;

    /// Karteneinstellungen
    mutable float scale = 0.1f;
    mutable float shift_x = 0;
    mutable float shift_y = 0;

};
