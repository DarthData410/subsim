#pragma once

#include "objekt_steuerbar.hpp"

class Torpedo final : public Objekt_Steuerbar {

public:

    Objekt::Typ get_typ() const override final { return Typ::TORPEDO; }

    void tick(Welt* welt, float s) override final;

private:


};
