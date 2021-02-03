#pragma once

#include <imgui.h>
#include <OgreSharedPtr.h>
#include <OgreImage.h>
#include <OgreTexture.h>

class Image final {

public:

    static constexpr Ogre::PixelFormat RGB  = Ogre::PixelFormat::PF_R8G8B8;
    static constexpr Ogre::PixelFormat RGBA = Ogre::PixelFormat::PF_R8G8B8A8;

    explicit Image(const std::string& filename);

    Image(uint8_t* pixel, int w, int h, Ogre::PixelFormat pixel_format = RGB);

    ~Image();

    ImTextureID get_as_imgui() const;

    int get_w() const { return w; }

    int get_h() const { return h; }

    const ImVec2& get_size() const { return size; }

    void add_pixel_row(uint8_t* pixel);

private:

    static inline const std::string ressource_group = "nada";

    void reload_texture_from_image(bool new_id = true);

    int w;
    int h;
    ImVec2 size;

    std::string ressource_id;
    Ogre::Image img;
    Ogre::Texture* texture;
    uint8_t* raw_data;

};
