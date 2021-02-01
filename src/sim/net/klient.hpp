#pragma once

#include "kommando.hpp"
#include "../game/team.hpp."
#include "../game/objects/zone.hpp"

#include <enet/enet.h>
#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>

class Klient final {

public:

    explicit Klient(const std::string& ip);

    ~Klient();

    /// Getter: Zeitraffer (siehe `class Welt`). @note Setzt einen ``std::mutex`` ein.
    float get_timelapse();

    /// Getter: Teams. @note Setzt einen ``std::mutex`` ein.
    std::unordered_map<uint8_t, Team> get_teams();

    /// Getter: Zonen. @note Setzt einen ``std::mutex`` ein.
    std::vector<Zone> get_zonen();

    /// Stellt zu einem Server über die eingestellten IP eine Verbindung her. `true` bei Erfolg.
    bool connect();

    /// Sendet gegebenes Kommando an den Server.
    void kommando(const Kommando& cmd);

    /// Fragt Daten vom Server an. @note Vgl. `Net::Request` und `host.cpp` wegen Serialisierung.
    std::string request(Net::Request request_typ, std::optional<Net::id_t> objekt_id = std::nullopt);

private:

    /// Sagt dem Server, dass dieser Klient noch läuft und empfängt ggf. Broadcasts von Kanal 1.
    void keep_alive();

    /// Sendet gegebene Daten an den Server über Kanal 0.
    void sende(const std::string& paket_daten);

    /// Sendet gegebene Daten an den Server über Kanal 0 und wartet auf eine Antwort, dessen Inhalt zurückgegeben wird.
    std::string sende_und_empfange(const std::string& paket_daten);

    /// Thread-Daten
    std::mutex connection_mutex;
    std::atomic<bool> alive = true;
    std::thread flush_thread;

    /// Verbindungsdaten
    std::string host_ip;
    ENetHost* klient = nullptr;
    ENetPeer* server = nullptr;

    /// Simlationsdaten
    float timelapse;
    std::unordered_map<uint8_t, Team> teams;
    std::vector<Zone> zonen;

};
