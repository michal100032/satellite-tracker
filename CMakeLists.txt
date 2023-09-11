cmake_minimum_required (VERSION 3.12)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# You may need to set CMAKE_TOOLCHAIN_FILE
# set(CMAKE_TOOLCHAIN_FILE "<path to vcpkg>/scripts/buildsystems/vcpkg.cmake" CACHE STRING "vcpkg toolchain file")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

project (satellite-tracker)
add_executable (
	${PROJECT_NAME}
	"src/main.cpp"
	"src/Application.cpp"
	"src/Earth.cpp"
	"src/Mesh.cpp"
	"src/Shader.cpp"
	"src/Texture.cpp"
	"src/stb.cpp"
	"src/Camera.cpp"
	"src/consts.cpp"
    "src/Orbit.cpp"
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm::glm GLEW::GLEW)
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})