#include "szene.hpp"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreViewport.h>
#include <OgreImGuiOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreOverlaySystem.h>
#include <iostream>
#include <memory>

Szene::Szene() : OgreBites::ApplicationContext("nadasub") {

}

Szene::~Szene() {
    Ogre::OverlayManager::getSingleton().destroy("ImGuiOverlay");
}

void Szene::setup() {
    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();

    // register for input events
    addInputListener(this);

    // get a pointer to the already created root
    Ogre::Root* root = getRoot();
    Ogre::SceneManager* scnMgr = root->createSceneManager();

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
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);

    // without light we would just get a black screen
    Ogre::Light* light = scnMgr->createLight("MainLight");
    lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(light);

    // also need to tell where we are
    Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(0, 0, 15);
    camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

    // create the camera
    Ogre::Camera* cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);

    // and tell it to render into the main window
    getRenderWindow()->addViewport(cam);

    // finally something to render
    Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    Ogre::Entity* ent = scnMgr->createEntity("Sinbad.mesh");
    node->attachObject(ent);
}

bool Szene::keyPressed(const OgreBites::KeyboardEvent& evt) {
    switch (evt.keysym.sym) {
        using namespace OgreBites;
        case SDLK_ESCAPE: getRoot()->queueEndRendering(); break;
        case SDLK_RIGHT: lightNode->translate( 1,0,0); break;
        case SDLK_LEFT:  lightNode->translate(-1,0,0); break;
        case SDLK_UP:    lightNode->translate(0 ,1,0); break;
        case SDLK_DOWN:  lightNode->translate(0,-1,0); break;
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
