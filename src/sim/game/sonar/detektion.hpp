#include <cstdint>

/// Einzelne Sichtung.
struct Detektion final {

    enum Typ : uint8_t {
        MOVEMENT_SIGNATURE = 1,
    };

    uint32_t objekt_id;
    float gain;
    float bearing;
    Typ typ;

    template <class Archive> void serialize(Archive& ar) {
        ar(objekt_id, gain, bearing, typ);
    }

};
