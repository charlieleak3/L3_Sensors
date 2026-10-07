#include <iostream>
#include "UltrasonicRanger.hpp"

// Platform-specific wrapper implementations (e.g., Arduino / STM32 HAL / ESP-IDF)
void hal_write_trig_pin(bool level) {
    // Platform GPIO write implementation
}

uint32_t hal_read_echo_pulse(uint32_t timeoutUs) {
    // Platform microsecond pulse duration measurement
    return 1166; // Simulated echo time (~20 cm distance)
}

void hal_delay_microseconds(uint32_t us) {
    // Platform microsecond delay implementation
}

int main() {
    // Instantiate using universal class or aliased sensor name
    Hardware::UltrasonicRanger distanceSensor(
        hal_write_trig_pin,
        hal_read_echo_pulse,
        hal_delay_microseconds
    );

    // Dynamic temperature compensation setup
    distanceSensor.setTemperature(25.0f); // Set ambient temperature to 25°C

    float distance = distanceSensor.getDistanceCm();
    
    if (distance >= 0.0f) {
        std::cout << "Target Distance: " << distance << " cm" << std::endl;
    } else {
        std::cout << "Measurement Out of Range / Timeout" << std::endl;
    }

    return 0;
}