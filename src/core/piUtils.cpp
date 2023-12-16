#include <piUtils.h>

#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
//======================================================================================================

// Pretty print for glm::vec2
std::ostream& operator<<(std::ostream& os, const glm::vec2& vec) {
    os << "(" << std::fixed << std::setprecision(4) << vec.x << ", "
              << std::fixed << std::setprecision(4) << vec.y << ")";
    return os;
}

// Pretty print for glm::vec3
std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
    os << "(" << std::fixed << std::setprecision(4) << vec.x << ", "
              << std::fixed << std::setprecision(4) << vec.y << ", "
              << std::fixed << std::setprecision(4) << vec.z << ")";
    return os;
}

// Pretty print for glm::vec4
std::ostream& operator<<(std::ostream& os, const glm::vec4& vec) {
    os << "(" << std::fixed << std::setprecision(4) << vec.x << ", "
              << std::fixed << std::setprecision(4) << vec.y << ", "
              << std::fixed << std::setprecision(4) << vec.z << ", " << vec.w << ")";
    return os;
}

// Pretty print for glm::mat4
std::ostream& operator<<(std::ostream& os, const glm::mat4& mat) {

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            os << std::fixed << std::setprecision(4) << mat[j][i] << " ";
        }
        os << std::endl;
    }
    return os;
}

// Pretty print for glm::quat
std::ostream& operator<<(std::ostream& os, const glm::quat& q) {
    os << std::fixed << std::setprecision(4) << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
    return os;
}

// Assuming GLfloat is float, adjust if it's a different type
using GLfloat = float;

// Overload << operator for std::vector<GLfloat>
std::ostream& operator<<(std::ostream& os, const std::vector<GLfloat>& vertices)
{
    const size_t columnCount = 9; // Number of columns per row
    const size_t vertexCount = vertices.size() / columnCount; // Number of vertices

    for (size_t i = 0; i < vertexCount; ++i)
    {
        for (size_t j = 0; j < columnCount; ++j)
        {
            // Set the formatting to show a space for positive values and a '-' sign for negative values
            os << std::showpos << std::setw(5) << std::setprecision(3) << vertices[i * columnCount + j];

            switch(j)
            {
                case 2: os << "      "; break;
                case 6: os << "      "; break;
                case 8: os << "      "; break;
                default:
                    os << " "; break;
            }
            // if (j < columnCount - 1)
            // {
            //     os << "   "; // Adjust the number of spaces as needed
            // }
        }
        os << '\n';
    }

    return os;
}
//======================================================================================================

void analyzeTransformation(const glm::mat4& model, const char* name /* = "" */)
{
    // Print the transformation matrix
    std::cout << std::endl << " ==================== " << name << " ==================== "<< std::endl<< std::endl;
    std::cout << "Transformation Matrix:\n" << model << std::endl;

    // Verify the transformation by applying it to a point (0, 0, 0, 1)
    glm::vec4 pointToTransform = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 transformedPoint = model * pointToTransform;

    // Decompose the matrix
    glm::vec3 translation, scaling, skew;
    glm::quat rotation;
    glm::vec4 perspective;

    glm::decompose(model, scaling, rotation, translation, skew, perspective);

    // Print the results
    std::cout << " Translation: " << translation << std::endl;
    std::cout << "     Scaling: " << scaling     << std::endl;
    std::cout << "        Skew: " << skew        << std::endl;
    std::cout << "    Rotation: " << rotation    << std::endl;
    std::cout << " Perspective: " << perspective << std::endl;

    // Convert quaternion to Euler angles in degrees
    glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(rotation));

    // Print the Euler angles
    std::cout << "Euler Angles (degrees): " << eulerAngles << std::endl;

    // Print the result
    std::cout << std::endl << std::endl;
    std::cout << "    Starter Point: " << pointToTransform << std::endl;
    std::cout << "Transformed Point: " << transformedPoint << std::endl;

    std::cout << std::endl << " ============================================================ "<< std::endl;
    std::cout << std::endl << std::endl;
}

//======================================================================================================

void checkGLError(const char* operation)
{
#ifdef _DEBUG
    for (GLenum error = glGetError(); error != GL_NO_ERROR; error = glGetError())
    {
        const char* errorString = nullptr;
        switch (error)
        {
            case GL_INVALID_ENUM:
                errorString = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                errorString = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                errorString = "GL_INVALID_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                errorString = "GL_OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            default:
                errorString = "Unknown Error";
                break;
        }//SWITCH
        fprintf(stderr, "\nERROR:  OpenGL Error in %s: %s\n\n", operation, errorString);
    }//FOR
#endif
}

//======================================================================================================

