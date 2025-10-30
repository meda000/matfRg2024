//
// Created by matfrg on 10/29/25.
//

#include "StatueController.hpp"
#include <engine/core/Controller.hpp>
#include "engine/platform/PlatformController.hpp"
#include "spdlog/spdlog.h"

namespace app {
void StatueController::initialize() {
    transition_to_state(State::STILL);
}
void StatueController::poll_events() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if(platform->key(engine::platform::KeyId::KEY_M).state() == engine::platform::Key::State::JustPressed && spotlightEnabled) {
        trigger_movement();
    }
    if(platform->key(engine::platform::KeyId::KEY_L).state() == engine::platform::Key::State::JustPressed) {
        toggle_spotlight();
    }
}

void StatueController::update() {
    switch (currentState) {
        case State::STILL:
            update_still();
        break;
        case State::MOVING:
            update_moving();
        break;
        case State::WAITING:
            update_waiting();
        break;
        case State::WAITING_AT_END:
            update_waiting_at_end();
        break;
        case State::LAMP_OFF:
            update_lamp_off();
        break;
    }
}

void StatueController::update_still() {

}

void StatueController::update_moving() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    float moveSpeed = 0.6f;

    if(!returningToStart) {
        statueOffset += moveSpeed * platform->dt();
        if(statueOffset >= 1.5) {
            statueOffset = 1.5f;
            transition_to_state(State::WAITING_AT_END);
        }
    } else {
        statueOffset -= moveSpeed * platform->dt();

        if(statueOffset <= 0.0f) {
            statueOffset = 0.0f;
            returningToStart = false;
            transition_to_state(State::WAITING);
        }
    }
}

void StatueController::update_waiting_at_end() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    moveTimer += platform->dt();

    if(moveTimer >= 1.0f) {
        returningToStart = true;
        transition_to_state(State::MOVING);
    }
}


void StatueController::update_waiting() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    moveTimer += platform->dt();

    if(moveTimer >= 2.0f) {
        transition_to_state(State::LAMP_OFF);
    }
}

void StatueController::update_lamp_off(){
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    moveTimer += platform->dt();

    if(moveTimer >= 1.0f) {
        spotlightEnabled = false;
        statueOffset = 0.0f;
        transition_to_state(State::STILL);
    }
}


void StatueController::trigger_movement() {

        if(currentState == State::STILL) {
            spotlightEnabled = true;
            transition_to_state(State::MOVING);
        }


}
void StatueController::toggle_spotlight() {
    if(currentState == State::STILL) {
        spotlightEnabled = !spotlightEnabled;
    } else if (currentState == State::MOVING || currentState == State::WAITING || currentState == State::LAMP_OFF) {
        spotlightEnabled = false;
        statueOffset = 0.0f;
        transition_to_state(State::STILL);
        //spdlog::info("Statue: interrupted, returning");
    }

}
void StatueController::transition_to_state(State newState) {
    currentState = newState;
    moveTimer = 0.0f;

    switch (newState) {
        case State::STILL:
            spdlog::info("Statua -> STILL");
            break;
        case State::MOVING:
            spdlog::info("Statua -> MOVING");
            break;
        case State::WAITING:
            spdlog::info("Statua -> WAITING");
            break;
        case State::WAITING_AT_END:
            spdlog::info("Statue-WAITING_AT_END");
            break;
        case State::LAMP_OFF:
            spdlog::info("Statue -> LAMP_OFF");
            break;
    }
}

}

