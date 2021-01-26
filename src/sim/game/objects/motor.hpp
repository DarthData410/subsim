#pragma once

struct Motor final {

    Motor() = default;

    Motor(float v_max, float a);

    /// Tick mit Zeit in Sekunden.
    void tick(float s);

    /// Liefert den Bremsweg bei aktueller Geschwindigkeit.
    float get_bremsweg() const;

    /// Höchstgeschwindigkeit
    float v_max;

    /// Zielgeschwindigkeit
    float v_target;

    /// Aktuelle Geschwindigkeit. Entspricht Neigung bei Rudern.
    float v;

    /// Beschleunigung.
    float a;

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(v_max, v_target, v, a);
    }

};
