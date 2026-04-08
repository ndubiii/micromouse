#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

class MotorDriver {
public:
    static void init();
    static void setSpeed(int left, int right); // -255 to 255
};

#endif