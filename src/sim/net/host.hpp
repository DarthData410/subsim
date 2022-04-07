#pragma once

#include "../welt.hpp"
#include "net.hpp"

#include <enet/enet.h>

/// UDP Server für Multiplayer.
class Host final {

public:

    /// Initialisiert (aber startet nicht) den Host am gg. UDP Port.
    explicit Host(uint16_t port = Net::PORT);

    /// Startet den Server.
    void start();

    /// Stoppt den Server, gibt den Port aber noch nicht frei.
    void stop();

    /// Dtor. Gibt den Port wieder frei.
    ~Host();

private:

    /// Verwaltet den Fall, wenn ein `ENET_EVENT_TYPE_RECEIVE` auftritt.
    void handle_receive(ENetEvent& event);

    /// Sendet eine Antwort an den Peer aus `event`, wiederverwendet dabei `event->packet`.
    void sende_antwort(ENetEvent& event, const std::string& response);

    /// ENet-Serverobjekt.
    ENetHost* server = nullptr;

    /// Simlations-Loop + Netzwerk-Listening aktiv?
    bool loop = true;

    /// Simulierte Welt.
    Welt welt;

};
