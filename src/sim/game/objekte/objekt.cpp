#include "objekt.hpp"
#include "torpedo.hpp"
#include "sub_ai.hpp"
#include "ping.hpp"
#include "decoy.hpp"
#include <nada/random.hpp>

Objekt::Objekt(const Vektor& pos, const float& bearing) :
        pos(pos),
        kurs(bearing),
        pitch(0)
{
    regenerate_id();
}

void Objekt::regenerate_id() {
    id = nada::random::random_hash<decltype(id)>();
}

Objekt* Objekt::copy(const Objekt* o) {
    if (o == nullptr) return nullptr;
    switch (o->get_typ()) {
        // Sortiert nach Häufigkeit des Vorkommens
        case Typ::PING:             return new Ping(*((Ping*)o));
        case Typ::SUB_AI:           return new Sub_AI(*((Sub_AI*)o));
        case Typ::SUB:              return new Sub(*((Sub*)o));
        case Typ::DECOY:            return new Decoy(*((Decoy*)o));
        case Typ::TORPEDO:          return new Torpedo(*((Torpedo*)o));
        case Typ::EXPLOSION:        return new Explosion(*((Explosion*)o));
        case Typ::OBJEKT:           return nullptr;
        case Typ::OBJEKT_STEUERBAR: return nullptr;
    }
    nada::Log::err() << "Objekt::copy Typ ist nicht kopierbar: " << (int)o->get_typ() << '\n';
    return nullptr;
}

const std::string& Objekt::get_name() const {
    static const std::string s("Invalid Object [A]");
    return s;
}
