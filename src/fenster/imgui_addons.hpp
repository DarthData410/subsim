#pragma once

#include <imgui.h>

namespace ImGui::Nada {

    /// Modifiziert vom Original: https://github.com/Flix01/imgui
    bool KnobDegree(const char* label, float* p_value,
                    float v_min = 0.f, float v_max = 360.f, float v_step = 1.f,
                    float radius = 40.f, const char* fmt = "%.0f");

}
