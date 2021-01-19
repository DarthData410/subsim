#pragma once

#include "spielszene.hpp"

#include <OgreApplicationContext.h>
#include <OgreRenderTargetListener.h>
#include <OgreImGuiInputListener.h>

class Szene : public OgreBites::ApplicationContext,
              public OgreBites::InputListener,
              public Ogre::RenderTargetListener {

public:

    Szene();

    void setup() override;

    bool keyPressed(const OgreBites::KeyboardEvent& evt) override;

    void preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt) override;

    //bool frameStarted(const Ogre::FrameEvent& evt) override;

    virtual ~Szene();

private:

    Spielszene spielszene;

    std::unique_ptr<OgreBites::ImGuiInputListener> mImguiListener;
    Ogre::RTShader::ShaderGenerator* shadergen = nullptr;
    Ogre::SceneManager* scnMgr = nullptr;

};
