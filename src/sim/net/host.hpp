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

    /// Verwaltet den Fall, wenn ein `ENET_EVENT_TYPE_RECEIVE` auftritt.
    void handle_receive(ENetEvent& event);

    /// Sendet eine Antwort an den Peer aus `event`, wiederverwendet dabei `event->packet`.
    void sende_antwort(ENetEvent& event, const std::string& response);

    /// Net
    ENetHost* server = nullptr;

    /// Model
    bool loop = true;

    /// Simulierte Welt.
    Welt welt;

};
