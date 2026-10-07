/**
 * @file ir_breakbeam.cpp
 * @project Universal Embedded Hardware Drivers (L3_Sensors)
 * @brief Implementation for decoupled optical break-beam driver channels.
 */

#include <sensors/ir_breakbeam.h>

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