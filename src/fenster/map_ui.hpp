#pragma once

#include "standard_ui.hpp"
#include "../sim/game/zone.hpp"
#include "../sim/game/team.hpp"

class Map_UI final : public Standard_UI {

public:

    Map_UI() : Standard_UI(nullptr) {}

    explicit Map_UI(Klient* klient);

    void update_and_show(const Sub* sub) override;

    void handle(sf::Event* event) override;

    void draw_gfx(const Sub* sub, sf::RenderWindow* window) override;

    /// Fog of War an/aus.
    void set_fow(bool fow) { Map_UI::fow = fow; }

private:

    void sync(bool force = false);

    void handle_imgui_events();

    void show_minimap(const Sub* sub) const;

    /// Netzwerk-Synchronisationsintervall in ms.
    static constexpr float SYNC_INTERVALL = 1000.f;

    /// SimDaten
    bool fow = false;
    std::vector<std::unique_ptr<Objekt>> objekte;
    std::vector<Zone> zonen;
    std::unordered_map<uint8_t, Team> teams;
    float height_at_mouse = 0;

    /// Karteneinstellungen
    mutable float scale = 0.1f;
    mutable float shift_x = 0;
    mutable float shift_y = 0;

};
