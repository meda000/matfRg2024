//
// Created by matfrg on 10/28/25.
//
#include "MainControler.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "spdlog/spdlog.h"

#include <engine/graphics/GraphicsController.hpp>

namespace app {
   void MainControler::initialize() {
    engine::graphics::OpenGL::enable_depth_testing();
}


bool MainControler::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if(platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }
void MainControler::draw_island() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
       auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

       engine::resources::Model *islandModel = resources->model("ostrvo");
       engine::resources::Shader *shader = resources->shader("basic");

       shader->use();
       shader->set_mat4("projection", graphics->projection_matrix());
       shader->set_mat4("view", graphics->camera()->view_matrix());
       glm::mat4 modelIsland = glm::mat4(1.0f);
       modelIsland = glm::translate(modelIsland, glm::vec3(0.0f,0.0f,-3.0f));
       modelIsland = glm::rotate(modelIsland, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
       modelIsland = glm::scale(modelIsland, glm::vec3(0.001f));
       shader->set_mat4("model", modelIsland);
       islandModel->draw(shader);
    }
void MainControler::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }
void MainControler::draw() {
        draw_island();
    }
void MainControler::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
       platform->swap_buffers();
    }
}