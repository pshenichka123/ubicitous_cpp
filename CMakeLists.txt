cmake_minimum_required(VERSION 3.20)

project(lab1-libcompressor)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

include(GNUInstallDirs)

add_library(project_warnings INTERFACE)

target_compile_options(project_warnings INTERFACE
                        -Wall -Wextra -Wpedantic -Werror)

find_package(ZLIB REQUIRED CONFIG)
find_package(BZip2 REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)
find_package(GTest REQUIRED CONFIG)

enable_testing()

add_subdirectory(libcompressor)
add_subdirectory(compressor)

find_package(Doxygen)

if(DOXYGEN_FOUND)
  set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
  set(DOXYGEN_EXTRACT_ALL YES)

  doxygen_add_docs(docs
                   ${CMAKE_CURRENT_SOURCE_DIR}/libcompressor/include
                   ${CMAKE_CURRENT_SOURCE_DIR}/libcompressor/src
                   COMMENT "Generating libcompressor documentation")
endif()

file(GLOB_RECURSE PROJECT_SOURCES
     ${CMAKE_CURRENT_SOURCE_DIR}/libcompressor/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/libcompressor/*.hpp
     ${CMAKE_CURRENT_SOURCE_DIR}/compressor/*.cpp)

find_program(CLANG_FORMAT_EXE NAMES clang-format)

if(CLANG_FORMAT_EXE)
  add_custom_target(format
                    COMMAND ${CLANG_FORMAT_EXE} -i --style=file
                            ${PROJECT_SOURCES}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    COMMENT "Formatting sources with clang-format")
endif()

find_program(CLANG_TIDY_EXE NAMES clang-tidy)

if(CLANG_TIDY_EXE)
  add_custom_target(tidy
                    COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR}
                            --config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy
                            ${PROJECT_SOURCES}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    COMMENT "Running clang-tidy static analysis")
endif()