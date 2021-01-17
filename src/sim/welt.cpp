#include "welt.hpp"

Welt::Welt() {

}

Welt::~Welt() {

}

void Welt::tick() {
    const float s = timer.getMilliseconds() / 1000.f; // Sekunden vergangen
    timer.reset();

    // Timing
    /*
    if (s > 0) {
        last_ticks = jetzt;
        physics_world->update(vergangen);
    }*/
}
