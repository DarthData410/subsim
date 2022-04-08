#include "ping.hpp"

Ping::Ping(const Objekt* quelle, dist_t range, float dauer) :
    Objekt(quelle->get_pos()),
    range(range), restzeit(dauer)
{
    //
}

bool Ping::tick(Welt* welt, float s) {
    restzeit -= s;
    return restzeit >= 0.0;
}
