#include "encoder.h"

static long leftTicks = 0;
static long rightTicks = 0;

void Encoder::init() {
    // TODO: attach interrupts
}

void Encoder::update() {
    // TODO: compute speed if needed
}

long Encoder::getLeftTicks() { return leftTicks; }
long Encoder::getRightTicks() { return rightTicks; }