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

class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};

void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    camera->rotate_camera(position.dx, position.dy);
}


void MainControler::initialize() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
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
       modelIsland = glm::rotate(modelIsland, glm::radians(-135.0f), glm::vec3(1.0f,0.0f,0.0f));
       modelIsland = glm::scale(modelIsland, glm::vec3(0.001f));
       shader->set_mat4("model", modelIsland);
       islandModel->draw(shader);
    }

void MainControler::update_camera() {
       auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
       auto graphic = engine::core::Controller::get<engine::graphics::GraphicsController>();
       auto camera = graphic->camera();
       float dt = platform->dt();

       if(platform->key(engine::platform::KeyId::KEY_W).is_down()) {
           camera->move_camera(engine::graphics::Camera::Movement::FORWARD,dt);
       }
       if(platform->key(engine::platform::KeyId::KEY_S).is_down()) {
           camera->move_camera(engine::graphics::Camera::Movement::BACKWARD,dt);
       }
       if(platform->key(engine::platform::KeyId::KEY_A).is_down()) {
           camera->move_camera(engine::graphics::Camera::Movement::LEFT,dt);
       }
       if(platform->key(engine::platform::KeyId::KEY_D).is_down()) {
           camera->move_camera(engine::graphics::Camera::Movement::RIGHT,dt);
       }

    }


void MainControler::update() {
        update_camera();
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