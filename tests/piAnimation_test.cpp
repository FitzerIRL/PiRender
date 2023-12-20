#include "gtest/gtest.h"

#include <piObject.h>
#include <piAnimation.h>

// Mockup class for piObjectPtr_t
class MockObject : public piObject {
public:
    virtual void draw() override {}
    virtual void update(glm::mat4 &projection, float time_secs = 0.0f) override {}
};

//----------------------------------------------------------------------------------------------------

// Test fixture class
class AnimatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize common setup for tests if needed
    }

    void TearDown() override {
        // Clean up common resources after tests if needed
    }
};

//----------------------------------------------------------------------------------------------------

// Basic tests for piEasingFunctions
TEST_F(AnimatorTest, EasingFunctions) {
    EXPECT_EQ(piEasingFunctions::Linear(0.5f), 0.5f);
    EXPECT_EQ(piEasingFunctions::QuadraticIn(0.5f), 0.25f);
    // Add more tests for other easing functions
}

//----------------------------------------------------------------------------------------------------

// Basic tests for piAnimator
TEST_F(AnimatorTest, Animator) {
    auto mockObject = std::make_shared<MockObject>();
    glm::vec2 value(0.0f, 0.0f);
    glm::vec2 endValue(1.0f, 1.0f);

    auto animator = piAnimator::create(mockObject, value, endValue, 1.0f);

    EXPECT_FALSE(animator->isComplete());

    // Run some updates to simulate animation progress
    animator->update(0.5f);

    // Add more tests based on the expected behavior of piAnimator
}

//----------------------------------------------------------------------------------------------------

// Basic tests for piObject
TEST_F(AnimatorTest, Object) {
    MockObject obj;

    obj.setSize(2.0f, 3.0f);
    EXPECT_EQ(obj.getSize().x, 2.0f);
    EXPECT_EQ(obj.getSize().y, 3.0f);

    // Add more tests based on the expected behavior of piObject
}


//----------------------------------------------------------------------------------------------------

//
// int main(int argc, char** argv)
//   ::testing::InitGoogleTest(&argc, argv)
//   return RUN_ALL_TESTS();
// }
//