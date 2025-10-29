//
// Created by matfrg on 10/28/25.
//
#include "MainController.hpp"

#include "GuiController.hpp"
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
    auto gui_controller = engine::core::Controller::get<GuiController>();
    if(!gui_controller->is_enabled()) {
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        camera->rotate_camera(position.dx, position.dy);
    }
}


void MainController::initialize() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
    engine::graphics::OpenGL::enable_depth_testing();

    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    camera->Position = glm::vec3(-9.4f,3.1f,-4.4f);
}


bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if(platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }
void MainController::draw_island() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
       auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

       engine::resources::Model *islandModel = resources->model("ostrvo");
       engine::resources::Shader *shader = resources->shader("basic");

       shader->use();
       shader->set_mat4("projection", graphics->projection_matrix());
       shader->set_mat4("view", graphics->camera()->view_matrix());
       glm::mat4 modelIsland = glm::mat4(1.0f);
       modelIsland = glm::translate(modelIsland, glm::vec3(0.0f,0.0f,-3.0f));
       modelIsland = glm::rotate(modelIsland, glm::radians(0.0f), glm::vec3(1.0f,0.0f,0.0f));
       modelIsland = glm::scale(modelIsland, glm::vec3(0.005f));
       shader->set_mat4("model", modelIsland);
       islandModel->draw(shader);
    }

void MainController::draw_tree() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    engine::resources::Model *treeModel = resources->model("tree");
    engine::resources::Shader *shader = resources->shader("basic");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::mat4 modelDrvo = glm::mat4(1.0f);
    modelDrvo =glm::translate(modelDrvo, glm::vec3(0.0f,0.25f,-3.0f));
    modelDrvo = glm::rotate(modelDrvo, glm::radians(-270.0f), glm::vec3(0.0f,0.0f,1.0f));
    modelDrvo = glm::scale(modelDrvo, glm::vec3(0.9f));
    shader->set_mat4("model", modelDrvo);
    treeModel->draw(shader);
    }

void MainController::draw_lamp() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Model *lampModel = resources->model("lampa");
        engine::resources::Shader *shader = resources->shader("basic");

        shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());

    glm::mat4 modelLampa = glm::mat4(1.0f);
    modelLampa =glm::translate(modelLampa, glm::vec3(-3.0f,0.3f,-1.0f));
    modelLampa = glm::rotate(modelLampa, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
    modelLampa = glm::scale(modelLampa, glm::vec3(0.5f));
    shader->set_mat4("model", modelLampa);
    lampModel->draw(shader);
    }


void MainController::update_camera() {
       auto gui_controller = engine::core::Controller::get<GuiController>();
       if(gui_controller->is_enabled()) {
           return;
       }
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


void MainController::update() {
        update_camera();
    }


void MainController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }
void MainController::draw() {
        draw_island();
        draw_tree();
        draw_lamp();
    }
void MainController::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
       platform->swap_buffers();
    }
}