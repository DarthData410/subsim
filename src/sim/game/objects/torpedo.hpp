#pragma once

#include "objekt_steuerbar.hpp"

#include <cereal/types/string.hpp>

class Sub;

class Torpedo final : public Objekt_Steuerbar {

public:

    /// Ctor. Muss zum Serialisieren existieren.
    Torpedo() = default;

    /// Ctor zur Erzeugung aus einem Torpedotypen heraus, der von einem Sub verschossen wird.
    Torpedo(const Torpedo& torpedo_typ, const Sub* sub, float distance_to_activate, float target_bearing, float target_depth);

    /// Ctor zur Erstellung eines neuen Torpedotypen (= Vorlage).
    Torpedo(const Motor& motor_linear, const Motor& motor_rot, const Motor& motor_tauch, const std::string& name, float range);

    Objekt::Typ get_typ() const override final { return Typ::TORPEDO; }

    bool tick(Welt* welt, float s) override final;

    const std::string& get_name() const { return name; }

    float get_range() const { return range; }

    /// Zur Eintragung in `unordered_map` z.B. Muss einmaligen Namen haben.
    friend bool operator<(const Torpedo& lhs, const Torpedo& rhs);

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Objekt_Steuerbar>(this),
                name, range, travelled, distance_to_activate, target_bearing, target_depth
        );
    }

private:

    /// Typname. Muss Welt-weit einmalig sein.
    std::string name;

    /// Reichweite.
    float range;

    /// Wie weit das Torpedo bereits gereist ist.
    float travelled = 0.f;

    /// In welcher Entfernung soll der Sucher aktiv werden?
    float distance_to_activate;

    /// In welche Richtung soll das Torpedo initial steuern?
    float target_bearing;

    /// In welcher Tiefe soll das Torpedo unterwegs sein?
    float target_depth;

};
