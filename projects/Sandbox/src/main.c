#include <VampTest/VampTest.h>

void VampRunBeforeAllTests()
{
    printf("Starting Tests...\n\n");
}


void VampRunAfterAllTests()
{
    printf("Finished running tests.\n\n");
}


VAMP_TEST(Suite1, Test123456789)
{
    VAMP_EXPECT(1 == 2, "1 should not be %s", "equal to 2")
}


VAMP_TEST(Suite1, Test2)
{
    VAMP_EXPECT(1 == 1, "1 should not be %s", "equal to true")
}


VAMP_TEST(Suite2, Test1)
{
    VAMP_EXPECT(1 == 1, "1 should not be %s", "equal to true")
}


VAMP_TEST_FRAMEWORK(
    VAMP_REGISTER_TEST(Suite1, Test123456789);
    VAMP_REGISTER_TEST(Suite1, Test2);
    VAMP_REGISTER_TEST(Suite2, Test1);
)