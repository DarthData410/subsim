#include "image.hpp"

#include <OgreTextureManager.h>

Image::Image(const std::string& filename)
    : raw_data(nullptr), ressource_id(filename)
{
    img.load(filename, ressource_group);
    size = {static_cast<float>(img.getWidth()), static_cast<float>(img.getHeight())};
    reload_texture_from_image(false);
}

Image::Image(uint8_t* pixel, int w, int h, Ogre::PixelFormat pixel_format)
    : img(pixel_format, w, h, 1, pixel, false), raw_data(pixel)
{
    size = {static_cast<float>(w), static_cast<float>(h)};
    reload_texture_from_image();
}

Image::~Image() {
    // TODO check for leaks
    //Ogre::TextureManager::getSingleton().unload(texture->getHandle());
    //delete[] raw_data;
    //raw_data = nullptr;
}

ImTextureID Image::get_as_imgui() const {
    reinterpret_cast<ImTextureID>(texture->getHandle());
}

void Image::reload_texture_from_image(bool new_id) {
    // Ogre Ressourcenmanagement
    if (!Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(ressource_group)) {
        Ogre::ResourceGroupManager::getSingleton().createResourceGroup(ressource_group);
    }
    // Textur laden; Einmalige ID zuweisen, wenn "custom pixel"
    static uint64_t counter = 0;
    if (new_id) ressource_id = std::to_string(counter++); // TODO unload old
    texture = Ogre::TextureManager::getSingleton().loadImage(ressource_id, ressource_group, img).get();
}

void Image::add_pixel_row(uint8_t* pixel) { // TODO fix
    raw_data = img.getData();
    std::memmove(raw_data, raw_data + w, w * (h-1)); // 1px nach oben schieben
    std::copy(pixel, pixel + w, raw_data + w * (h-1));
    reload_texture_from_image(true);
}
