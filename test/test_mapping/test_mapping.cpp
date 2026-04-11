#include <Arduino.h>
#include <unity.h>
#include "SPIFFS.h"

#include "Mapper.h"
#include "IRScanner.h"
#include "Sonar.h"
#include "../test_common/unity_test_runner.h"

//--Fake Sensors
class FakeIr : public IRScanner
{
public:
    float value;
    FakeIr(float v = 200) : IRScanner(0), value(v) {}
    float getDistanceMM() override { return value; }
};

class FakeSonar : public Sonar
{
public:
    float value;
    FakeSonar(float v = 30) : Sonar(0, 1), value(v) {}
    float readDistanceCM() override { return value; }
};

//--TEST
void test_mapper_build_and_save_maze()
{
    // FILESYSTEM
    TEST_ASSERT_TRUE(SPIFFS.begin(true));

    // Fake sensors
    FakeIr leftIR(50);
    FakeIr rightIR(50);
    FakeSonar front(5);

    //--Mapper
    Mapper mapper(leftIR, rightIR, front);

    //--movements
    mapper.updateWalls();

    // forward
    front.value = 200;
    mapper.updateWalls(); // (0,0)

    mapper.turnRight();
    mapper.moveForward(); // (1,0)
    mapper.updateWalls();

    mapper.turnLeft();
    mapper.moveForward(); // (1,1)
    mapper.updateWalls();
    // save
    mapper.saveMazeToFile();

    // file
    File f = SPIFFS.open("/maze.txt");
    TEST_ASSERT_TRUE(f);

    Serial.println("\n--SAVED MAZE---");
    while (f.available())
    {
        Serial.write(f.read());
    }
    f.close();

    // Basics
    TEST_ASSERT_TRUE(mapper.hasWall(0, 0, 0));
    TEST_ASSERT_TRUE(mapper.hasWall(0, 0, 3));
}

void run_all_tests()
{
    RUN_TEST(test_mapper_build_and_save_maze);
}
void setup()
{
    Serial.begin(115200);
    delay(2000);
    TestFunc tests[] = {run_all_tests};
    runUnityTests(tests, 1);
}

void loop() {}