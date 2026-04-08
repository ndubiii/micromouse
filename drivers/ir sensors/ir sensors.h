#ifndef IR_SENSORS_H
#define IR_SENSORS_H

class IRSensors {
public:
    static void init();
    static void read();
    static int getLeft();
    static int getRight();
};

#endif