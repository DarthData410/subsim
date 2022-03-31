#pragma once

/// Antrieb für physikalische Objekte in der Simulation.
struct Motor final {

    /// Ctor.
    Motor() = default;

    /// Erstellt einen Motor mit Höchstgeschwindigkeit v_max und Beschleunigung a.
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
