#pragma once

#include "kommando.hpp"

#include <enet/enet.h>
#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>

class Klient final {

public:

    explicit Klient(const std::string& ip);

    ~Klient();

    bool connect();

    void test();

    void sende_kommando(const Kommando& cmd);

private:

    void keep_alive();

    void sende(const std::string& paket);

    std::mutex connection_mutex;
    std::atomic<bool> alive = true;
    std::thread flush_thread;

    std::string host_ip;
    ENetHost* klient = nullptr;
    ENetPeer* server = nullptr;

};
