#pragma once

#include "../sim/game/objects/sub.hpp"
#include "../sim/net/klient.hpp"

#include <OgreInput.h>
#include <OgreSceneManager.h>

class Spielszene final {

public:

    /// Root-Ctor, wird von anderen Ctors aufgerufen zur Verbindungserstellung.
    explicit Spielszene(const std::string& ip = "127.0.0.1");

    Spielszene(Ogre::RenderWindow* window, Ogre::SceneManager* scene_manager);

    void render();

    void key_pressed(const OgreBites::Keysym& key);

    virtual void sync();

private:

    /// Zeigt das eigene Sub + dessen Steuerung
    void render_subcontrol();

    /// Netzwerkklient
    Klient klient;

    /// Simulation
    std::optional<Sub> player_sub = std::nullopt;

    /// Gfx
    Ogre::RenderWindow* window = nullptr;
    Ogre::SceneManager* scene_manager = nullptr;
    Ogre::SceneNode* camNode   = nullptr;
    Ogre::SceneNode* lightNode = nullptr;
    Ogre::SceneNode* subNode = nullptr;

};
