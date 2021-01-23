#include "spielszene.hpp"
#include "../sim/welt.hpp"

#include <imgui.h>
#include <OgreNode.h>
#include <OgreEntity.h>
#include <OgreRenderWindow.h>
#include <OgreMeshManager.h>
#include <OgreViewport.h>
#include <SDL2/SDL_keycode.h>

Spielszene::Spielszene(Ogre::RenderWindow* window, Ogre::SceneManager* scene_manager)
    : player_sub(Ogre::Vector3(0, 2, 0),
                 Motor(5, 0.2),
                 Motor(2, 0.1),
                 Motor(20, 0.1))
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
    camNode->setAutoTracking(true, subNode);

    // Himmel // drawFirst evtl. problematisch
    scene_manager->setSkyBox(true, "Examples/MorningSkyBox", 200, false);

    // Unterwasser https://ogrecave.github.io/ogre/api/latest/tut__terrain_sky_fog.html
    //Ogre::ColourValue fadeColour(0.5, 0.5, 0.7);
    //window->getViewport(0)->setBackgroundColour(fadeColour);
    //scene_manager->setFog(Ogre::FOG_LINEAR, fadeColour, 0, 0, 250);

    // finally something to render
    Ogre::Entity* ent = scene_manager->createEntity("Sinbad.mesh"); //sub1.mesh
    subNode = scene_manager->getRootSceneNode()->createChildSceneNode();
    //ent->getWorldBoundingBox().intersects(ent->getWorldBoundingBox());
    ent->setCastShadows(true);
    subNode->attachObject(ent);

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

void Spielszene::render() {
    subNode->setPosition(player_sub.get_pos());
    subNode->setOrientation(player_sub.get_orientation());
    camNode->setPosition(subNode->getPosition() + Ogre::Vector3(10, 1, 10));

    ImGui::Begin("debugWindow");
    ImGui::Text("Sub: %.1f %.1f %.1f", player_sub.get_pos().x, player_sub.get_pos().y, player_sub.get_pos().z);
    ImGui::Text("Pitch: %.1f", player_sub.get_pitch());
    ImGui::Text("Bearing: %.1f", player_sub.get_bearing());

    static float target_bearing = 0;
    ImGui::SliderFloat("target_bearing", &target_bearing, 0, 359);
    if (ImGui::SameLine(); ImGui::Button("Set")) player_sub.set_target_bearing(target_bearing);

    if (ImGui::Button("vorwärts")) player_sub.set_target_v(100);
    if (ImGui::Button("stop")) player_sub.stop();
    if (ImGui::Button("rückwärts")) player_sub.set_target_v(-100);
    if (ImGui::Button("rechts")) player_sub.set_target_rudder(100);
    if (ImGui::Button("links")) player_sub.set_target_rudder(-100);
    if (ImGui::Button("auf")) player_sub.set_target_pitch(100);
    if (ImGui::Button("ab")) player_sub.set_target_pitch(-100);
    ImGui::End();

    static Ogre::Timer timer;
    static Welt welt;
    if (timer.getMilliseconds() > 50) {
        player_sub.tick(&welt, (float)timer.getMilliseconds() / 100.f);
        timer.reset();
    }
}

void Spielszene::key_pressed(const OgreBites::Keysym& key) {
    switch (key.sym) {
        case SDLK_RIGHT: camNode->yaw(Ogre::Degree(-1)); break;
        case SDLK_LEFT:  camNode->yaw(Ogre::Degree(1)); break;
        case SDLK_UP:    camNode->pitch(Ogre::Degree(1)); break;
        case SDLK_DOWN:  camNode->pitch(Ogre::Degree(-1)); break;
        default: break;
    }
}
