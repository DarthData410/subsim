#pragma once

#include "standard_ui.hpp"
#include <vector>
#include <cstdint>

/// UI zur Anzeige vom Passiven Sonar.
class Sonar_UI final : public Standard_UI {

public:

    Sonar_UI();

    explicit Sonar_UI(const Sub* sub);

    void update_and_show(const Sub* sub) override;

    void draw_gfx(const Sub* sub, sf::RenderWindow* window) override;

private:

    void draw_ps(const Sub*, sf::RenderWindow* window);

private:

    std::vector<uint8_t> ps_pic;

};
