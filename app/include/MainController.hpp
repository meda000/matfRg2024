//
// Created by matfrg on 10/28/25.
//

#ifndef MAINCONTROLER_HPP
#define MAINCONTROLER_HPP

#include <engine/core/Controller.hpp>
#include <engine/graphics/Framebuffer.hpp>

namespace app {
class MainController : public engine::core::Controller {
    void initialize() override;
    bool loop() override;
    void draw_island();
    void draw_tree();
    void draw_lamp();
    void draw_skybox();
    void update() override;
    void update_camera();
    void update_spotlight_color();
    void setup_lighting();
    void update_pp();
    void begin_draw() override;
    void draw() override;
    void end_draw() override;
    //void update_spotlight();
    void draw_statue();
    //void update_statue();
    void toggle_postprocessing();


public:
    std::string_view name() const override {
      return "app:MainControler";
    }
private:
    //bool spotlightEnabled = false;
    engine::graphics::Framebuffer fb;

    float spotlightBlueComponent = 0.1f;
    float spotlightRedComponentAmb = 0.1f;
    float spotlightRedComponentDif = 0.1f;
    bool postProcessingEnabled = false;
    //bool statueMove = false;
    //float statueOffset = 0.0f;
    //float statueMoveTimer = 0.0f;
};
}
#endif //MAINCONTROLER_HPP
