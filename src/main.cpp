#include <iostream>

#include <reactphysics3d/reactphysics3d.h>

using namespace reactphysics3d;


int main() {
    std::cout << "Hello, World!" << std::endl;

    PhysicsCommon physicsCommon;
    PhysicsWorld* world = physicsCommon.createPhysicsWorld();
    Vector3 position(0, 20, 0);
    Quaternion orientation = Quaternion::identity();
    Transform transform(position, orientation);
    RigidBody* body = world->createRigidBody(transform);
    const decimal timeStep = 1.0f / 60.0f;

    for (int i=0; i < 20; i++) {

        world->update(timeStep);

        // Get the updated position of the body
        const Transform& transform = body->getTransform();
        const Vector3& position = transform.getPosition();

        // Display the position of the body
        std::cout << "Body Position: (" << position.x << ", "
                  << position.y << ", " << position.z << ")" << std::endl;
    }

    world->destroyRigidBody(body);
    physicsCommon.destroyPhysicsWorld(world);

    return 0;
}
