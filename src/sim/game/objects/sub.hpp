#pragma once

#include "objekt_steuerbar.hpp"

class Sub final : public Objekt_Steuerbar {

public:

    Sub() = default;

    Sub(const Ogre::Vector3& pos,
        const Motor& motor_linear, const Motor& motor_rot, const Motor& motor_tauch);

    Typ get_typ() const override final { return Typ::SUB; }

    void tick(Welt* welt, float s) override final;

};
