#pragma once

#include "objekt.hpp"

#include <cereal/types/base_class.hpp>

class Torpedo;

class Explosion final : public Objekt {

    friend class Torpedo;

public:

    Explosion() = default;

    explicit Explosion(const Torpedo* torpedo);

    Explosion(dist_t radius, float power, float remaining_time,
              const Vektor& pos = {0,0,0}, float bearing = 0, oid_t source = 0);

    Typ get_typ() const override { return Objekt::Typ::EXPLOSION; }

    /// Explosionen fügen.
    bool tick(Welt* welt, float s) override;

    float get_noise() const override { return 1.0; };

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Objekt>(this),
           quelle, radius, power, remaining_time, damage_done
        );
    }

private:

    /// Quelle (Sub-Objekt) der Explosion. Wird über Torpedo an die Explosion 'weitergereicht'.
    oid_t quelle;

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
