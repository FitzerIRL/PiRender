#include <gtest/gtest.h>
#include <memory>
#include "piTexture.h"

class PiTextureTest : public ::testing::Test {
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

TEST_F(PiTextureTest, DefaultConstructor) {
    piTexture tex;

    // Perform assertions to check the default state of the texture
    ASSERT_EQ(tex.name(), "");
    ASSERT_EQ(tex.texID(), 0);
    ASSERT_EQ(tex.width(), 0);
    ASSERT_EQ(tex.height(), 0);
}

//----------------------------------------------------------------------------------------------------

TEST_F(PiTextureTest, LoadTexture) {
    piTexture tex;
    const char* imagePath = "path/to/your/image.png";

    tex.loadTexture(imagePath);

    // Perform assertions to check if LoadTexture method works as expected
    ASSERT_EQ(tex.name(), imagePath);
    ASSERT_NE(tex.texID(), 0);
    ASSERT_GT(tex.width(), 0);
    ASSERT_GT(tex.height(), 0);
}

//----------------------------------------------------------------------------------------------------
