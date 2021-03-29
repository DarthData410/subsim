#pragma once

#include <cereal/archives/binary.hpp>

/// Globale Netzwerkdefinitionen.
class Net final {

public:

    /// Port für Server + Klient.
    static constexpr uint16_t PORT = 29078;

    /// Typen von Anfragen vom Klienten.
    enum Request : uint8_t {
        SUB_CMD            = 0,   // "Kommando"
        AKTION_NEUES_UBOOT = 1,   // "uint8_t Team" -> "Sub"
        REQUEST_SUB        = 2,   // "uint32_t ID"  -> "Sub" oder ""
        ALLE_OBJEKTE       = 3,   // "" -> std::vector<Objekt>
        BROADCAST          = 100, // ServerBroadcast -> Klienten auf Kanal 2: welt - "teams" & "zonen"
    };

    /// Einheitliche Klasse zur Serialisierung.
    typedef cereal::BinaryOutputArchive Serializer;

    /// Einheitliche Klasse zur Deserialisierung.
    typedef cereal::BinaryInputArchive Deserializer;

    /// Einheitlicher Typ für IDs.
    typedef uint32_t id_t;

    /// Serialisiert gegebenes Objekt in den `stringstream`, der zuvor **zurückgesetzt** wird.
    template<typename T>
    static std::string serialize(const T& t) {
        std::stringstream ss;
        Serializer s(ss);
        s << t;
        return ss.str();
    }

    /// Deserialisiert einen String in gegebenen Objekttypen über default-Ctor und gibt es zurück.
    template<typename T>
    static T deserialize(const std::string& s) {
        std::stringstream ss(s);
        Deserializer ds(ss);
        T t;
        ds >> t;
        return t;
    }

};
