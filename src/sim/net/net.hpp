#pragma once

#include <cereal/archives/binary.hpp>

class Net final {

public:

    /// Einheitliche Klasse zur Serialisierung.
    typedef cereal::BinaryOutputArchive Serializer;

    /// Einheitliche Klasse zur Deserialisierung.
    typedef cereal::BinaryInputArchive Deserializer;

    /// Port für Server + Klient.
    static constexpr uint16_t PORT = 29078;

    /// Typen von Anfragen vom Klienten.
    enum Request : uint8_t {
        SUB_CMD = 0
    };

};
