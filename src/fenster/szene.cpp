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
    delete spielszene;
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
    spielszene = new Spielszene(getRenderWindow(), scnMgr);
}

bool Szene::keyPressed(const OgreBites::KeyboardEvent& evt) {
    switch (evt.keysym.sym) {
        using namespace OgreBites;
        case SDLK_ESCAPE: getRoot()->queueEndRendering(); break;
        default: spielszene->key_pressed(evt.keysym);
    }
    return true;
}

void Szene::preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt) {
    if (!evt.source->getOverlaysEnabled()) return;
    //if (!mTrayMgr->getTraysLayer()->isVisible()) return;
    Ogre::ImGuiOverlay::NewFrame();
    //ImGui::ShowDemoWindow();
    spielszene->render();
}
