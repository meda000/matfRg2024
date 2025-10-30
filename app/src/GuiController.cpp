//
// Created by matfrg on 10/29/25.
//

#include "GuiController.hpp"

#include <engine/graphics/GraphicsController.hpp>
#include <engine/platform/PlatformController.hpp>
#include <imgui.h>

namespace app {
    void GuiController::initialize() {
        set_enable(false);
}

    void GuiController::poll_events() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if(platform->key(engine::platform::KeyId::KEY_I).state() == engine::platform::Key::State::JustPressed) {
            set_enable(!is_enabled());
        }
}


    void GuiController::draw() {
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera = graphics->camera();
        graphics->begin_gui();

        ImGui::Begin("Camera info");
        ImGui::Text("Camera position: (%f,%f,%f)", camera->Position.x, camera->Position.y, camera->Position.z);
        ImGui::Text("(Yaw, Pitch): (%f, %f)", camera->Yaw, camera->Pitch);
        ImGui::Text("camera front: (%f, %f, %f)", camera->Front.x, camera->Front.y, camera->Front.z);

        ImGui::End();
        graphics->end_gui();
}

}