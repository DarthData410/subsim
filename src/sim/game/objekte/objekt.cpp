#include "objekt.hpp"
#include <zufall.hpp>
#include "torpedo.hpp"
#include "sub_ai.hpp"

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

Objekt* Objekt::copy(const Objekt* o) {
    Objekt* kopie = nullptr;
    switch (o->get_typ()) {
        case Typ::OBJEKT: Log::err() << "Objekt::copy Typ::OBJEKT ist nicht kopierbar\n"; break;
        case Typ::SUB:              kopie = new Sub(*((Sub*)o)); break;
        case Typ::SUB_AI:           kopie = new Sub_AI(*((Sub_AI*)o)); break;
        case Typ::TORPEDO:          kopie = new Torpedo(*((Torpedo*)o)); break;
        case Typ::EXPLOSION:        kopie = new Explosion(*((Explosion*)o)); break;
        case Typ::OBJEKT_STEUERBAR: kopie = new Objekt_Steuerbar(*((Objekt_Steuerbar*)o)); break;
    }
    return kopie;
}
