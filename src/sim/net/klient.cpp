#include "klient.hpp"
#include "net.hpp"

#include <log.hpp>
#include <thread>

Klient::Klient(const std::string& ip) : host_ip(ip) {
    klient = enet_host_create(NULL, // 0 = Klient
                              1,  // Verbindungen
                              2,  // Kanäle
                              0,0 // Bandbreite
    );
    if (klient == NULL) {
        Log::err() << "Klient: An error occurred while trying to create an ENet client host.\n";
        return;
    }
    connect();
    flush_thread = std::thread(&Klient::keep_alive, this);
}

Klient::~Klient() {
    alive = false;
    flush_thread.join();
    if (klient) enet_host_destroy(klient);
}

bool Klient::connect() {
    Log::debug() << "Klient: Connecting " << host_ip << Log::endl;
    ENetAddress host_address;
    enet_address_set_host(&host_address, host_ip.c_str());
    host_address.port = Net::PORT;
    if (server = enet_host_connect(klient, &host_address, 2, 0); server == NULL) {
        Log::err() << "Klient: No available peers for initiating an ENet connection.\n";
        return false;
    }
    ENetEvent event;
    if (enet_host_service(klient, &event, 1000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        Log::out() << "Klient: Connection established to Host " << host_ip << Log::endl;
        return true;
    }
    enet_peer_reset(server);
    Log::err() << "Klient: Connection failed to Host " << host_ip << Log::endl;
    return false;
}

void Klient::keep_alive() {
    while (alive.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        ENetEvent event;
        std::scoped_lock lock(connection_mutex);
        enet_host_service(klient, &event, 1000);
    }
}

void Klient::sende(const std::string& paket_daten) {
    Log::debug() << "Klient::sende paket_daten=" << paket_daten.size() << Log::endl;
    ENetPacket* packet = enet_packet_create(paket_daten.c_str(), paket_daten.size(), ENET_PACKET_FLAG_RELIABLE);
    std::scoped_lock lock(connection_mutex);
    enet_peer_send(server, 0, packet);
    enet_host_flush(klient);
}

std::string Klient::sende_und_empfange(const std::string& paket_daten) {
    Log::debug() << "Klient::sende_und_empfange paket_daten >> " << paket_daten.size() << Log::endl;
    ENetPacket* packet = enet_packet_create(paket_daten.c_str(), paket_daten.size(), ENET_PACKET_FLAG_RELIABLE);
    std::scoped_lock lock(connection_mutex);
    enet_peer_send(server, 0, packet);
    while (true) {
        // Antwort abwarten
        ENetEvent event;
        enet_host_service(klient, &event, 50);
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE: {
                std::string antwort((const char*) event.packet->data, event.packet->dataLength);
                Log::debug() << "Klient::sende_und_empfange paket_daten << " << antwort.size() << Log::endl;
                enet_packet_destroy(event.packet);
                return antwort;
            }
            case ENET_EVENT_TYPE_DISCONNECT:
                Log::err() << "Klient: " << __func__ << " disconnected.\n";
                return "";
            default: break;
        }
    }
}

std::string Klient::request(Net::Request request_typ, std::optional<Net::id_t> objekt_id) {
    std::stringstream ss;
    Net::Serializer s(ss);
    s << request_typ;
    if (objekt_id) s << objekt_id.value();
    return sende_und_empfange(ss.str());
}

void Klient::kommando(const Kommando& cmd) {
    std::stringstream ss;
    Net::Serializer s(ss);
    s << Net::SUB_CMD;
    s << cmd;
    sende(ss.str());
}
