#pragma once

#include "sub.hpp"

class Sub_AI final : public Sub {

    enum Status : uint8_t {
        DONE    = 0,
        TRAVEL = 1 << 0,
        SEARCH  = 1 << 1,
        HIDE    = 1 << 2,
        EVADE   = 1 << 3,
        ATTACK  = 1 << 4,
    };

public:

    Typ get_typ() const override { return Typ::SUB_AI; }

    void tick(Welt* welt, float s) override;

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Sub>(this));
    }

private:

    bool hat_status(Status status) const { return this->status & status; }

    void clear_status() { status = DONE; }

    void add_status(Status status) { this->status |= status; }

    void remove_status(Status status) { this->status &= ~status; }

private:

    /// Timer für neue Aufgaben.
    Ogre::Timer timer;

    /// Zielkoordinaten. Zieltyp ist kontextabhängig.
    Ogre::Vector3 ziel;

    /// Aktuelle Aufgabe.
    uint8_t status = DONE;

};
