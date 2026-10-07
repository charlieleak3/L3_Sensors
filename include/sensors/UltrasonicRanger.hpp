/**
 * @file UltrasonicRanger.hpp
 * @brief Drivers and abstractions for standard pulse-triggered ultrasonic range sensors
 *        (e.g., HC-SR04, HY-SRF05, JSN-SR04T, US-100).
 * 
 * @author Charlie Leak (charlieleak3@gmail.com)
 * 
 * ====================================================================================
 * SUPPORTING HARDWARE VARIANTS ACROSS THE FAMILY
 * ====================================================================================
 *  - HC-SR04   : Standard 4-pin ultrasonic module (VCC, Trig, Echo, GND).
 *  - HY-SRF05  : 5-pin enhanced replacement (supports standard 4-pin or combined 3-pin mode).
 *  - JSN-SR04T : Sealed waterproof ultrasonic distance sensor module (Mode 1 / Pulse mode).
 *  - US-100    : Dual-mode ultrasonic distance sensor (Pulse mode with jumper set).
 * ====================================================================================
 */

#ifndef ULTRASONIC_RANGER_HPP
#define ULTRASONIC_RANGER_HPP

#include <cstdint>

namespace Hardware {

/**
 * @brief Generic driver for pulse-triggered ultrasonic range sensors 
 * (HC-SR04, HY-SRF05, US-100 in pulse mode, JSN-SR04T, etc.).
 */
class UltrasonicRanger {
public:
    // Functional interface abstraction for cross-platform hardware operations
    using DelayMicrosFn = void (*)(uint32_t us);
    using WritePinFn    = void (*)(bool level);
    using PulseInFn     = uint32_t (*)(uint32_t timeoutUs);

    /**
     * @brief Environmental & operational parameters.
     */
    struct Config {
        uint32_t timeout_us = 30000;         ///< Maximum timeout for echo pulse (~5m range)
        float speed_of_sound_m_s = 343.0f;   ///< Default speed of sound at 20°C ambient
    };

    /**
     * @brief Constructor for 4-pin (Trig/Echo) or wrapped 3-pin ultrasonic modules.
     */
    UltrasonicRanger(WritePinFn writeTrig, 
                     PulseInFn readEchoPulse, 
                     DelayMicrosFn delayUs, 
                     Config config = Config())
        : m_writeTrig(writeTrig),
          m_readEchoPulse(readEchoPulse),
          m_delayUs(delayUs),
          m_config(config) {}

    /**
     * @brief Sets ambient temperature to dynamically recalibrate the speed of sound.
     * @param temperatureC Ambient temperature in Celsius.
     */
    void setTemperature(float temperatureC) noexcept {
        // v = 331.3 + (0.606 * T) m/s
        m_config.speed_of_sound_m_s = 331.3f + (0.606f * temperatureC);
    }

    /**
     * @brief Triggers a pulse and measures raw echo time in microseconds.
     * @return Duration in microseconds, or 0 on timeout.
     */
    uint32_t readEchoDurationUs() const {
        if (!m_writeTrig || !m_readEchoPulse || !m_delayUs) {
            return 0;
        }

        // Ensure trigger pin is low before initiating pulse
        m_writeTrig(false);
        m_delayUs(2);

        // Standard 10us HIGH trigger pulse required by HC-SR04 family
        m_writeTrig(true);
        m_delayUs(10);
        m_writeTrig(false);

        // Read returning high pulse on the Echo pin
        return m_readEchoPulse(m_config.timeout_us);
    }

    /**
     * @brief Reads distance from sensor in centimeters.
     * @return Distance in cm, or -1.0f if out-of-bounds/timeout.
     */
    float getDistanceCm() const {
        uint32_t durationUs = readEchoDurationUs();
        if (durationUs == 0) {
            return -1.0f; // Timeout or sensor disconnected
        }

        // Distance = (Speed * Time) / 2 (accounting for round trip)
        // Speed in cm/us = speed_of_sound_m_s / 10000.0f
        float speedCmPerUs = m_config.speed_of_sound_m_s / 10000.0f;
        return (durationUs * speedCmPerUs) / 2.0f;
    }

    /**
     * @brief Reads distance from sensor in inches.
     * @return Distance in inches, or -1.0f if out-of-bounds/timeout.
     */
    float getDistanceInches() const {
        float distanceCm = getDistanceCm();
        if (distanceCm < 0.0f) {
            return -1.0f;
        }
        return distanceCm * 0.393701f;
    }

private:
    WritePinFn m_writeTrig;
    PulseInFn m_readEchoPulse;
    DelayMicrosFn m_delayUs;
    Config m_config;
};

// Convenience type definitions for specific compatible module names
using HCSR04   = UltrasonicRanger;
using HYSRF05  = UltrasonicRanger;
using JSNSR04T = UltrasonicRanger;

} // namespace Hardware

#endif // ULTRASONIC_RANGER_HPP