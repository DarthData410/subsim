#pragma once

#include "objekt_steuerbar.hpp"
#include "sonar_passiv.hpp"

class Sub : public Objekt_Steuerbar {

    friend class Team;

public:

    Sub() = default;

    Sub(const Ogre::Vector3& pos,
        const Motor& motor_linear, const Motor& motor_rot, const Motor& motor_tauch);

    void tick(Welt* welt, float s) override;

    Typ get_typ() const override { return Typ::SUB; }

    float get_noise() const override;

    const std::vector<Sonar_Passiv>& get_sonars() const { return sonars; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Objekt_Steuerbar>(this),
           sonars, tarnung);
    }

protected:

    /// Passive Sonars an Board.
    std::vector<Sonar_Passiv> sonars;

    /// 1.0 = keine Tarnung
    float tarnung;

};
