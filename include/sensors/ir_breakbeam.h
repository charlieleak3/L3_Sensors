/**
 * @file ir_breakbeam.h
 * @project Universal Embedded Hardware Drivers (L3_Sensors)
 * @brief Public interface definitions for IR break-beam optical channel drivers.
 */

#ifndef SENSORS_IR_BREAKBEAM_H
#define SENSORS_IR_BREAKBEAM_H

#include <Arduino.h>

/**
 * @brief Logic level representing a unbroken beam condition.
 */
enum class BeamLogic {
    ACTIVE_LOW,  ///< Pin goes LOW when beam is broken
    ACTIVE_HIGH  ///< Pin goes HIGH when beam is broken
};

/**
 * @brief Event edge condition to trigger callback notifications.
 */
enum class TriggerEdge {
    ON_ENTER,    ///< Trigger when beam is first broken
    ON_EXIT,     ///< Trigger when beam is restored/cleared
    BOTH         ///< Trigger on both enter and exit events
};

/**
 * @brief Configuration parameter structure for an IR break-beam channel.
 */
struct IRChannelConfig {
    uint8_t channelId;         ///< Unique channel/sensor index identifier
    uint8_t gpioPin;           ///< Microcontroller GPIO pin connected to sensor output
    uint32_t debounceMicros;   ///< Minimum time in microseconds to filter out chatter
    BeamLogic logic;           ///< Active signal level when beam is broken
    TriggerEdge edge;          ///< Edge transition type that fires trigger events
    bool useInternalPullup;    ///< True to enable internal pull-up resistor (open-collector)
};

/// @brief Event callback function signature passing channel ID and beam broken duration in microseconds.
typedef void (*IRTriggerCallback)(uint8_t channelId, uint32_t durationUs);

/**
 * @brief Driver class handling state detection, software debouncing, and event triggering
 *        for optical slot and reflective pair break-beam sensors.
 */
class IRBreakBeamChannel {
public:
    IRBreakBeamChannel();

    /**
     * @brief Initialize the break-beam channel with pin settings and configurations.
     * @param config Reference to IRChannelConfig structure.
     */
    void begin(const IRChannelConfig& config);

    /**
     * @brief Register a callback function for beam trigger events.
     * @param callback Function pointer matching IRTriggerCallback signature.
     */
    void setTriggerCallback(IRTriggerCallback callback) noexcept {
        m_onTriggerCallback = callback;
    }

    /**
     * @brief Non-blocking state update method to be called periodically in the main loop or timer ISR.
     */
    void update();

    /**
     * @brief Check if the optical beam is currently broken.
     * @return true if beam is broken, false if beam is clear/unbroken.
     */
    bool isBeamBroken() const noexcept { return m_isBroken; }

    /**
     * @brief Get the total number of valid trigger events detected.
     * @return Total event count.
     */
    uint32_t getCount() const noexcept { return m_count; }

    /**
     * @brief Get duration of the most recent beam occlusion in microseconds.
     * @return Duration in microseconds.
     */
    uint32_t getLastDurationUs() const noexcept { return m_lastDurationUs; }

    /**
     * @brief Get timestamp (micros) of the most recent trigger event.
     * @return Microsecond timestamp.
     */
    uint32_t getLastTriggerUs() const noexcept { return m_lastTriggerUs; }

    /**
     * @brief Reset event counter to zero.
     */
    void resetCount() noexcept { m_count = 0; }

private:
    uint8_t m_id;
    uint8_t m_pin;
    uint32_t m_debounceUs;
    BeamLogic m_logic;
    TriggerEdge m_edge;

    bool m_rawState;
    bool m_isBroken;
    uint32_t m_lastStateChangeUs;
    uint32_t m_beamBreakStartUs;

    uint32_t m_lastTriggerUs;
    uint32_t m_lastDurationUs;
    uint32_t m_count;

    IRTriggerCallback m_onTriggerCallback;

    /**
     * @brief Helper function evaluating whether a raw GPIO level corresponds to a broken beam.
     */
    bool evaluateBrokenState(bool rawPinLevel) const noexcept;
};

#endif // SENSORS_IR_BREAKBEAM_H