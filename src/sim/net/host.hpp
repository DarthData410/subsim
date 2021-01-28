#pragma once

#include "../welt.hpp"

#include <enet/enet.h>

class Welt;

class Host final {

public:

    explicit Host(uint16_t port);

    void start();

    void stop();

    ~Host();

private:

    // Model
    bool loop = true;

    Welt welt;

    // Net
    ENetHost* server = nullptr;

};
