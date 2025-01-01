# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("OpenGL3D")

find_package(OpenGL REQUIRED)

# Include sub-projects.

add_executable (OpenGL3D
	src/main.cpp
	src/config.h
	src/glad.c
)

target_include_directories(OpenGL3D PRIVATE include)

target_link_libraries(OpenGL3D "${PROJECT_SOURCE_DIR}/lib/glfw3.lib" OpenGL::GL)

if (IS_DIRECTORY "${PROJECT_SOURCE_DIR}/src/shader")
    add_custom_command(TARGET OpenGL3D POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "shader")
    add_custom_command(TARGET OpenGL3D POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/src/shader"
            $<TARGET_FILE_DIR:OpenGL3D>/shader)
endif()