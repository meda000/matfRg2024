//
// Created by matfrg on 10/29/25.
//

#ifndef STATUECONTROLLER_HPP
#define STATUECONTROLLER_HPP

#include <engine/core/Controller.hpp>

namespace app {
class StatueController : public engine::core::Controller{
public:
    enum class State {
        STILL,
        MOVING,
        WAITING,
        WAITING_AT_END,
        LAMP_OFF
    };
    void initialize() override;
    void poll_events() override;
    void update() override;

    void trigger_movement();
    void toggle_spotlight();

    float get_statue_offset() const {return statueOffset;}
    bool should_draw_statue() const {return currentState != State::STILL;}

    bool is_spotlight_enabled() const {
        return spotlightEnabled;
    }
private:
    State currentState;
    float statueOffset;
    float moveTimer;
    bool spotlightEnabled;
    bool returningToStart = false;

    void update_still();
    void update_moving();
    void update_waiting_at_end();
    void update_waiting();
    void update_lamp_off();
    void transition_to_state(State newState);
};
}


#endif //STATUECONTROLLER_HPP
