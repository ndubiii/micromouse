#ifndef IMU_H
#define IMU_H

class IMU {
public:
    static void init();
    static void update();
    static float getAngle();
};

#endif