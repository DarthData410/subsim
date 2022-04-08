#pragma once

#include "objekt.hpp"

/// Kurzlebige, lokalisierte Geräuschquelle
class Ping final : public Objekt {

public:

    Ping() = default;

    /// TODO: quelle unbenutzt. Reichweite in m, Dauer in s.
    Ping(const Objekt*quelle, dist_t range, float dauer = 5);

    bool tick(Welt* welt, float s) override;

    Typ get_typ() const override { return Typ::PING; }

    float get_noise() const override { return 1.0f; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Objekt>(this),
            restzeit, range
        );
    }

private:

    float restzeit;

    /// Reichweite, in der der Ping wahrnehmbar ist.
    dist_t range;

};
CEREAL_REGISTER_TYPE(Ping)
