
#include <ostream>

#include <gtest/gtest.h>
#include <piSpritesheet.h>

class PiSpritesheetTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any common resources for the tests.
    }

    void TearDown() override {
        // Clean up any common resources used by the tests.
    }

    piSpritesheet spritesheet;
};

//----------------------------------------------------------------------------------------------------

TEST_F(PiSpritesheetTest, DefaultConstructor) {
    EXPECT_EQ(spritesheet.getSprites().size(), 0);
    EXPECT_EQ(spritesheet.getTexture(), nullptr);
}

//----------------------------------------------------------------------------------------------------

TEST_F(PiSpritesheetTest, ReadFromFile) {
    std::string path = "spritesheet.json";
    ASSERT_TRUE(spritesheet.read(path));
    EXPECT_GT(spritesheet.getSprites().size(), 0);
    EXPECT_NE(spritesheet.getTexture(), nullptr);
    EXPECT_EQ(spritesheet.getW(), spritesheet.getTexture()->width());
    EXPECT_EQ(spritesheet.getH(), spritesheet.getTexture()->height());
}

//----------------------------------------------------------------------------------------------------

TEST_F(PiSpritesheetTest, GetSprite) {
    std::string path = "spritesheet.json";
    ASSERT_TRUE(spritesheet.read(path));

    std::string spriteName = "ball.png";
    piSpritePtr_t sprite = spritesheet.getSprite(spriteName);

    EXPECT_NE(sprite, nullptr);
    EXPECT_EQ(sprite->name, spriteName);
}

//----------------------------------------------------------------------------------------------------

TEST_F(PiSpritesheetTest, GetNonexistentSprite) {
    std::string path = "spritesheet.json";
    ASSERT_TRUE(spritesheet.read(path));

    std::string nonexistentSpriteName = "nonexistentSprite";
    piSpritePtr_t sprite = spritesheet.getSprite(nonexistentSpriteName);
    EXPECT_EQ(sprite, nullptr);
}

//----------------------------------------------------------------------------------------------------

// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
