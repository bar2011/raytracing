cmake_minimum_required(VERSION 3.28)
project(raytracer LANGUAGES CXX OBJCXX)

# CMake configuration
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Apple Frameworks
find_library(METAL Metal REQUIRED)
find_library(APPKIT AppKit REQUIRED)
find_library(METALKIT MetalKit REQUIRED)

# Include metal shader compilation capabilities
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")
include(MetalShaderSupport)
enable_language(Metal)

# Helper function to glob all source files in a directory
function(glob_sources result_dir result_var)
  file(
    GLOB_RECURSE
    tmp_list
    "${result_dir}/*.cpp"
    "${result_dir}/*.mm"
    "${result_dir}/*.c"
    "${result_dir}/*.m"
    "${result_dir}/*.cc"
    "${result_dir}/*.cxx"
    "${result_dir}/*.metal")
  set(${result_var}
      ${tmp_list}
      PARENT_SCOPE)
endfunction()

# Grab sources by directory
glob_sources("${CMAKE_SOURCE_DIR}/src/app" APP_SOURCES)
glob_sources("${CMAKE_SOURCE_DIR}/src/render" RENDER_SOURCES)
glob_sources("${CMAKE_SOURCE_DIR}/src/utils" UTILS_SOURCES)
glob_sources("${CMAKE_SOURCE_DIR}/src/runtime" RUNTIME_SOURCES)
glob_sources("${CMAKE_SOURCE_DIR}/shaders" METAL_SHADER_SOURCES)

# Compile each folder as a static library
add_library(app STATIC ${APP_SOURCES})
add_library(render STATIC ${RENDER_SOURCES})
add_library(utils STATIC ${UTILS_SOURCES})
add_library(runtime STATIC ${RUNTIME_SOURCES})

# Main executable
add_executable(raytracer MACOSX_BUNDLE "src/runtime/main.cpp")

# Set executable resources path
set_target_properties(
  raytracer PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE
                       INSTALL_RPATH "@executable_path/../Resources")

# Add shaders to executable
add_metal_shader_library(default ${METAL_SHADER_SOURCES})
target_embed_metal_shader_libraries(raytracer default)

# Linking all libraries
target_link_libraries(
  raytracer
  PRIVATE app
          render
          utils
          runtime
          stdc++
          "-framework Metal"
          "-framework AppKit"
          "-framework MetalKit"
          objc)

# Include directories
target_include_directories(app PRIVATE dependencies src)
target_include_directories(render PRIVATE dependencies src)
target_include_directories(utils PRIVATE dependencies src)
target_include_directories(runtime PRIVATE dependencies src)
target_include_directories(raytracer PRIVATE dependencies src)
