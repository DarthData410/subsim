#include "szene.hpp"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreViewport.h>
#include <OgreImGuiOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreOverlaySystem.h>
#include <OgreMeshManager.h>

#include <iostream>
#include <memory>

Szene::Szene() : OgreBites::ApplicationContext("nadasub") {

}

Szene::~Szene() {
    /*getRenderWindow()->removeAllViewports();
    removeInputListener(mImguiListener.get());
    scnMgr->removeRenderQueueListener(Ogre::OverlaySystem::getSingletonPtr());
    Ogre::OverlayManager::getSingleton().destroyAll();
    shadergen->removeSceneManager(scnMgr);
    getRoot()->destroySceneManager(scnMgr);
    removeInputListener(this);
    getRenderWindow()->removeListener(this);*/
}

void Szene::setup() {
    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();

    // register for input events
    addInputListener(this);

    // get a pointer to the already created root
    Ogre::Root* root = getRoot();
    scnMgr = root->createSceneManager();

    // Setup ImGui as Overlay
    Ogre::ImGuiOverlay* imguiOverlay = new Ogre::ImGuiOverlay();
    imguiOverlay->setZOrder(300);
    imguiOverlay->show();
    Ogre::OverlayManager::getSingleton().addOverlay(imguiOverlay); // now owned by overlaymgr
    mImguiListener = std::make_unique<OgreBites::ImGuiInputListener>();
    addInputListener(mImguiListener.get());
    scnMgr->addRenderQueueListener(Ogre::OverlaySystem::getSingletonPtr());
    getRenderWindow()->addListener(this);

    // register our scene with the RTSS
    shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // without light we would just get a black screen
    Ogre::Light* light = scnMgr->createLight("MainLight");
    lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(light);

    // also need to tell where we are
    camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(0, 0, 15);
    camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

    // create the camera
    Ogre::Camera* cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);
    getRenderWindow()->addViewport(cam);

    // Himmel // drawFirst evtl. problematisch
    scnMgr->setSkyBox(true, "Examples/MorningSkyBox", 200, false);

    // Unterwasser https://ogrecave.github.io/ogre/api/latest/tut__terrain_sky_fog.html
    Ogre::ColourValue fadeColour(0.5, 0.5, 0.7);
    getRenderWindow()->getViewport(0)->setBackgroundColour(fadeColour);
    scnMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0, 0, 250);

    // finally something to render
    Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    Ogre::Entity* ent = scnMgr->createEntity("sub1.mesh");
    //ent->getWorldBoundingBox().intersects(ent->getWorldBoundingBox());
    node->attachObject(ent);
    node->setPosition(0, -1.5, 0);
    node->pitch(Ogre::Degree(-90));
    node->roll(Ogre::Degree(45));

    // Ground
    Ogre::MeshManager::getSingleton().createPlane("water", Ogre::RGN_DEFAULT,
                                                  {Ogre::Vector3::UNIT_Y, 0},
                                                  1500, 1500,
                                                  20, 20,
                                                  true, 1,
                                                  5, 5,
                                                  Ogre::Vector3::UNIT_Z);
    Ogre::Entity* ground = scnMgr->createEntity("water");
    ground->setCastShadows(false);
    ground->setMaterialName("Examples/Water4"); // 4 is ok
    Ogre::SceneNode* node_ground = scnMgr->getRootSceneNode()->createChildSceneNode();
    node_ground->attachObject(ground);
    node_ground->setPosition(0,0,0); // TODO move
}

bool Szene::keyPressed(const OgreBites::KeyboardEvent& evt) {
    switch (evt.keysym.sym) {
        using namespace OgreBites;
        case SDLK_ESCAPE: getRoot()->queueEndRendering(); break;
        case SDLK_RIGHT: camNode->translate( 1,0,0); break;
        case SDLK_LEFT:  camNode->translate(-1,0,0); break;
        case SDLK_UP:    camNode->translate(0 ,1,0); break;
        case SDLK_DOWN:  camNode->translate(0,-1,0); break;
        default: break;
    }
    return true;
}

void Szene::preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt) {
    if (!evt.source->getOverlaysEnabled()) return;
    //if (!mTrayMgr->getTraysLayer()->isVisible()) return;
    Ogre::ImGuiOverlay::NewFrame();
    ImGui::ShowDemoWindow();


}
