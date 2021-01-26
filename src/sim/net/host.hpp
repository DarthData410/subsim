#pragma once

#include <rpc/server.h>

class Welt;

class Host final {

public:

    explicit Host(uint16_t port);

    void start();

    void stop();

private:

    Welt* welt;

    rpc::server server;

};