#include "host.hpp"
#include "../welt.hpp"

Host::Host(uint16_t port) : server(port) {

}

void Host::start() {
    server.run();
}

void Host::stop() {
    server.stop();
}
