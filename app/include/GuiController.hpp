//
// Created by matfrg on 10/29/25.
//

#ifndef GUICONTROLLER_HPP
#define GUICONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {
class GuiController : public engine::core::Controller{
public:
    std::string_view name() const override {
        return "app::GuiController";
    }
private:
    void initialize() override;;
    void poll_events() override;
    void draw() override;
};
}

#endif //GUICONTROLLER_HPP
