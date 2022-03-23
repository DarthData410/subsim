#pragma once

#include "spielszene.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

class Szene {

public:

    Szene();

    void show();

    ~Szene();

private:

    sf::RenderWindow window;

};
