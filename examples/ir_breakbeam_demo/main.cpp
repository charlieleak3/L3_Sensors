/**
 * @file main.cpp
 * @project Universal Embedded Hardware Drivers (L3_Sensors)
 * @brief Demo application for testing IR Break-Beam channel driver logic.
 */

#include <Arduino.h>
#include <sensors/ir_breakbeam.h>

constexpr uint8_t IR_SENSOR_PIN = 4;

IRBreakBeamChannel irSensor;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    Serial.println("\n--- IR Break-Beam Sensor Demo ---");

    IRChannelConfig config;
    config.channelId = 1;
    config.gpioPin = IR_SENSOR_PIN;
    config.debounceMicros = 150000;
    config.logic = BeamLogic::ACTIVE_HIGH;
    config.edge = TriggerEdge::ON_EXIT;
    config.useInternalPullup = true;

    irSensor.begin(config);

    Serial.println("IR Break-Beam initialized. Ready for detection...");
}

void loop() {
    delay(10);
}