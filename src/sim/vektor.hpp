#pragma once

class Vektor {

    using p_type = double;

public:

    Vektor() = default;
    Vektor(p_type x, p_type y, p_type z);

    p_type x() const { return vx; }
    p_type y() const { return vy; }
    p_type z() const { return vz; }
    void x(p_type x) { Vektor::vx = x; }
    void y(p_type y) { Vektor::vy = y; }
    void z(p_type z) { Vektor::vz = z; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(vx, vy, vz);
    }

private:

    p_type vx;
    p_type vy;
    p_type vz;

};
