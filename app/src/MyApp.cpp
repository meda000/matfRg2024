//
// Created by matfrg on 10/28/25.
//
#include "MyApp.hpp"

#include "GuiController.hpp"
#include "MainController.hpp"
#include <spdlog/spdlog.h>
#include "StatueController.hpp"

namespace app {
        void MyApp::app_setup() {
            spdlog::info("App setup");
            auto main_controller = register_controller<app::MainController>();
            auto gui_controller = register_controller<app::GuiController>();
            auto statue_controller = register_controller<app::StatueController>();
            main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
            main_controller->before(gui_controller);
            main_controller->before(statue_controller);
        }
}
