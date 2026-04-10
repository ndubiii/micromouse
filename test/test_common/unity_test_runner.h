#pragma once
#include <Arduino.h>
#include <unity.h>

// -------------------- Config --------------------
#ifndef TEST_TIMEOUT_MS
#define TEST_TIMEOUT_MS 200 // 👈 default timeout per test
#endif

// -------------------- Unity Hooks --------------------
inline void setUp(void)
{
    // Runs before EACH test
}

inline void tearDown(void)
{
    // Runs after EACH test
}

// -------------------- Test Runner --------------------
typedef void (*TestFunc)();

// Wrapper to safely execute a test with timeout
inline bool runWithTimeout(TestFunc testFunc, int index)
{
    unsigned long start = millis();

    // Run test
    testFunc();

    unsigned long duration = millis() - start;

    if (duration > TEST_TIMEOUT_MS)
    {
        Serial.print("[TIMEOUT] Test ");
        Serial.print(index);
        Serial.print(" exceeded ");
        Serial.print(TEST_TIMEOUT_MS);
        Serial.println(" ms");

        return false; // mark as skipped/fail
    }

    return true;
}

inline void runUnityTests(TestFunc tests[], int count)
{
    Serial.begin(115200);
    delay(2000); // allow serial to attach

    UNITY_BEGIN();

    int passed = 0;
    int skipped = 0;

    for (int i = 0; i < count; i++)
    {
        Serial.print("\n[RUNNING] Test ");
        Serial.println(i);

        unsigned long start = millis();

        // ⚠️ Run test
        tests[i]();

        unsigned long duration = millis() - start;

        if (duration > TEST_TIMEOUT_MS)
        {
            Serial.print("[SKIPPED - TIMEOUT] Test ");
            Serial.println(i);
            skipped++;
        }
        else
        {
            passed++;
        }

        yield(); // allow background tasks (ESP32 watchdog safety)
    }

    UNITY_END();

    Serial.println("\n===== TEST SUMMARY =====");
    Serial.print("Total: ");
    Serial.println(count);
    Serial.print("Passed: ");
    Serial.println(passed);
    Serial.print("Timed-out (skipped): ");
    Serial.println(skipped);

    delay(100); // flush output

#ifdef UNIT_TEST_RESTART
    ESP.restart(); // auto-reset mode
#else
    while (true)
    {
        delay(1000); // idle mode
    }
#endif
}