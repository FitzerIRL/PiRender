cmake_minimum_required(VERSION 3.10)
project(RenderTests)

# Set C++ standard and other options for the tests
set(CMAKE_CXX_STANDARD 17)  # or 14 if your compiler doesn't support 17
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Include the FetchContent module
include(FetchContent)

# Fetch and include Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0  # Use the release tag you want
)

FetchContent_MakeAvailable(googletest)


# Include the glm directory
include_directories(
         ${CMAKE_SOURCE_DIR}/../deps
         ${CMAKE_SOURCE_DIR}/../deps/rt
         ${CMAKE_SOURCE_DIR}/../deps/glm-master)


# Include directories for the whole project
include_directories(
    ${CMAKE_SOURCE_DIR}/../deps/glm

    ${CMAKE_SOURCE_DIR}/../src/core
    ${CMAKE_SOURCE_DIR}/../src/shader
    ${CMAKE_SOURCE_DIR}/../src/scene
)

file(GLOB_RECURSE TEST_FILES *.cpp)
list(FILTER TEST_FILES EXCLUDE REGEX ".*CompilerIdCXX.*")

# Add your test source files
add_executable(${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/../src/core/piUtils.cpp
    ${CMAKE_SOURCE_DIR}/../src/core/piObject.cpp
    ${CMAKE_SOURCE_DIR}/../src/core/piValue.cpp
    ${CMAKE_SOURCE_DIR}/../src/core/piTexture.cpp

    ${CMAKE_SOURCE_DIR}/../src/scene/piImage.cpp
    ${CMAKE_SOURCE_DIR}/../src/scene/piScene.cpp
    ${CMAKE_SOURCE_DIR}/../src/scene/piSpritesheet.cpp


    ${TEST_FILES} )


#     piImage_test.cpp)

# add_executable(RenderTests ${TEST_FILES})
# target_link_libraries(RenderTests gtest gtest_main YourMainProjectLibrary)

# Link against Google Test libraries
target_link_libraries(${PROJECT_NAME} gtest glfw GLESv2 gtest_main )

# Link against your main project library or source files
# target_link_libraries(${PROJECT_NAME} YourMainProjectLibrary)

# Optionally, configure any pre-test setup or fixtures
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})




# Copy files using file(COPY ...)
file(COPY ${CMAKE_SOURCE_DIR}/../resources/images/STpadd_fg_1280_720.png DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY ${CMAKE_SOURCE_DIR}/../resources/images/ball.png DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY ${CMAKE_SOURCE_DIR}/../resources/images/Smiling_Face.png DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY ${CMAKE_SOURCE_DIR}/../resources/images/Robin_240_160.png DESTINATION ${CMAKE_BINARY_DIR}/)

file(COPY ${CMAKE_SOURCE_DIR}/../resources/shaders/vertexShader.glsl DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY ${CMAKE_SOURCE_DIR}/../resources/shaders/fragmentShader.glsl DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY ${CMAKE_SOURCE_DIR}/../resources/shaders/fragStarField_1.glsl DESTINATION ${CMAKE_BINARY_DIR}/)

file(COPY ${CMAKE_SOURCE_DIR}/../resources/spritesheet/spritesheet.json DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY ${CMAKE_SOURCE_DIR}/../resources/spritesheet/spritesheet.png DESTINATION ${CMAKE_BINARY_DIR}/)

