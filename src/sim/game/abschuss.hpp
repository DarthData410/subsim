#pragma once

#include <memory>
#include <cereal/types/memory.hpp>

class Objekt;

class Abschuss final {

public:

    Abschuss() = default;

    Abschuss(const Objekt* o_sieger, const Objekt* o_opfer, const Objekt* waffe);

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(o_sieger, o_opfer, waffe);
    }

private:

    std::unique_ptr<Objekt> o_sieger;
    std::unique_ptr<Objekt> o_opfer;
    std::unique_ptr<Objekt> waffe;

};
