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

    void kommando(const Kommando& cmd);

    std::string request(Net::Request request_typ, std::optional<Net::id_t> objekt_id = std::nullopt);

private:

    void keep_alive();

    void sende(const std::string& paket_daten);

    std::string sende_und_empfange(const std::string& paket_daten);

    std::mutex connection_mutex;
    std::atomic<bool> alive = true;
    std::thread flush_thread;

    std::string host_ip;
    ENetHost* klient = nullptr;
    ENetPeer* server = nullptr;

};
