#pragma once

#include "objekt.hpp"

/// Kurzlebige, lokalisierte Geräuschquelle
class Ping final : public Objekt {

    friend class Test_Objekte;

public:

    Ping() = default;

    /// Ctor. Reichweite in m, Dauer in s.
    Ping(const Objekt* quelle, dist_t range, float dauer = 5);

    bool tick(Welt* welt, float s) override;

    bool apply_damage(Explosion* explosion, float damage) override { (void)explosion; (void)damage; return false; }

    Typ get_typ() const override { return Typ::PING; }

    float get_noise() const override { return 1.0f; }

    /// Getter: Objektname.
    const std::string& get_name() const override;

    dist_t get_range() const { return range; }

    oid_t get_quelle() const { return quelle; }

    float get_noise_relative(dist_t dist) const;

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Objekt>(this),
            restzeit, range, quelle
        );
    }

private:

    float restzeit;

    /// Reichweite, in der der Ping wahrnehmbar ist.
    dist_t range;

    oid_t quelle;

};
CEREAL_REGISTER_TYPE(Ping)
