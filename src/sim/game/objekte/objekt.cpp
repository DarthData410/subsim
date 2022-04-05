#include "objekt.hpp"
#include <zufall.hpp>

Objekt::Objekt(const Vektor& pos, const float& bearing) :
        pos(pos),
        kurs(bearing),
        pitch(0)
{
    regenerate_id();
}

void Objekt::regenerate_id() {
    id = Zufall::random_hash<decltype(id)>();
}

float Objekt::get_bearing() const {
    return kurs;
}

float Objekt::get_pitch() const {
    return pitch;
}
