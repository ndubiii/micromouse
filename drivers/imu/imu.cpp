#include "imu.h"

static float angle = 0;

void IMU::init() {
    // TODO: initialize MPU6050
}

void IMU::update() {
    // TODO: read gyro + apply complementary filter
}

float IMU::getAngle() {
    return angle;
}