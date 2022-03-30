#pragma once

class Vektor {

    typedef double dist_t;

public:

    Vektor() = default;
    Vektor(dist_t x, dist_t y, dist_t z);

    dist_t x() const { return vx; }
    dist_t y() const { return vy; }
    dist_t z() const { return vz; }
    void x(dist_t x) { Vektor::vx = x; }
    void y(dist_t y) { Vektor::vy = y; }
    void z(dist_t z) { Vektor::vz = z; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(vx, vy, vz);
    }

private:

    dist_t vx;
    dist_t vy;
    dist_t vz;

};
