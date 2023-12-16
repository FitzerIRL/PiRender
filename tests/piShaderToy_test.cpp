#include <gtest/gtest.h>
#include <piShaderToy.h>

//----------------------------------------------------------------------------------------------------

class PiShaderToyTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Any setup code specific to the tests
    }

    void TearDown() override
    {
        // Any teardown code specific to the tests
    }

    // You can add more helper methods or variables specific to the tests
};

//----------------------------------------------------------------------------------------------------
TEST_F(PiShaderToyTest, Constructor)
{
    piShaderToy shaderToy;
    // Add assertions to test the constructor, e.g., shaderProgram_ should not be 0
    EXPECT_NE(shaderToy.getProgram(), 0);
}

//----------------------------------------------------------------------------------------------------
TEST_F(PiShaderToyTest, Update)
{
    piShaderToy shaderToy;
    glm::mat4 projection(1.0f); // You may need to initialize this with a valid projection matrix
    float time_secs = 1.0f;
    shaderToy.update(projection, time_secs);
    // Add assertions to test the update method
    // For example, you can check if iResolutionLoc has been set correctly
}

// Add more tests as needed

// int main(int argc, char **argv)
// {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
