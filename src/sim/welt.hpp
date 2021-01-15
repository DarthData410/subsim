#include "physik.hpp"

class Welt final : public Physik {

public:

    Welt();

    virtual ~Welt();

    void tick();

    rp3d::RigidBody* body = nullptr;

private:

    uint32_t last_ticks;

};
