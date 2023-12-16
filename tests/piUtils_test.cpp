#include "piUtils.h"
#include "gtest/gtest.h"

//----------------------------------------------------------------------------------------------------

// // Mocking checkGLError for testing purposes
// void checkGLError(const char* operation) {
//     // Mock implementation or leave it empty for testing
// }


//======================================================================================================

// Vertex shader source code
static const char* sourceVertex =

R"(
    attribute vec3 aPosition;
    attribute vec4 aColor;
    attribute vec2 aTexCoord;

    uniform mat4  u_mvpMatrix;
    varying vec4  v_Color;
    varying vec2  v_TexCoord;

    void main()
    {
        gl_Position = u_mvpMatrix * vec4(aPosition.xyz, 1.0);

        v_Color    = aColor;
        v_TexCoord = aTexCoord;

        // v_TexCoord  = (aPosition * 0.5 + 0.5) * vec2(1.0, -1.0); // Flip y-axis
    }
)";

// Fragment shader source code
static const char* sourceFragment = R"(

    varying vec4 v_Color;
    varying vec2 v_TexCoord;

    uniform sampler2D textureSampler;
    uniform float alpha;

    void main()
    {
        // Apply the tint only if the alpha value of v_Color is greater than zero
   //     vec4 color   = texture2D(textureSampler, v_TexCoord);
    //    gl_FragColor = mix(color, color + v_Color, v_Color.a * color.a); //step(0.0, v_Color.a * color.a));



    vec4 originalColor = texture2D(textureSampler, v_TexCoord);
    vec4 tintedColor = vec4(v_Color.rgb, 1.0);

    // Blend between the original color and the tinted color based on v_Color.a
    gl_FragColor = mix(originalColor, tintedColor, originalColor.a > 0.0 ? v_Color.a : 0.0);

// gl_FragColor = vec4(1.0, 0.0, 0.0,  1.0);
        // vec4 color   = texture2D(textureSampler, v_TexCoord);
        // gl_FragColor = vec4(color.rgb, color.a * alpha) * vec4(1.0,0.0,0.0, v_Color.a);
    }
)";

//----------------------------------------------------------------------------------------------------

class piUtilsTest : public ::testing::Test {
protected:
    // Member variables for test setup
    GLuint vertexShader;
    GLuint fragmentShader;

    void SetUp() override {
        vertexShader   = piUtils::createShader(GL_VERTEX_SHADER, sourceVertex);
        fragmentShader = piUtils::createShader(GL_FRAGMENT_SHADER, sourceFragment);
    }

    void TearDown() override {
        // Clean up resources after tests
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
};

//----------------------------------------------------------------------------------------------------

TEST_F(piUtilsTest, CreateShader) {
    // Test createShader function
    const char* source = sourceVertex;
    GLuint shader = piUtils::createShader(GL_VERTEX_SHADER, source);
    EXPECT_NE(shader, 0u);
    checkGLError("piUtilsTest::CreateShader");
    glDeleteShader(shader);
}

//----------------------------------------------------------------------------------------------------

TEST_F(piUtilsTest, CreateProgram) {
    // Test createProgram function
    GLuint program = piUtils::createProgram(vertexShader, fragmentShader);
    EXPECT_NE(program, 0u);
    checkGLError("piUtilsTest::CreateProgram");
    glDeleteProgram(program);
}

//----------------------------------------------------------------------------------------------------

TEST_F(piUtilsTest, CreateProgramFromFiles) {
    // Test createProgramFromFiles function
    const char* vertexPath = "vertexShader.glsl";
    const char* fragmentPath = "fragmentShader.glsl";
    GLuint program = piUtils::createProgramFromFiles(vertexPath, fragmentPath);
    EXPECT_NE(program, 0u);
    checkGLError("piUtilsTest::CreateProgramFromFiles");
    glDeleteProgram(program);
}

//----------------------------------------------------------------------------------------------------

TEST_F(piUtilsTest, CreateProgramFromSource) {
    // Test createProgramFromSource function
    // const char* vertexSource = "your_vertex_shader_source";
    // const char* fragmentSource = "your_fragment_shader_source";
    GLuint program = piUtils::createProgramFromSource(sourceVertex, sourceFragment);
    EXPECT_NE(program, 0u);
    checkGLError("piUtilsTest::CreateProgramFromSource");
    glDeleteProgram(program);
}

//----------------------------------------------------------------------------------------------------

// Add more test cases as needed for other functions in piUtils

// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
