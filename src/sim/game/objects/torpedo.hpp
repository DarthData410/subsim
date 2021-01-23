#pragma once

#include "objekt_steuerbar.hpp"

class Torpedo final : public Objekt_Steuerbar {

public:

    Objekt::Typ get_typ() const override { return Typ::TORPEDO; }

    virtual void tick(Welt* welt, float s) final;

private:


};
