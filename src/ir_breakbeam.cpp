/**
 * @file IRBreakBeam.cpp
 * @project Universal Embedded Hardware Drivers
 * @brief Implementation for decoupled optical break-beam driver library.
 */

#include "ir_breakbeam.h"

IRBreakBeamChannel::IRBreakBeamChannel() 
    : m_id(0), m_pin(0), m_debounceUs(150000), 
      m_logic(BeamLogic::ACTIVE_HIGH), m_edge(TriggerEdge::ON_EXIT),
      m_lastTriggerUs(0), m_lastDurationUs(0), m_count(0), 
      m_onTriggerCallback(nullptr) {}

void IRBreakBeamChannel::begin(const IRChannelConfig& config) {
    m_id = config.channelId;
    m_pin = config.gpioPin;
    m_debounceUs = config.debounceMicros;
    m_logic = config.logic;
    m_edge = config.edge;

    if (config.useInternalPullup) {
        pinMode(m_pin, INPUT_PULLUP);
    } else {
        pinMode(m_pin, INPUT);
    }
}

void IRBreakBeamChannel::setTriggerCallback(TimingTriggerCallback callback) {
    m_onTriggerCallback = callback;
}

void IRAM_ATTR IRBreakBeamChannel::handleInterruptISR() {
    uint32_t now = micros();
    
    if (now - m_lastTriggerUs >= m_debounceUs) {
        if (m_lastTriggerUs != 0) { // Ignore startup boot trigger
            m_lastDurationUs = now - m_lastTriggerUs;
            m_count++;

            if (m_onTriggerCallback) {
                m_onTriggerCallback(m_id, m_lastDurationUs, m_count);
            }
        }
        m_lastTriggerUs = now;
    }
}

BeamState IRBreakBeamChannel::pollState() const {
    int pinValue = digitalRead(m_pin);
    if (m_logic == BeamLogic::ACTIVE_HIGH) {
        return (pinValue == HIGH) ? BeamState::BROKEN : BeamState::INTACT;
    } else {
        return (pinValue == LOW) ? BeamState::BROKEN : BeamState::INTACT;
    }
}

void IRBreakBeamChannel::reset() {
    m_lastTriggerUs = 0;
    m_lastDurationUs = 0;
    m_count = 0;
}