#include <gtest/gtest.h>

#include <GLFW/glfw3.h>

#include <piImage.h>

//----------------------------------------------------------------------------------------------------

// Test fixture for piImage tests
class piImageTest : public ::testing::Test
{
protected:
    static void SetUpTestCase() {
        // Code to run before the first test case
        // Initialize common resources or perform setup
        std::cout << "Setting up the test suite" << std::endl;


    #define RES_W  1280.0f
    #define RES_H   720.0f

        if (!glfwInit())
        {
            fprintf(stderr, "Failed to initialize GLFW\n");
            return;// EXIT_FAILURE;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        // Create a GLFW window
        GLFWwindow* window = glfwCreateWindow(RES_W, RES_H, "OpenGL ES 2.0 Example", nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;// EXIT_FAILURE;
        }

        // Make the window's context current
        glfwMakeContextCurrent(window);
        glfwSetWindowPos(window, 2500, 750);
        glfwSwapInterval(1); // 1 for vsync, 0 for no vsync

    }

    static void TearDownTestCase() {
        // Code to run after the last test case
        // Clean up resources or perform teardown
        std::cout << "Tearing down the test suite" << std::endl;
    }

protected:
    void SetUp() override {
        // Optional: Setup code that will be called before each test
    }

    void TearDown() override {
        // Optional: Teardown code that will be called after each test
    }
};

//----------------------------------------------------------------------------------------------------

// Example tests for piImage class
TEST_F(piImageTest, ConstructorTest) {
    piImagePtr_t image = piImage::create();  // Use your factory functions for testing
    ASSERT_TRUE(image != nullptr);
    // Add more assertions as needed
}

//----------------------------------------------------------------------------------------------------

TEST_F(piImageTest, DrawTest) {
    piImagePtr_t image = piImage::create();  // Create an instance for testing
    ASSERT_NO_THROW(image->draw());  // Example assertion, modify based on your class behavior
    // Add more assertions as needed
}

//----------------------------------------------------------------------------------------------------

// Add more tests for other functionalities of piImage

// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
