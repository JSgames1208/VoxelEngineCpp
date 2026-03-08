cmake_minimum_required(VERSION 3.15)
project(3dEngine)

set(CMAKE_CXX_STANDARD 17)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw ${CMAKE_BINARY_DIR}/glfw_build)

add_executable(${PROJECT_NAME} "src/engine/main.cpp" "src/engine/Engine.cpp" "src/engine/Shader.cpp" "src/engine/Camera.cpp" "src/engine/texture/Texture.cpp"
"src/engine/texture/TextureManager.cpp" "src/engine/scene/Scene.cpp" "src/engine/mesh/Mesh.cpp" "src/engine/mesh/Meshmanager.cpp" "src/engine/renderer/block/BakedQuad.cpp" "src/engine/renderer/block/FaceBakery.cpp" "src/engine/texture/TextureAtlas.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE include)

file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/textures DESTINATION ${CMAKE_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw opengl32 glm::glm)
