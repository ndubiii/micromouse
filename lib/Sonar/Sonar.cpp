#include "Sonar.h"

// -------------------- Constructor --------------------
Sonar::Sonar(int t, int e) : _trig(t), _echo(e) {}

// -------------------- Initialize pins --------------------
void Sonar::begin()
{
    pinMode(_trig, OUTPUT);
    pinMode(_echo, INPUT);
}

// -------------------- Single measurement --------------------
float Sonar::singleReadingCM()
{
    digitalWrite(_trig, LOW);
    delayMicroseconds(2);
    digitalWrite(_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trig, LOW);

    long duration = pulseIn(_echo, HIGH, 3000); // 30ms timeout
    if (duration == 0)
        return -1; // timeout
    return (duration * 0.034) / 2.0;
}

// -------------------- Smoothed reading --------------------
float Sonar::readDistanceCM()
{
    float sum = 0;
    int valid = 0;
    for (int i = 0; i < _samples; i++)
    {
        float d = singleReadingCM();
        if (d >= 0)
        {
            sum += d;
            valid++;
        }
        delay(5);
    }
    return valid > 0 ? sum / valid : -1; // average of valid readings
}