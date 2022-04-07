#include "ping.hpp"

Ping::Ping(const Objekt* quelle, float dauer) :
    restzeit(dauer)
{
    //
}

bool Ping::tick(Welt* welt, float s) {
    restzeit -= s;
    return restzeit >= 0.0;
}
