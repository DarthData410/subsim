#pragma once

#include "../sim/game/objects/sub.hpp"
#include "../sim/net/klient.hpp"
#include "sonar_ui.hpp"
#include "waffen_ui.hpp"

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

    virtual ~Spielszene();

private:

    enum Tab : uint8_t {
        MAINMENU,   // TODO
        NAV,
        SONAR,      // TODO
        WEAPONS,    // TODO
        THREE_D
    };

    /// Zeigt das Hauptmenü.
    void render_menu();

    /// Zeigt die Navigation inkl. Strategie - (Teams, Zonen, Karte).
    void render_nav();

    /// Zeigt Aktives, Passives und Externes Sonar.
    void render_sonar();

    /// Zeigt Zielsteuerung & Waffenkontrolle.
    void render_weapons();

    /// Zeigt die 3D-Ansicht.
    void render_3d();

    /// Anzuzeigendes Fenster.
    Tab tab = NAV;

    /// Netzwerkklient
    Klient* klient = nullptr;

    /// Simulation
    std::optional<Sub> player_sub = std::nullopt;
    Sonar_UI sonar_ui;
    Waffen_UI waffen_ui;

    /// Gfx
    Ogre::RenderWindow* window = nullptr;
    Ogre::SceneManager* scene_manager = nullptr;
    Ogre::SceneNode* camNode   = nullptr;
    Ogre::SceneNode* lightNode = nullptr;
    Ogre::SceneNode* subNode = nullptr;
};
