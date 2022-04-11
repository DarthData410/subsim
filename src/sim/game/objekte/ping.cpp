#include "ping.hpp"

Ping::Ping(const Objekt* quelle, dist_t range, float dauer) :
    Objekt(quelle->get_pos()),
    range(range), restzeit(dauer), quelle(quelle->get_id())
{
    //
}

bool Ping::tick(Welt* welt, float s) {
    restzeit -= s;
    return restzeit >= 0.0;
}

float Ping::get_noise_relative(dist_t dist) const {
    if (dist == 0) return get_noise();
    return get_noise() - ((dist / range) * get_noise());
}
