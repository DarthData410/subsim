#pragma once

#include "net.hpp"
#include <cereal/types/string.hpp>
#include <cereal/types/tuple.hpp>

class Welt;

/// Eine vom Klienten gesendete, auszuführende Aktion auf das Klient-Sub.
class Kommando final {

public:

    /// Welches Kommando soll ausgeführt werden?
    enum Typ : uint8_t {
        STOP = 0,       // Alle Motoren stopp.
        MOTOR_LINEAR,   // Etwas am MOTOR_LINEAR ändern, z.B. schneller / langsamer.
        MOTOR_ROT,      // Etwas am MOTOR_ROT ändern.
        MOTOR_TAUCH,    // Etwas am MOTOR_TAUCH ändern.
        AUTO_POS,       // Automatische Wegfindung zu einer Position.
        AUTO_KURS,      // Automatisches Drehen zu einem Kurs.
        AUTO_TIEFE,     // Automatisches Tauchen zu einer Tiefe.
        TORP_LAUNCH,    // Torpedo starten.
        TIMELAPSE       // Simulationszeit Ändern
    };

    /// Ctor.
    Kommando() = default;

    /// Ctor für Kommandos ohne Daten.
    Kommando(Typ typ, uint32_t sub_id) : typ(typ), sub_id(sub_id) {}

    /// Ctor mit optionalen Daten.
    template<typename T>
    Kommando(Typ typ, uint32_t sub_id, const T& daten) : Kommando(typ, sub_id) {
        data = as_string(daten);
    }

    /// Wendet dieses Kommando auf `welt` an.
    void apply(Welt* welt);

    /// Serialisierung via Cereal.
    template<class Archive>
    void serialize(Archive& ar) { ar(typ, sub_id, data); }

private:

    /// Hilfsmethode, die die Daten aus diesem Kommando (data) in T konvertiert.
    template<typename T>
    T as() {
        std::stringstream ss(data);
        Net::Deserializer ds(ss);
        T t;
        ds >> t;
        return t;
    }

    /** 
     * Wandelt ein beliebiges Objekt in einen String um, 
     * um diese als Daten (daten in diesem Kommando) zu speichern.
     */
    template<typename T>
    std::string as_string(const T& daten) {
        std::stringstream ss;
        Net::Serializer s(ss);
        s << daten;
        return ss.str();
    }

private:

    Typ typ;

    uint32_t sub_id;

    /// Enthält evtl. zusätzliche Daten zum senden in einem Netzwerkpaket.
    std::string data;

};