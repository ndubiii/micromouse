#ifndef ULTRASONIC_H
#define ULTRASONIC_H

class Ultrasonic {
public:
    static void init();
    static void read();
    static float getDistance();
};

#endif