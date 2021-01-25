#include "welt.hpp"

Welt::Welt() {

}

Welt::~Welt() {

}

void Welt::tick() {
    // Timing
    const float s = timer.getMilliseconds() / 1000.f; // Sekunden vergangen
    timer.reset();

    for (const auto& objekt : objekte) objekt.second->tick(this, s);
}
