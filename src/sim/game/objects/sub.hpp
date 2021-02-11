#pragma once

#include "objekt_steuerbar.hpp"
#include "sonar_passiv.hpp"
#include "torpedo.hpp"

#include <cereal/types/map.hpp>

class Sub : public Objekt_Steuerbar {

    friend class Team;

public:

    Sub() = default;

    Sub(const Ogre::Vector3& pos,
        const Motor& motor_linear, const Motor& motor_rot, const Motor& motor_tauch);

    bool tick(Welt* welt, float s) override;

    Typ get_typ() const override { return Typ::SUB; }

    float get_noise() const override;

    const std::vector<Sonar_Passiv>& get_sonars() const { return sonars; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Objekt_Steuerbar>(this),
           sonars, tarnung, torpedos
        );
    }

protected:

    /// Passive Sonars an Board.
    std::vector<Sonar_Passiv> sonars;

    /// Torpedos und deren Anzahl.
    std::map<Torpedo, uint8_t> torpedos;

    /// Faktor, mit dem `get_noise` multipliziert wird. Das heißt 1.0 = keine Tarnung, 0.0 = unsichtbar.
    float tarnung;

};
