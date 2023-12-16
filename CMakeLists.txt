cmake_minimum_required(VERSION 3.10)
project(PiRender)

# Find OpenGL package
# find_package(OpenGL REQUIRED)

# Find GLFW package
find_package(glfw3 REQUIRED)


set(CMAKE_BUILD_TYPE Debug)

include_directories(
    src/core
    src/shader
    src/scene
)

# Set source files
set(SOURCES
    # src/PiRender.cpp
    src/TestRender4  # big scene
    # src/TestRender5_rotation.cpp

    # src/TestRender_batch.cpp
    # src/TestRender_program.cpp

    src/core/piObject.cpp
    src/core/piValue.cpp
    src/core/piUtils.cpp
    src/core/piTexture.cpp

    src/shader/piProgram.cpp
    src/shader/piShaderToy.cpp
    src/shader/piShaderManager.cpp
    src/shader/piShaderUtils.cpp

    src/scene/piScene.cpp
    src/scene/piSpritesheet.cpp
    src/scene/piImage.cpp
 
    )

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})


# Include the glm directory
target_include_directories(PiRender PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(PiRender PRIVATE ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(PiRender PRIVATE ${CMAKE_SOURCE_DIR}/deps/rt)
target_include_directories(PiRender PRIVATE ${CMAKE_SOURCE_DIR}/deps/glm-master)


# Link against GLFW, GLESv2, and OpenGL
# target_link_libraries(${PROJECT_NAME} PRIVATE glfw GLESv2 OpenGL::GL)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw GLESv2)


# Set C++ standard
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

# Copy files using file(COPY ...)
file(COPY ${CMAKE_SOURCE_DIR}/resources/images/STpadd_fg_1280_720.png DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY ${CMAKE_SOURCE_DIR}/resources/images/ball.png DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY ${CMAKE_SOURCE_DIR}/resources/images/Smiling_Face.png DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY ${CMAKE_SOURCE_DIR}/resources/images/Robin_240_160.png DESTINATION ${CMAKE_BINARY_DIR}/)

file(COPY ${CMAKE_SOURCE_DIR}/resources/shaders/vertexShader.glsl DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY ${CMAKE_SOURCE_DIR}/resources/shaders/fragmentShader.glsl DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY ${CMAKE_SOURCE_DIR}/resources/shaders/fragStarField_1.glsl DESTINATION ${CMAKE_BINARY_DIR}/)

file(COPY ${CMAKE_SOURCE_DIR}/resources/spritesheet/spritesheet.json DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY ${CMAKE_SOURCE_DIR}/resources/spritesheet/spritesheet.png DESTINATION ${CMAKE_BINARY_DIR}/)

