#pragma once

#include <memory>
#include <cereal/types/memory.hpp>

class Objekt;

/// Führt Statistik über einen Abschuss.
class Abschuss final {

public:

    Abschuss() = default;

    Abschuss(const Abschuss& a);

    Abschuss(Abschuss&& a) = default;

    /// TODO Sicherheit der Ctors wenig getestet
    Abschuss(const Objekt* o_sieger, const Objekt* o_opfer, const Objekt* waffe);

    const std::string& get_as_text() const;

    const std::unique_ptr<Objekt>& get_sieger() const { return o_sieger; }

    const std::unique_ptr<Objekt>& get_opfer() const { return o_opfer; }

    const std::unique_ptr<Objekt>& get_waffe() const { return waffe; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(o_sieger, o_opfer, waffe, text);
    }

private:

    std::unique_ptr<Objekt> o_sieger;
    std::unique_ptr<Objekt> o_opfer;
    std::unique_ptr<Objekt> waffe;
    mutable std::string text;

};
