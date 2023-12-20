#include <gtest/gtest.h>
#include <memory>
#include "piObject.h"

class piObjectTest : public ::testing::Test {
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

TEST_F(piObjectTest, DefaultConstructor) {
    piObject obj;
    EXPECT_EQ(obj.getSize(), glm::vec2(0.0f, 0.0f));
    EXPECT_EQ(obj.getPos(), glm::vec2(0.0f, 0.0f));
    // Add more assertions for other default values
}

//----------------------------------------------------------------------------------------------------

TEST_F(piObjectTest, SizeFunctions) {
    piObject obj;
    obj.setSize(10.0f, 20.0f);
    EXPECT_EQ(obj.getSize(), glm::vec2(10.0f, 20.0f));
    obj.setSize(glm::vec2(30.0f, 40.0f));
    EXPECT_EQ(obj.getSize(), glm::vec2(30.0f, 40.0f));
    // Add more size-related function tests
}

//----------------------------------------------------------------------------------------------------

TEST_F(piObjectTest, PositionFunctions) {
    piObject obj;
    obj.setPos(5.0f, 10.0f);
    EXPECT_EQ(obj.getPos(), glm::vec2(5.0f, 10.0f));
    obj.setPos(glm::vec2(15.0f, 20.0f));
    EXPECT_EQ(obj.getPos(), glm::vec2(15.0f, 20.0f));
    // Add more position-related function tests
}

//----------------------------------------------------------------------------------------------------

// Add similar tests for Anchor, Scale, Physics, and other parts of the class

TEST_F(piObjectTest, AlphaFunctions) {
    piObject obj;
    obj.setAlpha(0.5f);
    EXPECT_FLOAT_EQ(obj.alpha(), 0.5f);
    // Add more alpha-related function tests
}

//----------------------------------------------------------------------------------------------------

TEST_F(piObjectTest, NameFunctions) {
    piObject obj;
    obj.setName("TestObject");
    EXPECT_EQ(obj.getName(), "TestObject");
    // Add more name-related function tests
}

//----------------------------------------------------------------------------------------------------

TEST_F(piObjectTest, MVPMatrixFunction) {
    piObject obj;
    glm::mat4 identity(1.0f);
    obj.update(identity, 0.0f);
    EXPECT_EQ(obj.getMVPmatrix(), identity);
    // Add more MVP matrix-related function tests
}

//----------------------------------------------------------------------------------------------------
