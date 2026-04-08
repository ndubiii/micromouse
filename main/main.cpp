#include <Arduino.h>
#include "config.h"
#include "pins.h"

// Drivers
#include "motor_driver.h"
#include "encoder.h"
#include "imu.h"
#include "ir_sensors.h"
#include "ultrasonic.h"

// Control
#include "motion_controller.h"
#include "wall_follow.h"

// Navigation
#include "maze.h"
#include "floodfill.h"

void setup() {
    Serial.begin(115200);

    // Initialize drivers
    MotorDriver::init();
    Encoder::init();
    IMU::init();
    IRSensors::init();
    Ultrasonic::init();

    // Initialize control
    MotionController::init();
    WallFollow::init();

    // Initialize navigation
    Maze::init();

    Serial.println("Micromouse Initialized");
}

void loop() {
    // === SENSOR UPDATE ===
    IRSensors::read();
    Ultrasonic::read();   // Works with IR (front sensing fusion)
    IMU::update();
    Encoder::update();

    // === CONTROL ===
    WallFollow::update();     // Uses IR + ultrasonic
    MotionController::update();

    // === NAVIGATION ===
    // TODO: Call floodfill / planner here

    delay(10); // basic loop timing
}