#include "nav_ui.hpp"

#include "../sim/net/klient.hpp"
#include "../sim/physik.hpp"
#include "gfx/ui.hpp"
#include "gfx/grafik.hpp"
#include "gfx/multigrafik.hpp"

#include <implot.h>
#include <nada/misc.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <cmath>

Nav_UI::Nav_UI() : Standard_UI(nullptr) {}

Nav_UI::Nav_UI(Klient* klient) : Standard_UI(klient),
    bg(new gfx::Grafik("data/gfx/bg_nav.png")),
    schalter_vent_air(new gfx::Multigrafik({
        "data/gfx/switch1_off.png", "data/gfx/switch1_on.png"
    }, 448 , 50)),
    schalter_emergency_surface(new gfx::Multigrafik({
        "data/gfx/switch2_off.png", "data/gfx/switch2_on.png"
    }, 1480 , 0)),
    lampe_air_available(new gfx::Multigrafik({
        "data/gfx/lampe_gruen_aus.png", "data/gfx/lampe_gruen_an.png"
    }, 366-400 , 260-400)),
    lampe_emergency_surface_aktiv(new gfx::Multigrafik({
        "data/gfx/lampe_rot_aus.png", "data/gfx/lampe_rot_an.png"
    }, 1770-400 , 40-400))
{

}

Nav_UI::~Nav_UI() {
    delete bg;
    delete schalter_vent_air;
    delete schalter_emergency_surface;
    delete lampe_air_available;
    delete lampe_emergency_surface_aktiv;
}

void Nav_UI::update_and_show(const Sub* sub) {
    sync(sub);
    handle_imgui_events(sub);
    ui::Font f(ui::FONT::MONO_16);
    show_navigation(sub);
    show_noise_signature(sub, sub->get_speed());
}

void Nav_UI::sync(const Sub* sub) {
    if (static sf::Clock timer; timer.getElapsedTime().asSeconds() < 1) return;
    else timer.restart();
    // Notauftauchen, wenn Heben betätigt TODO
    if (emergency_surface) klient->kommando({Kommando::MOTOR_TAUCH, sub->get_id(), sub->get_speed_max_tauch()});
}

void Nav_UI::show_navigation(const Sub* sub) const {
    ui::BeginInvisible("Nav View", {1474,432}, {300,358});
    ui::Text("Sub: %.1f %.1f Depth: %.1f", sub->get_pos().x(), sub->get_pos().y(), sub->get_pos().z());
    ui::Text("Pitch:   %.1f", sub->get_pitch());
    ui::Text("Bearing: %.1f (Target: %.f)", sub->get_bearing(), sub->get_target_bearing());
    ui::Text("Speed:   %.1f (Target: %.f)", sub->get_speed(), sub->get_target_speed());

/*    static float target_x = 0, target_z = 0; // - ohne auto-pos mehr immersion?
    ImGui::InputFloat("Target_x", &target_x);
    ImGui::InputFloat("Target_z", &target_z);
    if (ImGui::Button("Set##set_target_pos")) {
        klient->kommando({Kommando::AUTO_POS, sub->get_id(),
                          std::tuple<float, float>(target_x, target_z)});
    }*/

    ui::Separator();
    static float target_bearing = 0;
    ui::KnobDegree("Target Course", &target_bearing,
                            0.f, 360.f, 1.f, 40.f, 2.f, "%.0f°", std::make_optional(sub->get_bearing()));
    if (ImGui::SameLine(); ui::Button("Set##set_bearing")) {
        klient->kommando({Kommando::AUTO_KURS, sub->get_id(), target_bearing});
    }
    ui::Separator();

    static float target_speed = 0;
    ui::SliderFloat("Target Speed", &target_speed, -sub->get_speed_max(), sub->get_speed_max(), "%.1f");
    ui::MouseWheel(target_speed, 0.5f, -sub->get_speed_max(), sub->get_speed_max());
    if (ImGui::SameLine(); ui::Button("Set##set_speed")) {
        klient->kommando({Kommando::MOTOR_LINEAR, sub->get_id(), target_speed / sub->get_speed_max()});
    }
    ui::Separator();

    static int target_depth = std::abs(sub->get_pos().z());
    ui::SliderInt("Target Depth", &target_depth, std::abs(sub->SURFACE_DEPTH), 1000, "%d");
    ui::MouseWheel(target_speed, 0.5f, -sub->get_speed_max(), sub->get_speed_max());
    if (ImGui::SameLine(); ui::Button("Set##set_depth")) {
        klient->kommando({Kommando::AUTO_TIEFE, sub->get_id(), (float)-target_depth});
    }
    ui::Separator();

    if (ui::Button("stop")) {
        klient->kommando({Kommando::STOP, sub->get_id()});
    }
    if (ui::Button("rechts")) {
        klient->kommando({Kommando::MOTOR_ROT, sub->get_id(), 100.f});
    }
    if (ui::Button("links")) {
        klient->kommando({Kommando::MOTOR_ROT, sub->get_id(), -100.f});
    }
    ImGui::End();
}

