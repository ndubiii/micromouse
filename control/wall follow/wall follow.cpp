#include "wall_follow.h"
#include "ir_sensors.h"
#include "ultrasonic.h"

void WallFollow::init() {}

void WallFollow::update() {
    int left = IRSensors::getLeft();
    int right = IRSensors::getRight();
    float front = Ultrasonic::getDistance();

    // TODO:
    // Combine IR (side) + ultrasonic (front)
    // Example:
    // - Use IR for centering
    // - Use ultrasonic to detect front wall

}