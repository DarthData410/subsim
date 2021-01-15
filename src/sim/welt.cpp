#include "welt.hpp"

Welt::Welt() {
    rp3d::PhysicsWorld::WorldSettings settings;
    settings.gravity = rp3d::Vector3(0, 0, 0);
    physics_world = physics_common.createPhysicsWorld(settings);
    //last_ticks = SDL_GetTicks();
}

void Welt::tick() {
    if (!body) {
        // Create a rigid body in the world
        rp3d::Vector3 position = rp3d::Vector3(100, 200, 300);
        rp3d::Transform transform(position, rp3d::Quaternion::identity());
        body = physics_world->createRigidBody(transform);
        body->enableGravity(false);

        // Form + Collider
        rp3d::CapsuleShape* capsule_shape = physics_common.createCapsuleShape(2.0, 10.0);
        rp3d::Transform transform_capsule(position, rp3d::Quaternion::identity()); // TODO drehen
        rp3d::Collider* collider = body->addCollider(capsule_shape, transform_capsule);
        (void) collider;
        body->updateMassPropertiesFromColliders();
        body->setLinearDamping(0.8);
        //body->setAngularDamping(100);
    }

    // Timing
    /* TODO
    const auto jetzt = SDL_GetTicks();
    const auto vergangen = (1.f / 60'000.f) * static_cast<float>(jetzt - last_ticks);
    if (vergangen > 0) {
        last_ticks = jetzt;
        physics_world->update(vergangen);
    }*/
}

Welt::~Welt() {
    physics_world->destroyRigidBody(body);
    physics_common.destroyPhysicsWorld(physics_world);
}
