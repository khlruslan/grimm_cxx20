cmake_minimum_required(VERSION 3.14)

project(cond_var)

if (MSVC)
  # warning level 4
  add_compile_options(/W4)
else()
  # additional warnings
  add_compile_options(-Wall -Wextra -Wpedantic)
  add_compile_options(-fno-omit-frame-pointer -O2 )
endif()

# Add sanitizer flags for C++ compiler for "Debug" configuration
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")

OPTION(DEFINE_MACRO "Option description" ON) # Enabled by default
IF(DEFINE_MACRO)
    ADD_DEFINITIONS(-Dmy_debug)
ENDIF(DEFINE_MACRO)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories( ${CMAKE_SOURCE_DIR}/inc)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)

