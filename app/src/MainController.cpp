//
// Created by matfrg on 10/28/25.
//
#include "MainController.hpp"

#include "../../engine/libs/assimp/code/AssetLib/3MF/3MFXmlTags.h"
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

    setup_lighting();

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

setup_lighting();

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

    setup_lighting();

    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());

    glm::mat4 modelLampa = glm::mat4(1.0f);
    modelLampa =glm::translate(modelLampa, glm::vec3(-3.0f,0.3f,-1.0f));
    modelLampa = glm::rotate(modelLampa, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
    modelLampa = glm::scale(modelLampa, glm::vec3(0.5f));
    shader->set_mat4("model", modelLampa);
    lampModel->draw(shader);
    }

void MainController::draw_skybox() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto skybox = resources->skybox("pozadina");
        auto shader = resources->shader("skybox");
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        graphics->draw_skybox(shader, skybox);

    }

void MainController::draw_statue() {
        if(!spotlightEnabled) {
           return;
        }
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Model *statuaModel = resources->model("statua");
        engine::resources::Shader *shader = resources->shader("basic");

        shader->use();

        setup_lighting();

        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::mat4 modelStatua = glm::mat4(1.0f);
        modelStatua =glm::translate(modelStatua, glm::vec3(-5.0f + statueOffset,0.3f,-4.5f));
        //modelLampa = glm::rotate(modelLampa, glm::radians(-90.0f), glm::vec3(1.0f,0.0f,0.0f));
        modelStatua = glm::scale(modelStatua, glm::vec3(0.2f));
        shader->set_mat4("model", modelStatua);
        statuaModel->draw(shader);
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
void MainController::update_spotlight() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if(platform->key(engine::platform::KeyId::KEY_L).state() == engine::platform::Key::State::JustPressed) {
            spotlightEnabled = !spotlightEnabled;

            if(!spotlightEnabled) {
                statueOffset = 0.0f;
                statueMove = false;
                statueMoveTimer = 0.0f;
            }
        }
    }
void MainController::update_spotlight_color() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if(platform->key(engine::platform::KeyId::KEY_B).state() == engine::platform::Key::State::JustPressed) {
            spotlightBlueComponent += 0.1f;
            if(spotlightBlueComponent > 3.0f) {
                spotlightBlueComponent = 0.2f;
            }
        }
    if(platform->key(engine::platform::KeyId::KEY_R).state() == engine::platform::Key::State::JustPressed) {
        spotlightRedComponentAmb += 0.1f;
        spotlightRedComponentDif += 0.1f;
        if(spotlightRedComponentAmb > 3.0f || spotlightRedComponentDif > 3.0f) {
            spotlightRedComponentAmb = 0.2f;
            spotlightRedComponentDif = 0.2f;
        }
        statueMove = true;
        statueMoveTimer = 0.0f;
    }
    }

void MainController::update_statue() {
        if(statueMove) {
            auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
            statueMoveTimer += platform->dt();

            if(statueMoveTimer > 1.0f) {
                statueOffset += 0.07f;
                statueMove = false;
            }
        }
    }


void MainController::update() {
        update_camera();
        update_spotlight_color();
        update_spotlight();
        update_statue();
    }


void MainController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }
void MainController::setup_lighting() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();

    engine::resources::Shader *modelShader = resources->shader("basic");

    modelShader->use();

    modelShader->set_vec3("dirLight.direction", glm::vec3(0.8f,-1.0f,1.0f));
    modelShader->set_vec3("dirLight.ambient", glm::vec3(0.3f,0.3f,0.3f)*2.0f);
    modelShader->set_vec3("dirLight.diffuse", glm::vec3(0.4f,0.4f,0.4f)*0.5f);
    modelShader->set_vec3("dirLight.specular", glm::vec3(0.8f,0.3f,0.3f));

    modelShader->set_vec3("spotLight.position", glm::vec3(-1.0f,6.0f,-4.0f));
    modelShader->set_vec3("spotLight.direction", glm::vec3(-0.6f,-1.0f,0.0f));
    modelShader->set_float("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    modelShader->set_float("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

    modelShader->set_float("spotLight.constant", 1.0f);
    modelShader->set_float("spotLight.linear", 0.045f);
    modelShader->set_float("spotLight.quadratic", 0.0075f);
    if(spotlightEnabled) {
        modelShader->set_vec3("spotLight.ambient", glm::vec3(spotlightRedComponentAmb,0.2f,spotlightBlueComponent));
        modelShader->set_vec3("spotLight.diffuse", glm::vec3(spotlightRedComponentDif,1.0f,spotlightBlueComponent));
        modelShader->set_vec3("spotLight.specular", glm::vec3(1.0f,1.0f,spotlightBlueComponent));
    } else {
        modelShader->set_vec3("spotLight.ambient", glm::vec3(0.0f));
        modelShader->set_vec3("spotLight.diffuse", glm::vec3(0.0f));
        modelShader->set_vec3("spotLight.specular", glm::vec3(0.0f));
        spotlightRedComponentAmb = 0.1f;
        spotlightRedComponentDif = 0.1f;
    }
    modelShader->set_float("material_shininess", 30.0f);
    modelShader->set_vec3("viewPos", graphics->camera()->Position);

}

void MainController::draw() {

        //setup_lighting();
        draw_island();
        draw_tree();
        draw_lamp();
        draw_statue();
        draw_skybox();
    }
void MainController::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
       platform->swap_buffers();
    }
}