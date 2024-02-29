cmake_minimum_required(VERSION 3.10)

project(cpp-opengl)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog REQUIRED)

add_subdirectory(src/stb)

include_directories(
    src
)

list(APPEND LINK_LIBS
    glfw
    OpenGL::GL
    glad::glad
    glm::glm
    spdlog::spdlog
    stb
)

file(GLOB SRC_FILE
    src/*.cpp
    src/io/*.cpp
    src/graphics/*.cpp
    src/graphics/models/*.cpp
)

add_executable(${PROJECT_NAME}
    ${SRC_FILE}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${LINK_LIBS}
)