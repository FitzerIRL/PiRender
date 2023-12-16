
#include <piProgram.h>

// #include <GL/glew.h>

#include <iostream>

// Internal state variables
GLint piProgram::savedProgram = -1;
GLint piProgram::savedVertexArray = -1;
GLint piProgram::savedElementArrayBuffer = -1;
GLint piProgram::savedArrayBuffer = -1;

void piProgram::saveAndSetGLState()
{
    glGetIntegerv(GL_CURRENT_PROGRAM, &savedProgram);
    glGetIntegerv(GL_ARRAY_BUFFER, &savedVertexArray);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER, &savedElementArrayBuffer);
    // glGetIntegerv(GL_VERTEX_ARRAY_BINDING_OES, &savedVertexArray);
    // glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING_OES, &savedElementArrayBuffer);
    // Add more state variables as needed

    // Set desired OpenGL state here
    // For example, glEnable, glDisable, glBlendFunc, etc.
}

void piProgram::restoreGLState()
{
    glUseProgram(savedProgram);

    // Bind the VBO + EBO for the background
    glBindBuffer(GL_ARRAY_BUFFER, savedVertexArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, savedElementArrayBuffer);


    // glBindVertexArrayOES(savedVertexArray);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_OES, savedElementArrayBuffer);
    // Restore additional saved state variables

    // Reset any OpenGL state that was changed in saveAndSetGLState
}


GLuint piProgram::createProgram(GLuint vertexShader, GLuint fragmentShader)
{
    saveAndSetGLState();

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    checkProgramLinking(program);

    restoreGLState();
    return program;
}

//

GLuint piProgram::compileShader(GLenum type, const char *source)
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
        fprintf(stderr, "Shader compilation failed: %s\n", infoLog);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint piProgram::loadAndCompileShader(const GLuint shaderType, const char *path)
{
    // Load shader source code from files
    FILE *shaderFile = fopen(path, "r");
    if (!shaderFile) {
        fprintf(stderr, "Failed to open vertex shader file: %s\n", path);
        return 0;
    }

    fseek(shaderFile, 0, SEEK_END);
    long fileSize = ftell(shaderFile);
    fseek(shaderFile, 0, SEEK_SET);

    char *shaderSource = (char *)malloc(fileSize + 1);
    size_t bytes = fread(shaderSource, 1, fileSize, shaderFile);
    fclose(shaderFile);

    shaderSource[fileSize] = '\0';

    GLuint shader = compileShader(shaderType, shaderSource);

    return shader;
}

//

GLuint piProgram::createProgramFromFiles(const char *vertexShaderPath, const char *fragmentShaderPath)
{
    // Load and compile shaders (you'll need to implement shader loading)
    GLuint vertexShader   = loadAndCompileShader(GL_VERTEX_SHADER, vertexShaderPath);
    GLuint fragmentShader = loadAndCompileShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

    return createProgram(vertexShader, fragmentShader);
}

GLuint piProgram::createProgramFromSource(const char *vertexShaderSource, const char *fragmentShaderSource)
{
    // Compile shaders (you'll need to implement shader compilation)
    GLuint vertexShader   = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    return createProgram(vertexShader, fragmentShader);
}

void piProgram::checkShaderCompilation(GLuint shader)
{
    saveAndSetGLState();

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        GLchar *log = new GLchar[logLength];
        glGetShaderInfoLog(shader, logLength, nullptr, log);

        // Handle shader compilation error (print log, throw an exception, etc.)
        // For simplicity, we'll print the log here
        std::cerr << "Shader Compilation Error:\n" << log << std::endl;

        delete[] log;
    }

    restoreGLState();
}

void piProgram::checkProgramLinking(GLuint program)
{
    saveAndSetGLState();

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

        GLchar *log = new GLchar[logLength];
        glGetProgramInfoLog(program, logLength, nullptr, log);

        // Handle program linking error (print log, throw an exception, etc.)
        // For simplicity, we'll print the log here
        std::cerr << "Program Linking Error:\n" << log << std::endl;

        delete[] log;
    }

    restoreGLState();
}


GLuint piProgram::createVBO(const void *data, GLsizei dataSize)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vbo;
}

GLuint piProgram::createEBO(const GLuint *indices, GLsizei indexCount)
{
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return ebo;
}

void piProgram::bindVBO(GLuint vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void piProgram::bindEBO(GLuint ebo)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}