#pragma once

#include "objekt.hpp"

#include <cereal/types/base_class.hpp>

class Explosion final : public Objekt {

public:

    Explosion() = default;

    Typ get_typ() const override { return Objekt::Typ::EXPLOSION; }

    /// Explosionen fügen.
    bool tick(Welt* welt, float s) override;

    float get_noise() const override { return 1.0; };

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Objekt>(this),
           source, radius, power, remaining_time, damage_done
        );
    }

private:

    /// Quelle (Sub-Objekt) der Explosion.
    oid_t source;

    /// Explosionsradius
    dist_t radius;

    /// Stärke (zur Zufügung von Schaden).
    float power;

    /// Wie lange die Explosion "lebt" in Sekunden.
    float remaining_time;

    /// Wurde angrenzenden Objekten Schaden bereits zugefügt durch diese explosion?
    bool damage_done;

};
CEREAL_REGISTER_TYPE(Explosion)
