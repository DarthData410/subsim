#pragma once

#include "objekt.hpp"

/// Kurzlebige, lokalisierte Geräuschquelle
class Ping final : public Objekt {

public:

    Ping() = default;

    Ping(const Objekt*quelle, float dauer);

    bool tick(Welt* welt, float s) override;

    Typ get_typ() const override { return Typ::PING; }

    float get_noise() const override { return 1.0f; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Objekt>(this),
            restzeit
        );
    }

private:

    float restzeit;

};
CEREAL_REGISTER_TYPE(Ping)
