/**
 * @file ir_breakbeam.h
 * @project Universal Embedded Hardware Drivers (L3_Sensors)
 * @brief Public interface definitions for IR break-beam optical channel drivers.
 */

#ifndef SENSORS_IR_BREAKBEAM_H
#define SENSORS_IR_BREAKBEAM_H

#include <Arduino.h>

enum class BeamLogic {
    ACTIVE_LOW,
    ACTIVE_HIGH
};

enum class TriggerEdge {
    ON_ENTER,
    ON_EXIT,
    BOTH
};

struct IRChannelConfig {
    uint8_t channelId;
    uint8_t gpioPin;
    uint32_t debounceMicros;
    BeamLogic logic;
    TriggerEdge edge;
    bool useInternalPullup;
};

typedef void (*IRTriggerCallback)(uint8_t channelId, uint32_t durationUs);

class IRBreakBeamChannel {
public:
    IRBreakBeamChannel();
    void begin(const IRChannelConfig& config);

private:
    uint8_t m_id;
    uint8_t m_pin;
    uint32_t m_debounceUs;
    BeamLogic m_logic;
    TriggerEdge m_edge;
    uint32_t m_lastTriggerUs;
    uint32_t m_lastDurationUs;
    uint32_t m_count;
    IRTriggerCallback m_onTriggerCallback;
};

#endif // SENSORS_IR_BREAKBEAM_H