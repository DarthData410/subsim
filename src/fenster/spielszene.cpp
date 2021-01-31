#include "spielszene.hpp"
#include "../sim/physik.hpp"
#include "imgui_addons.hpp"

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
    Spielszene::scene_manager = scene_manager;

    // without light we would just get a black screen
    Ogre::Light* light = scene_manager->createLight("MainLight");
    lightNode = scene_manager->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(light);

    // also need to tell where we are
    Ogre::Camera* cam = scene_manager->createCamera("myCam");
    cam->setAutoAspectRatio(true);
    cam->setNearClipDistance(5); // specific to this sample
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
    node_ground->setPosition(0,0,0); // TODO move?
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
            if (player_sub) { Log::debug() << "New player_sub not needed\n"; break; }
            const std::string& antwort = klient->request(Net::AKTION_NEUES_UBOOT, 1);
            if (!antwort.empty()) player_sub = Net::deserialize<Sub>(antwort);
            else Log::err() << "New player_sub not available\n";
        } break;
        default: break;
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
        render_subcontrol();
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

void Spielszene::render_subcontrol() {
    ImGui::SetNextWindowSize({300,0});
    ImGui::Begin("debugWindow");

    ImGui::Text("Sub: %.1f %.1f %.1f", player_sub->get_pos().x, player_sub->get_pos().y, player_sub->get_pos().z);
    ImGui::Text("Pitch: %.1f", player_sub->get_pitch());
    ImGui::Text("Bearing: %.1f", player_sub->get_bearing());

    static float target_x = 0, target_z = 0;
    ImGui::InputFloat("Target_x", &target_x);
    ImGui::InputFloat("Target_z", &target_z);
    if (ImGui::Button("Set##set_target_pos")) player_sub->set_target_pos(target_x, target_z);
    if (ImGui::Button("bearing")) { // Physik::bearing Test
        std::cout << Physik::bearing(player_sub->get_pos().x, player_sub->get_pos().z, target_x, target_z) << '\n';
    }

    static float target_bearing = 0;
    ImGui::Nada::KnobDegree("target_bearing", &target_bearing);
    if (ImGui::Button("Set##set_bearing")) player_sub->set_target_bearing(target_bearing);
    ImGui::Separator();

    static float target_speed = 0;
    ImGui::SliderFloat("target_speed", &target_speed, -1.0f, 1.0f);
    if (ImGui::Button("Set##set_speed")) {
        klient->kommando({Kommando::MOTOR_LINEAR, player_sub->get_id(), target_speed});
    }
    ImGui::Separator();

    if (ImGui::Button("vorwärts")) player_sub->set_target_v(100);
    if (ImGui::Button("stop")) player_sub->stop();
    if (ImGui::Button("rückwärts")) player_sub->set_target_v(-100);
    if (ImGui::Button("rechts")) player_sub->set_target_rudder(100);
    if (ImGui::Button("links")) player_sub->set_target_rudder(-100);
    if (ImGui::Button("auf")) player_sub->set_target_pitch(100);
    if (ImGui::Button("ab")) player_sub->set_target_pitch(-100);

    ImGui::End();
}
