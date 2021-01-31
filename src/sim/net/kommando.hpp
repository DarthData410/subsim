#pragma once

#include "net.hpp"
#include <cereal/types/string.hpp>
#include <cereal/types/tuple.hpp>

class Welt;

class Kommando final {

public:

    enum Typ : uint8_t {
        STOP = 0,
        MOTOR_LINEAR,
        MOTOR_ROT,
        MOTOR_TAUCH,
        AUTO_POS,
        AUTO_KURS,
        AUTO_TIEFE
    };

    /// Ctor.
    Kommando() = default;

    /// Ctor für Kommandos ohne Daten.
    Kommando(Typ typ, uint32_t sub_id) : typ(typ), sub_id(sub_id) {}

    /// Ctor mit optionalen Daten.
    template<typename T>
    Kommando(Typ typ, uint32_t sub_id, T daten) : Kommando(typ, sub_id) {
        data = as_string(daten);
    }

    Typ typ;

    uint32_t sub_id;

    std::string data;

    void apply(Welt* welt);

    template<class Archive>
    void serialize(Archive& ar) { ar(typ, sub_id, data); }

private:

    template<typename T>
    T as() {
        std::stringstream ss(data);
        Net::Deserializer ds(ss);
        T t;
        ds >> t;
        return t;
    }

    template<typename T>
    std::string as_string(const T& daten) {
        std::stringstream ss;
        Net::Serializer s(ss);
        s << daten;
        return ss.str();
    }

};