void Nav_UI::show_noise_signature(const Sub* sub, std::optional<float> mark_v) const {
    ui::BeginInvisible("Noise signature", {60,383}, {507,340});
    const auto x_max = sub->get_speed_max() + 1;
    ImPlot::SetNextAxisLimits(ImAxis_X1, 0, x_max);
    ImPlot::SetNextAxisLimits(ImAxis_Y1, 0, 1);
    if (ImPlot::BeginPlot("##eigensub_noise_level", {-1,0}, ImPlotFlags_NoMenus | ImPlotFlags_Crosshairs)) {
        ImPlot::SetupAxes("v in m/s", "Noise Level", ImPlotAxisFlags_Lock, ImPlotAxisFlags_Lock);
        static Sub* sub_cache = nullptr;
        static std::vector<float> data_x;
        static std::vector<float> data_y_1;
        static std::vector<float> data_y_10;
        static std::vector<float> data_y_25;
        static std::vector<float> data_y_50;
        if (sub_cache == nullptr || sub->get_id() != sub_cache->get_id()) { // ReCache Werte
            data_x.clear(); data_y_1.clear(); data_y_10.clear(); data_y_25.clear(); data_y_50.clear();
            for (int v = 0; v <= x_max; v++) {
                data_x.push_back(v);
                data_y_1.push_back(Physik::sichtbarkeit(sub->get_noise(), static_cast<float>(v), 1000));
                data_y_10.push_back(Physik::sichtbarkeit(sub->get_noise(), static_cast<float>(v), 10000));
                data_y_25.push_back(Physik::sichtbarkeit(sub->get_noise(), static_cast<float>(v), 25000));
                data_y_50.push_back(Physik::sichtbarkeit(sub->get_noise(), static_cast<float>(v), 50000));
            }
        }
        if (mark_v) ImPlot::TagX(mark_v.value(), ImColor(0xFF, 0xFF, 0xFF), "%.1f", mark_v.value());
        ImPlot::PlotLine("@ 1km", data_x.data(), data_y_1.data(), data_x.size());
        ImPlot::PlotLine("@ 10km", data_x.data(), data_y_10.data(), data_x.size());
        ImPlot::PlotLine("@ 25km", data_x.data(), data_y_25.data(), data_x.size());
        ImPlot::PlotLine("@ 50km", data_x.data(), data_y_50.data(), data_x.size());
        ImPlot::EndPlot();
    }
    ImGui::End();
}

