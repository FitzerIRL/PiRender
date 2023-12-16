#include <gtest/gtest.h>

#include "piValue.h"

//----------------------------------------------------------------------------------------------------

class PiValueTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialization code specific to each test
    }

    void TearDown() override {
        // Clean-up code specific to each test
    }

    // Add any member variables or helper functions you might need
};

//----------------------------------------------------------------------------------------------------

TEST_F(PiValueTest, DefaultConstructor) {
    piValue val;

    // Perform assertions to check the default state of the value
    ASSERT_EQ(val.isEmpty(), true);
    ASSERT_EQ(val.getType(), PI_voidType);
}

//----------------------------------------------------------------------------------------------------

TEST_F(PiValueTest, BoolAssignment) {
    piValue val;
    bool expectedValue = true;

    val = expectedValue;

    // Perform assertions to check if Bool assignment works as expected
    ASSERT_EQ(val.isEmpty(), false);
    ASSERT_EQ(val.getType(), PI_boolType);
    ASSERT_EQ(val.toBool(), expectedValue);
}

//----------------------------------------------------------------------------------------------------

TEST_F(PiValueTest, IntAssignment) {
    piValue val;
    int expectedValue = 42;

    val = expectedValue;

    // Perform assertions to check if Int assignment works as expected
    ASSERT_EQ(val.isEmpty(), false);
    ASSERT_EQ(val.getType(), PI_int32_tType);
    ASSERT_EQ(val.toInt32(), expectedValue);
}

//----------------------------------------------------------------------------------------------------

TEST_F(PiValueTest, DoubleAssignment) {
    piValue val;
    double expectedValue = 3.14;

    val = expectedValue;

    // Perform assertions to check if Double assignment works as expected
    ASSERT_EQ(val.isEmpty(), false);
    ASSERT_EQ(val.getType(), PI_doubleType);
    ASSERT_EQ(val.toDouble(), expectedValue);
}

//----------------------------------------------------------------------------------------------------

TEST_F(PiValueTest, Vec3Assignment) {
    piValue val;
    glm::vec3 expectedValue(1.0f, 2.0f, 3.0f);

    val = expectedValue;

    // Perform assertions to check if Vec3 assignment works as expected
    ASSERT_EQ(val.isEmpty(), false);
    ASSERT_EQ(val.getType(), PI_vec3Type);
    ASSERT_EQ(val.toVec3(), expectedValue);
}

//----------------------------------------------------------------------------------------------------

TEST_F(PiValueTest, CopyConstructor) {
    piValue original;
    original = 123;

    piValue copy(original);

    // Perform assertions to check if the copy constructor works as expected
    ASSERT_EQ(copy.isEmpty(), false);
    ASSERT_EQ(copy.getType(), PI_int32_tType);
    ASSERT_EQ(copy.toInt32(), 123);
}

//----------------------------------------------------------------------------------------------------

TEST_F(PiValueTest, MoveConstructor) {
    piValue original;
    original = 456;

    piValue moved(std::move(original));

    // Perform assertions to check if the move constructor works as expected
    ASSERT_EQ(original.isEmpty(), true);  // Original should be moved-from
    ASSERT_EQ(moved.isEmpty(), false);
    ASSERT_EQ(moved.getType(), PI_int32_tType);
    ASSERT_EQ(moved.toInt32(), 456);
}

//----------------------------------------------------------------------------------------------------

TEST_F(PiValueTest, ValueComparison) {
    piValue val1 = 123;
    piValue val2 = 456;

    // Perform assertions to check if value comparison works as expected
    ASSERT_EQ(val1 == val1, true);
    ASSERT_EQ(val1 == val2, false);
    ASSERT_EQ(val1 != val2, true);
}

//----------------------------------------------------------------------------------------------------

TEST_F(PiValueTest, TypeConversion) {
    piValue valInt = 123;
    piValue valDouble = 3.14;

    // Perform assertions to check if type conversion works as expected
    ASSERT_EQ(valInt.getType(), PI_int32_tType);
    ASSERT_EQ(valDouble.getType(), PI_doubleType);

    valInt = valDouble;

    ASSERT_EQ(valInt.getType(), PI_doubleType);
    ASSERT_EQ(valInt.toDouble(), 3.14);
}

//----------------------------------------------------------------------------------------------------

// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
