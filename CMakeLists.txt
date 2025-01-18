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

add_subdirectory(dependencies/glfw EXCLUDE_FROM_ALL)

# Include sub-projects.

add_executable (${PROJECT_NAME}
	src/main.cpp
	src/config.h
  src/config.cpp
	src/glad.c
	src/triangle_mesh.cpp
  src/stb_image.h
  src/Material.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_include_directories(${PROJECT_NAME} PUBLIC dependencies/glfw/include)

target_link_directories(${PROJECT_NAME} PRIVATE dependencies/glfw/src)

target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/src/shader"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shader
)

if (IS_DIRECTORY "${PROJECT_SOURCE_DIR}/asset")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "asset")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/asset"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/asset)
endif()