void Nav_UI::draw_gfx(const Sub* sub, sf::RenderWindow* window) {
    // Lichter + Schalter
    bg->draw(window);
    schalter_vent_air->draw(window, 0); // TODO
    schalter_emergency_surface->draw(window, emergency_surface ? 1 : 0); // TODO
    lampe_air_available->draw(window, sub->get_pos().z() >= sub->get_schnorcheltiefe());
    lampe_emergency_surface_aktiv->draw(window, emergency_surface ? 1 : 0); // TODO

    // LCD Text Geschwindigkeit + Tiefe
    static const sf::Font lcd_font = std::invoke([]() { sf::Font f; f.loadFromFile(ui::FILE_FONT_LCD); return f; });
    static sf::Text text_speed = std::invoke([]() {
        sf::Text t; t.setFont(lcd_font); t.setPosition(940, 76); t.setFillColor(sf::Color::Red); t.setCharacterSize(48); return t;
    });
    static sf::Text text_depth = std::invoke([]() {
        sf::Text t; t.setFont(lcd_font); t.setPosition(940, 184); t.setFillColor(sf::Color::Red); t.setCharacterSize(48); return t;
    });
    const std::string str_speed = std::invoke([&]() {
        std::string s(std::to_string(Physik::round(sub->get_speed(), 0.1f)));
        s = s.substr(0, s.find('.') + 2);
        return s;
    });
    const std::string str_depth(std::to_string((int)std::round(std::abs(sub->get_pos().z()))));
    text_speed.setString(str_speed);
    text_depth.setString(str_depth);
    window->draw(text_speed);
    window->draw(text_depth);

/*    static sf::Shader shader; // TODO Shader
    if (static Do_Once Q; Q) {
        shader.loadFromFile("data/gfx/shader/lcd_effect.glsl", sf::Shader::Fragment);
        shader.setUniform("res", sf::Vector3f(1920, 1080, 32));
    }
    shader.setUniform("tex2d", sf::Shader::CurrentTexture);
    window->draw(text_speed, &shader);*/

    // Zeichnet Zahlen als Text für Skalenbeschriftungen
    auto text = [&](std::string text, float x, float y, unsigned nachkommastellen = 0) {
        if (nachkommastellen > 0) {
            text = text.substr(0, text.find('.') + nachkommastellen + 1);
            if (text.back() == '0') text.pop_back();
        }
        sf::Text t(text, *ui::get_font(), 12);
        t.setPosition({x,y});
        window->draw(t);
    };

    // Bearing
    static sf::ConvexShape bearing_indicator = std::invoke([]() {
        const float length = 128;
        const float width  = 16;
        sf::ConvexShape cs(5);
        cs.setPoint(0, sf::Vector2f(0, length));
        cs.setPoint(1, sf::Vector2f(0,  width));
        cs.setPoint(2, sf::Vector2f(width / 2,  0));
        cs.setPoint(3, sf::Vector2f(width, width));
        cs.setPoint(4, sf::Vector2f(width, length));
        cs.setOrigin(width / 2, length / 2);
        cs.setPosition(946, 566);
        return cs;
    });
    bearing_indicator.setRotation(sub->get_bearing());
    window->draw(bearing_indicator);

    // Ruder
    static sf::ConvexShape kurs_indicator = std::invoke([]() {
        const float length = 80;
        sf::ConvexShape cs(3);
        cs.setPoint(0, sf::Vector2f(0, length));
        cs.setPoint(1, sf::Vector2f(2, 0));
        cs.setPoint(1, sf::Vector2f(4, length));
        cs.setOrigin(2, length);
        cs.setPosition(821, 936);
        return cs;
    });
    // Skala
    const float kurs_step = sub->get_speed_max_rot() / 3.f;
    text("0", 816, 868);
    text(std::to_string(kurs_step), 775, 880, 1);
    text(std::to_string(kurs_step), 848, 880, 1);
    text(std::to_string(kurs_step*2), 750, 920, 1);
    text(std::to_string(kurs_step*2), 868, 920, 1);
    text(std::to_string(sub->get_speed_max_rot()), 760, 964, 1);
    text(std::to_string(sub->get_speed_max_rot()), 860, 964, 1);
    const float kurs_grad_pro_wert = 90.f / (kurs_step * 2.3f); // 2,3 Steps <=> 90°
    kurs_indicator.setRotation(sub->get_speed_rot() * kurs_grad_pro_wert);
    window->draw(kurs_indicator);

    // Tiefenruder
    const float tiefe_step = sub->get_speed_max_tauch() / 5.f;
    const float tiefe_grad_pro_wert = 90.f / (tiefe_step * 3.25f); // 3,25 steps <=> 90°
    const unsigned nachkommastellen = tiefe_step < 0.1f ? 2 : 1;
    text("0", 1150, 935);
    text(std::to_string(tiefe_step), 1144, 906, nachkommastellen);
    text(std::to_string(tiefe_step), 1144, 960, nachkommastellen);
    text(std::to_string(tiefe_step*2), 1122, 884, nachkommastellen);
    text(std::to_string(tiefe_step*2), 1122, 980, nachkommastellen);
    text(std::to_string(tiefe_step*3), 1092, 870, nachkommastellen);
    text(std::to_string(tiefe_step*3), 1092, 996, nachkommastellen);
    text(std::to_string(tiefe_step*4), 1066, 878, nachkommastellen);
    text(std::to_string(tiefe_step*4), 1066, 992, nachkommastellen);
    text(std::to_string(sub->get_speed_max_tauch()), 1038, 894, nachkommastellen);
    text(std::to_string(sub->get_speed_max_tauch()), 1038, 980, nachkommastellen);
    static sf::ConvexShape tiefen_ruder = std::invoke([]() {
        const float length = 80;
        sf::ConvexShape cs(3);
        cs.setPoint(0, sf::Vector2f(0, length));
        cs.setPoint(1, sf::Vector2f(2, 0));
        cs.setPoint(1, sf::Vector2f(4, length));
        cs.setOrigin(2, length);
        cs.setPosition(1098, 940);
        return cs;
    });
    tiefen_ruder.setRotation(std::fmod(450.f - (sub->get_speed_tauch() * tiefe_grad_pro_wert), 360.f));
    window->draw(tiefen_ruder);

    // O2 Anzeige TODO

}

void Nav_UI::handle(sf::Event* event) {
    (void)event;
}

void Nav_UI::handle_imgui_events(const Sub* sub) {
    (void) sub;
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        const auto& maus_pos = ImGui::GetMousePos();
        if (schalter_emergency_surface->is_inside(maus_pos.x, maus_pos.y)) emergency_surface = !emergency_surface;
    }
}
