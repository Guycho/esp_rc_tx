#include "input.h"

using namespace Config;
InputController::InputController() {}
InputController::~InputController() {}
void InputController::init(const InputControllerConfig &config) {
    for (uint8_t i = 0; i < num_buttons; i++) {
        m_buttons[i] = config.buttons[i];
    }
    for (uint8_t i = 0; i < num_potentiometers; i++) {
        m_potentiometers[i] = config.potentiometers[i];
    }
}
void InputController::run() {
    m_data.throttle = m_potentiometers[Config::PotentiometerName::THROTTLE]->get_value();
    m_data.steering = m_potentiometers[Config::PotentiometerName::STEERING]->get_value();
    m_data.left_arrow = m_buttons[Config::ButtonName::LEFT_ARROW]->get_state();
    m_data.right_arrow = m_buttons[Config::ButtonName::RIGHT_ARROW]->get_state();
    m_data.up_arrow = m_buttons[Config::ButtonName::UP_ARROW]->get_state();
    m_data.down_arrow = m_buttons[Config::ButtonName::DOWN_ARROW]->get_state();
    m_data.sel = m_buttons[Config::ButtonName::SEL]->get_state();
    m_data.ch = m_buttons[Config::ButtonName::CH]->get_state();
    m_data.plus = m_buttons[Config::ButtonName::PLUS]->get_state();
    m_data.minus = m_buttons[Config::ButtonName::MINUS]->get_state();
    m_data.left_trim_l = m_buttons[Config::ButtonName::LEFT_TRIM_L]->get_state();
    m_data.left_trim_r = m_buttons[Config::ButtonName::LEFT_TRIM_R]->get_state();
    m_data.right_trim_l = m_buttons[Config::ButtonName::RIGHT_TRIM_L]->get_state();
    m_data.right_trim_r = m_buttons[Config::ButtonName::RIGHT_TRIM_R]->get_state();
    m_data.edge_switch = m_buttons[Config::ButtonName::EDGE_SWITCH]->get_state();
    m_data.bottom_switch = m_buttons[Config::ButtonName::BOTTOM_SWITCH]->get_state();
}
RemoteControllerData InputController::get_data() {
    run();
    return m_data;
}