cmake_minimum_required (VERSION 3.12)

# You may need to set CMAKE_TOOL_CHAIN_PATH
# set(CMAKE_TOOLCHAIN_FILE /path/to/vcpkg CACHE STRING "vcpkg toolchain file")

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

project (satellite-tracker)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_executable (${PROJECT_NAME} "src/main.cpp" "src/Application.cpp")

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm::glm GLEW::GLEW)