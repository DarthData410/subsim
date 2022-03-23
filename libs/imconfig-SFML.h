// Quelle: https://github.com/eliasdaler/imgui-sfml Version 2.4

/*
   The MIT License (MIT)

Copyright (c) 2016 Elias Daler
Copyright (c) 2014-2016 Omar Cornut and ImGui contributors
Copyright (c) 2014 Mischa Aster Alff

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#pragma once
// Add this to your imconfig.h

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "imgui-SFML_export.h"

#define IM_VEC2_CLASS_EXTRA           \
    template<typename T>              \
    ImVec2(const sf::Vector2<T>& v) { \
        x = static_cast<float>(v.x);  \
        y = static_cast<float>(v.y);  \
    }                                 \
                                      \
    template<typename T>              \
    operator sf::Vector2<T>() const { \
        return sf::Vector2<T>(x, y);  \
    }

#define IM_VEC4_CLASS_EXTRA                                                                        \
    ImVec4(const sf::Color& c) : x(c.r / 255.f), y(c.g / 255.f), z(c.b / 255.f), w(c.a / 255.f) {} \
    operator sf::Color() const {                                                                   \
        return sf::Color(static_cast<sf::Uint8>(x * 255.f), static_cast<sf::Uint8>(y * 255.f),     \
                         static_cast<sf::Uint8>(z * 255.f), static_cast<sf::Uint8>(w * 255.f));    \
    }

#define ImTextureID unsigned int

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
