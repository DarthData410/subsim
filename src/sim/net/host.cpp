#include "host.hpp"
#include "net.hpp"
#include "kommando.hpp"

#include <log.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/unordered_map.hpp>
#include <SFML/System/Clock.hpp>

Host::Host(uint16_t port) {
    ENetAddress address {
        .host = ENET_HOST_ANY,
        .port = port
    };
    if (server = enet_host_create(&address, 32, // Max. Klienten
                                  2, // Kanäle; 0 = Request/Receive, 1 = Broadcast
                                  0, 0); !server) {
        Log::err() << "An error occurred while trying to create an ENet server host.\n";
        exit(EXIT_FAILURE);
    }
}

void Host::start() {
    while (loop) {
        welt.tick(); // TODO benchmark ms

        //Log::out() << "Server eventloop" << Log::endl;
        ENetEvent event;
        while(enet_host_service(server, &event, 50) > 0) switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                printf("A new client connected from %x:%u.\n",
                       event.peer->address.host,
                       event.peer->address.port);
                //event.peer->data = "Client information";
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                handle_receive(event);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                printf("%s disconnected.\n", event.peer->data);
                event.peer->data = nullptr; // delete wenn nötig
                break;
        }
        // Broadcast
        if (static sf::Clock timer; timer.getElapsedTime().asMilliseconds() > 1000) {
            std::stringstream ss;
            Net::Serializer s(ss);
            s << Net::BROADCAST;
            s << welt.timelapse << welt.teams << welt.zonen;
            const std::string& data = ss.str();
            ENetPacket* paket = enet_packet_create(data.c_str(), data.size(),
                                                   ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
            enet_host_broadcast(server, 1, paket);
            enet_host_flush(server);
            timer.restart();
        }
    }
}

void Host::stop() {
    loop = false;
}

Host::~Host() {
    stop();
    enet_host_destroy(server);
}

void Host::handle_receive(ENetEvent& event) {
    Net::Request request;
    std::string data((const char*) event.packet->data, event.packet->dataLength);
    //Log::debug() << "Host::handle_receive paket_daten = " << data.size() << Log::endl;
    std::stringstream ss(data);
    Net::Deserializer ds(ss);
    ds >> request;

    switch (request) {
        case Net::SUB_CMD: {
            Kommando kommando;
            ds >> kommando;
            kommando.apply(&welt);
            enet_packet_destroy(event.packet); // Aufräumen
        } break;
        case Net::AKTION_NEUES_UBOOT: {
            Net::id_t team;
            ds >> team;
            sende_antwort(event, Net::serialize(*welt.add_new_sub(team, false)));
        } break;
        case Net::REQUEST_SUB: {
            Net::id_t sub_id;
            ds >> sub_id;
            if (welt.objekte.count(sub_id)) sende_antwort(event, Net::serialize(*(Sub*) welt.objekte[sub_id]));
            else sende_antwort(event, "");
        } break;
        case Net::ALLE_OBJEKTE: {
            std::vector<Objekt> objekte;
            objekte.reserve(welt.objekte.size());
            for (const auto& paar : welt.objekte) objekte.push_back(*paar.second);
            sende_antwort(event, Net::serialize(objekte));
        } break;
        default: Log::err() << "\tUnknown Request Net::" << request << '\n'; break;
    }
}

void Host::sende_antwort(ENetEvent& event, const std::string& response) {
    //Log::debug() << "Host::sende_antwort paket_daten=" << response.size() << Log::endl;
    ENetPacket* paket = event.packet;
    enet_packet_resize(paket, response.size());
    std::copy(response.begin(), response.end(), paket->data);
    enet_peer_send(event.peer, 0, paket);
    enet_host_flush(server);
}
