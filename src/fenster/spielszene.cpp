#include "spielszene.hpp"
#include "../sim/physik.hpp"

#include <log.hpp>
#include <OgreNode.h>
#include <OgreEntity.h>
#include <OgreRenderWindow.h>
#include <OgreMeshManager.h>
#include <OgreViewport.h>
#include <SDL2/SDL_keycode.h>

Spielszene::Spielszene(const std::string& ip) : klient(new Klient(ip)) {
    //
}

Spielszene::Spielszene(Ogre::RenderWindow* window, Ogre::SceneManager* scene_manager)
    : Spielszene()
{
    //window->resize(1600,900);
    Spielszene::scene_manager = scene_manager;

    // without light we would just get a black screen
    Ogre::Light* light = scene_manager->createLight("MainLight");
    lightNode = scene_manager->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(light);

    // also need to tell where we are
    Ogre::Camera* cam = scene_manager->createCamera("myCam");
    cam->setAutoAspectRatio(true);
    cam->setNearClipDistance(3); // specific to this sample
    //cam->setFarClipDistance() // TODO
    window->addViewport(cam);
    camNode = scene_manager->getRootSceneNode()->createChildSceneNode();
    camNode->attachObject(cam);
    //camNode->setPosition(0, 0, 15);
    //camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);
    //camNode->setPosition(Ogre::Vector3(0, 5, 15));

    // Himmel // drawFirst evtl. problematisch
    scene_manager->setSkyBox(true, "Examples/MorningSkyBox", 200, false);

    // Unterwasser https://ogrecave.github.io/ogre/api/latest/tut__terrain_sky_fog.html
    //Ogre::ColourValue fadeColour(0.5, 0.5, 0.7);
    //window->getViewport(0)->setBackgroundColour(fadeColour);
    //scene_manager->setFog(Ogre::FOG_LINEAR, fadeColour, 0, 0, 250);

    //camNode->setAutoTracking(true, subNode, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3(0, 0, 150));

    // Ozean
    // TODO Schöneres Wasser: https://github.com/OGRECave/ogre/tree/master/Samples/OceanDemo
    Ogre::MeshManager::getSingleton().createPlane("water", Ogre::RGN_DEFAULT,
                                                  {Ogre::Vector3::UNIT_Y, 0},
                                                  1500, 1500,
                                                  20, 20,
                                                  true, 1,
                                                  5, 5,
                                                  Ogre::Vector3::UNIT_Z);
    Ogre::Entity* ground = scene_manager->createEntity("water");
    ground->setCastShadows(false);
    ground->setMaterialName("Examples/Rockwall"); // 4 is ok //Examples/Water4
    Ogre::SceneNode* node_ground = scene_manager->getRootSceneNode()->createChildSceneNode();
    node_ground->attachObject(ground);
    node_ground->setPosition(0, 0, 0); // TODO move?
}

Spielszene::~Spielszene() {
    delete klient;
}

void Spielszene::key_pressed(const OgreBites::Keysym& key) {
    switch (key.sym) {
        case SDLK_RIGHT: camNode->yaw(Ogre::Degree(-1)); break;
        case SDLK_LEFT:  camNode->yaw(Ogre::Degree(1)); break;
        case SDLK_UP:    camNode->pitch(Ogre::Degree(1)); break;
        case SDLK_DOWN:  camNode->pitch(Ogre::Degree(-1)); break;
        case SDLK_m: {
            // Neues Spieler Sub geben (nur wenn keins vorhanden)
            if (player_sub) { Log::debug() << "New player_sub not needed\n"; break; }
            const std::string& antwort = klient->request(Net::AKTION_NEUES_UBOOT, 1);
            if (!antwort.empty()) {
                player_sub = Net::deserialize<Sub>(antwort);
                sonar_ui = Sonar_UI(&player_sub.value());
                waffen_ui = Waffen_UI(&player_sub.value());
            }
            else Log::err() << "New player_sub not available\n";
        } break;

        // Tabs
        case SDLK_F1: tab = NAV;      break;
        case SDLK_F2: tab = SONAR;    break;
        case SDLK_F3: tab = WEAPONS;  break;
        case SDLK_F4: tab = THREE_D;  break;
        case SDLK_F5: tab = MAINMENU; break;
        default: break;
    }
}

void Spielszene::sync() {
    // Net Sync
    if (static Ogre::Timer timer; timer.getMilliseconds() >= 500.f) {
        if (player_sub) {
            const std::string& antwort = klient->request(Net::REQUEST_SUB, player_sub->get_id());
            if (!antwort.empty()) player_sub = Net::deserialize<Sub>(antwort);
            else Log::err() << "Spielszene::" << __func__ << " no sub returned with ID " << player_sub->get_id() << '\n';
        }
        timer.reset();
    }
}

void Spielszene::render() {
    sync();

    // Gfx Interpolieren
    if (static Ogre::Timer timer_interpol; player_sub.has_value()) {
        player_sub->tick(nullptr, timer_interpol.getMilliseconds() / 1000.f);
        timer_interpol.reset();

        // Gfx erstellen?
        if (subNode == nullptr) {
            Ogre::Entity *ent = scene_manager->createEntity("Sinbad.mesh"); //sub1.mesh
            subNode = scene_manager->getRootSceneNode()->createChildSceneNode();
            //ent->getWorldBoundingBox().intersects(ent->getWorldBoundingBox());
            ent->setCastShadows(true);
            subNode->attachObject(ent);
            //camNode->setAutoTracking(true, subNode);
        }
        subNode->setPosition(player_sub->get_pos());
        subNode->setOrientation(player_sub->get_orientation());
        camNode->setPosition(subNode->getPosition() + Ogre::Vector3(10, 1, 10));
    }
    switch (tab) {
        case MAINMENU:  render_menu();      break;
        case NAV:       render_nav();       break;
        case SONAR:     render_sonar();     break;
        case WEAPONS:   render_weapons();   break;
        case THREE_D:   render_3d();        break;
        default:        tab = MAINMENU;     break;
    }
}
