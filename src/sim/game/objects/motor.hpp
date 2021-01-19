#pragma once

struct Motor final {

    Motor() = default;

    Motor(float v_max, float a);

    /// Tick mit Zeit in Sekunden.
    void tick(float s);

    /// Höchstgeschwindigkeit
    float v_max;

    /// Zielgeschwindigkeit
    float v_target;

    /// Aktuelle Geschwindigkeit. Entspricht Neigung bei Rudern.
    float v;

    /// Beschleunigung.
    float a;

};
