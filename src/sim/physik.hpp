#include <reactphysics3d/reactphysics3d.h>

class Physik {

public:

    static rp3d::PhysicsCommon& get_common() { return physics_common; }

    static rp3d::PhysicsWorld* get_world() { return physics_world; }

    static void rotate2d(rp3d::Vector3& v, float angle);

protected:

    static inline rp3d::PhysicsCommon physics_common;

    static inline rp3d::PhysicsWorld* physics_world = nullptr;

};
