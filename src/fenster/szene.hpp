#pragma once

#include "spielszene.hpp"

class Szene {

public:

    Szene();

    void show();

    ~Szene();

private:

    Spielszene* spielszene = nullptr;

};
