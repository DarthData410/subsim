#pragma once

#include "standard_ui.hpp"

class Waffen_UI final : public Standard_UI {

public:

    Waffen_UI();

    explicit Waffen_UI(Klient* klient);

    void update_and_show(const Sub* sub) override;

    void handle(sf::Event* event) override { (void)event; }

    void draw_gfx(const Sub* sub, sf::RenderWindow* window) override {
        (void) sub;
        (void) window;
    }

private:

    void show_intelligence() const;

    void show_weapons(const Sub* sub) const;

};