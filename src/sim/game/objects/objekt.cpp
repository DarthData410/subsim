#include "objekt.hpp"
#include <zufall.hpp>

Objekt::Objekt(const Vektor& pos, const float& bearing) :
               pos(pos),
               bearing(bearing),
               pitch(0)
{
    regenerate_id();
}

Objekt::~Objekt() {

}

void Objekt::regenerate_id() {
    id = Zufall::random_hash<decltype(id)>(); // TODO check if exists
}

float Objekt::get_bearing() const {
    return bearing;
}

float Objekt::get_pitch() const {
    return pitch;
}
