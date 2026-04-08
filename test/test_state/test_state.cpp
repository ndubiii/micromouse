#include <unity.h>
#include "MouseState.h"

void test_state_transition()
{
    StateMachine sm;
    TEST_ASSERT_EQUAL(STARTING, sm.getCurrentState());

    sm.setState(SEARCHING);
    TEST_ASSERT_EQUAL(SEARCHING, sm.getCurrentState());
}

void setup()
{
    UNITY_BEGIN();
    RUN_TEST(test_state_transition);
    UNITY_END();
}
void loop() {}