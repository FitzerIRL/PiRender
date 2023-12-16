#include <gtest/gtest.h>
#include <memory>
#include "piObject.h"

class PiObjectTest : public ::testing::Test {
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

TEST_F(PiObjectTest, DefaultConstructor) {
    piObject obj;

    // Perform assertions to check the default state of the object
    ASSERT_EQ(obj.getSizeW(), 0.0f);
    ASSERT_EQ(obj.getSizeH(), 0.0f);
    // Add more assertions for other members
}

//----------------------------------------------------------------------------------------------------

TEST_F(PiObjectTest, SetSize) {
    piObject obj;

    obj.setSize(2.0f, 3.0f);

    // Perform assertions to check if SetSize method works as expected
    ASSERT_EQ(obj.getSizeW(), 2.0f);
    ASSERT_EQ(obj.getSizeH(), 3.0f);
}

//----------------------------------------------------------------------------------------------------
