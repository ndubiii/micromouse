#ifndef ENCODER_H
#define ENCODER_H

class Encoder {
public:
    static void init();
    static void update();
    static long getLeftTicks();
    static long getRightTicks();
};

#endif