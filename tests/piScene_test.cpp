#include <gtest/gtest.h>
#include <memory>
#include <piScene.h>

//----------------------------------------------------------------------------------------------------

// Mock piObject class for testing
class MockPiObject : public piObject {
public:
    MockPiObject() : piObject() {}
    // Add any necessary mock methods or behaviors here
};

//----------------------------------------------------------------------------------------------------

TEST(PiSceneTest, AddObjectByReference) {
    piScene scene;
    MockPiObject mockObject;

    scene.addObject(mockObject);

    EXPECT_EQ(scene.getSize(), 1);
}

//----------------------------------------------------------------------------------------------------

TEST(PiSceneTest, AddObjectByPointer) {
    piScene scene;
    auto mockObject = std::make_shared<MockPiObject>();

    scene.addObject(mockObject);

    EXPECT_EQ(scene.getSize(), 1);
}

//----------------------------------------------------------------------------------------------------

TEST(PiSceneTest, RemoveObjectByReference) {
    piScene scene;
    MockPiObject mockObject;

    scene.addObject(mockObject);
    scene.removeObject(mockObject);

    EXPECT_EQ(scene.getSize(), 0);
}

//----------------------------------------------------------------------------------------------------

TEST(PiSceneTest, RemoveObjectByPointer) {
    piScene scene;
    auto mockObject = std::make_shared<MockPiObject>();

    scene.addObject(mockObject);
    scene.removeObject(mockObject);

    EXPECT_EQ(scene.getSize(), 0);
}

//----------------------------------------------------------------------------------------------------

TEST(PiSceneTest, UpdateAndDraw) {
    piScene scene;
    auto mockObject = std::make_shared<MockPiObject>();

    scene.addObject(mockObject);

    // Mock projection matrix
    glm::mat4 projection(1.0f);

    // Update and draw without errors
    ASSERT_NO_THROW({
        scene.update(projection, 1.0f);
        scene.draw();
    });
}

//----------------------------------------------------------------------------------------------------

// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
