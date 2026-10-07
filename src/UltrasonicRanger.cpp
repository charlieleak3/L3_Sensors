/**
 * @file ir_breakbeam.cpp
 * @project Universal Embedded Hardware Drivers (L3_Sensors)
 * @brief Implementation for decoupled optical break-beam driver channels.
 */

#include <sensors/ir_breakbeam.h>

IRBreakBeamChannel::IRBreakBeamChannel()
    : m_id(0),
      m_pin(0),
      m_debounceUs(150000),
      m_logic(BeamLogic::ACTIVE_HIGH),
      m_edge(TriggerEdge::ON_EXIT),
      m_rawState(false),
      m_isBroken(false),
      m_lastStateChangeUs(0),
      m_beamBreakStartUs(0),
      m_lastTriggerUs(0),
      m_lastDurationUs(0),
      m_count(0),
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

    // Read initial pin state
    bool initialPinLevel = (digitalRead(m_pin) == HIGH);
    m_rawState = initialPinLevel;
    m_isBroken = evaluateBrokenState(initialPinLevel);
    m_lastStateChangeUs = micros();
    m_beamBreakStartUs = m_isBroken ? m_lastStateChangeUs : 0;
}

bool IRBreakBeamChannel::evaluateBrokenState(bool rawPinLevel) const noexcept {
    if (m_logic == BeamLogic::ACTIVE_LOW) {
        return !rawPinLevel; // LOW pin signal indicates beam is broken
    }
    return rawPinLevel;      // HIGH pin signal indicates beam is broken
}

void IRBreakBeamChannel::update() {
    uint32_t nowUs = micros();
    bool currentPinLevel = (digitalRead(m_pin) == HIGH);

    // Filter out signal chatter: restart timer on raw state transitions
    if (currentPinLevel != m_rawState) {
        m_rawState = currentPinLevel;
        m_lastStateChangeUs = nowUs;
        return;
    }

    // Check if stable state duration exceeds debounce window
    if ((nowUs - m_lastStateChangeUs) >= m_debounceUs) {
        bool debouncedBrokenState = evaluateBrokenState(currentPinLevel);

        // State transition detected after debouncing
        if (debouncedBrokenState != m_isBroken) {
            m_isBroken = debouncedBrokenState;
            m_lastTriggerUs = nowUs;

            if (m_isBroken) {
                // Transition: Beam Entered (Unbroken -> Broken)
                m_beamBreakStartUs = nowUs;

                if (m_edge == TriggerEdge::ON_ENTER || m_edge == TriggerEdge::BOTH) {
                    m_count++;
                    if (m_onTriggerCallback) {
                        m_onTriggerCallback(m_id, 0);
                    }
                }
            } else {
                // Transition: Beam Exited (Broken -> Unbroken)
                m_lastDurationUs = nowUs - m_beamBreakStartUs;

                if (m_edge == TriggerEdge::ON_EXIT || m_edge == TriggerEdge::BOTH) {
                    m_count++;
                    if (m_onTriggerCallback) {
                        m_onTriggerCallback(m_id, m_lastDurationUs);
                    }
                }
            }
        }
    }
}