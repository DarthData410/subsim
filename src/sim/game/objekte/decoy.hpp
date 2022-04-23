#pragma once

#include "objekt_steuerbar.hpp"

class Sub;

/// Anti-Torpedo-Gegenmaßnahme.
class Decoy final : public Objekt_Steuerbar {

public:

    /// Nur für Serialisierung.
    Decoy() = default;

    /**
     * Ctor zur Erstellung eines neuen Decoy-Typen (= Vorlage).
     * @note Die v der 'Motoren' geben das feste Bewegungsmuster des Decoys an (falls dieses nicht konfigurierbar ist).
     */
    Decoy(const Motor& motor_linear, const Motor& motor_rot, const Motor& motor_tauch,
            const std::string& name, float noise, float lebenszeit_in_s);

    /// Ctor für Decoys, die von ihrem Sub ausgestoßen werden.
    Decoy(const Decoy& decoy_typ, const Sub* sub);

    Typ get_typ() const override { return Objekt::Typ::DECOY; }

    bool tick(Welt* welt, float s) override;

    /// Unzerstörbar.
    bool apply_damage(const Explosion* explosion, float damage) override { (void)explosion; (void)damage; return false; }

    /// Typenbezeichnung dieser Gegenmaßnahme.
    const std::string& get_name() const override { return name; }

    /// Lautstärke [0.0,1.0].
    float get_noise() const override { return noise; }

    /// Zur Eintragung in `unordered_map` z.B. Muss einmaligen Namen haben.
    friend bool operator<(const Decoy& lhs, const Decoy& rhs);

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Objekt_Steuerbar>(this),
           name, restzeit
        );
    }

private:

    /// Name des U-Bootmodells.
    std::string name;

    /// Lebenszeit. Zählt bei tick() runter.
    float restzeit;

};
CEREAL_REGISTER_TYPE(Decoy)
