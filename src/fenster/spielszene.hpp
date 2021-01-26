#pragma once

#include "../sim/game/objects/sub.hpp"

#include <OgreInput.h>
#include <OgreSceneManager.h>

class Spielszene final {

public:

    Spielszene() = default;

    Spielszene(Ogre::RenderWindow* window, Ogre::SceneManager* scene_manager);

    void render();

    void key_pressed(const OgreBites::Keysym& key);

    virtual void sync();

private:

    void render_subcontrol();

    /// Simulation Data
    std::optional<Sub> player_sub = std::nullopt;

    /// Gfx Data
    Ogre::RenderWindow* window = nullptr;
    Ogre::SceneManager* scene_manager = nullptr;
    Ogre::SceneNode* camNode   = nullptr;
    Ogre::SceneNode* lightNode = nullptr;
    Ogre::SceneNode* subNode = nullptr;

};
