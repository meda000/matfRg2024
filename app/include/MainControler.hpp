//
// Created by matfrg on 10/28/25.
//

#ifndef MAINCONTROLER_HPP
#define MAINCONTROLER_HPP

#include <engine/core/Controller.hpp>

namespace app {
class MainControler : public engine::core::Controller {
    void initialize() override;
    bool loop() override;
    void draw_island();
    void update() override;
    void update_camera();
    void begin_draw() override;
    void draw() override;
    void end_draw() override;
public:
    std::string_view name() const override {
      return "app:MainControler";
    };
};
}
#endif //MAINCONTROLER_HPP
