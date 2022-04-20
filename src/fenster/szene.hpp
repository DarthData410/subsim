#pragma once

#include "spielszene.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

class Szene final {

public:

    Szene();

    void show();

    ~Szene();

private:

    void load_fonts();
    void load_theme();

    sf::RenderWindow window;

};
