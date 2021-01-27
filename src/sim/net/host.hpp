#pragma once

#include "../welt.hpp"

#include <enet/enet.h>
#include <mutex>

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

    // Impl
    std::mutex welt_mutex;

    // Net
    ENetHost* server;

};