/*
If successful, glCreateShader returns a non-zero value, which is a
new unique identifier for the shader object. Otherwise, zero is
returned.
*/
GLuint piUtils::createShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Check shader compilation status
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);

        fprintf(stderr, "ERROR: createShader() ... Shader compilation failed: %s\n", infoLog);
        printf("ERROR: createShader() ... Shader compilation failed: %s\n", infoLog);
        glDeleteShader(shader);

        printf("ERROR: createShader() ... Source: \n === \n %s\n=== \n\n", source);

        return 0;
    }

    return shader;
}

//======================================================================================================

/*static*/ char *piUtils::loadShader(const char *path)
{
    FILE *shaderFile = fopen(path, "r");
    if (!shaderFile) {
        fprintf(stderr, "ERROR:  loadShader() ... Failed to open file: %s ...\n", path);
        return 0;
    }

    fseek(shaderFile, 0, SEEK_END);
    long fileSize = ftell(shaderFile);
    fseek(shaderFile, 0, SEEK_SET);

    char *shaderTxt = (char *)malloc(fileSize + 1);
    size_t bytes = fread(shaderTxt, 1, fileSize, shaderFile);
    fclose(shaderFile);
    shaderTxt[fileSize] = '\0';

    return shaderTxt;
}

GLuint piUtils::createProgramFromFiles(const char *vertexPath, const char *fragmentPath)
{
    // printf("\nDEBUG:  Vertex Shader: %s ..", vertexPath);
    // printf("\nDEBUG:    Frag Shader: %s ..", fragmentPath);

    // Load shader source code from files
    char* vertexSource   = loadShader(vertexPath);
    char* fragmentSource = loadShader(fragmentPath);

    GLuint prog = piUtils::createProgramFromSource(vertexSource, fragmentSource);

    free(vertexSource);
    free(fragmentSource);

    return prog;
}

//======================================================================================================

GLuint piUtils::createProgramFromSource( const char *vertexShaderSource, const char *fragmentShaderSource )
{
    GLuint vertexShader   = piUtils::createShader(GL_VERTEX_SHADER,   vertexShaderSource);
    checkGLError("createShader - VERTEX");

    // printf("\n DEBUG - createProgramFromSource() ... vertexShader: %d \n", vertexShader);

    GLuint fragmentShader = piUtils::createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    checkGLError("createShader - FRAGMENT");

    // printf("\n DEBUG - createProgramFromSource() ... fragmentShader: %d \n", fragmentShader);

    return piUtils::createProgram(vertexShader, fragmentShader);
}

//======================================================================================================

GLuint piUtils::createProgram(GLuint vertexShader, GLuint fragmentShader)
{
    if (vertexShader == 0 || fragmentShader == 0) {
        printf("\n ERROR - loading shaders failed. - BAD ARGS\n\n");
        return 0;
    }

    GLuint program = glCreateProgram();
    checkGLError("createProgram - VERTEX");

    glAttachShader(program, vertexShader);
    checkGLError("createProgram - glAttachShader - VERTEX");

    glAttachShader(program, fragmentShader);
    checkGLError("createProgram - glAttachShader - FRAGMENT");

    glLinkProgram(program);
    checkGLError("createProgram - linker ...");

    // Check program linking status
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
        fprintf(stderr, "Program linking failed: %s\n", infoLog);
        glDeleteProgram(program);
        return 0;
    }

    GLchar infoLog[512];

    // Check vertex shader compilation
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation failed:\n" << infoLog << std::endl;
    }
    checkShaderCompilation(vertexShader);


    // Check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment shader compilation failed:\n" << infoLog << std::endl;
    }
    checkShaderCompilation(fragmentShader);

    // Check program linking
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "Program linking failed:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

//  fprintf(stderr, "DEBUG: piUtils::createProgram()  program: %d\n", program);

    // CHECK IT
    checkProgramLinking(program);

    return program;
}

//======================================================================================================

void piUtils::checkShaderCompilation(GLuint shader)
{
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logSize;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
        std::vector<GLchar> log(logSize);
        glGetShaderInfoLog(shader, logSize, nullptr, log.data());
        std::cerr << "Shader compilation error:\n" << log.data() << std::endl;
        // Handle error (e.g., throw an exception)
    }
}

//======================================================================================================

void piUtils::checkProgramLinking(GLuint program)
{
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logSize;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
        std::vector<GLchar> log(logSize);
        glGetProgramInfoLog(program, logSize, nullptr, log.data());
        std::cerr << "Program linking error:\n" << log.data() << std::endl;
        // Handle error (e.g., throw an exception)
    }
}

//======================================================================================================
//======================================================================================================
