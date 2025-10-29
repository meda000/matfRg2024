//
// Created by matfrg on 10/28/25.
//
#include "MyApp.hpp"
#include "MainControler.hpp"
#include<spdlog/spdlog.h>

namespace app {
        void MyApp::app_setup() {
            spdlog::info("App setup");
            auto main_controller = register_controller<app::MainControler>();
            main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
        }
}
