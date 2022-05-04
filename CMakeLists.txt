cmake_minimum_required(VERSION 3.10)

# set the project name
project(ProtologTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

find_package(fmt)

add_subdirectory(test)
add_subdirectory(samples)
add_subdirectory(benchmarks)
add_subdirectory(protolog)
add_subdirectory(third-party/doctest)

target_include_directories(ProtologTest PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/third-party/doctest/"
                          "${PROJECT_SOURCE_DIR}/protolog/include"
                          "${PROJECT_SOURCE_DIR}/protolog/include/Formatters"
                          "${PROJECT_SOURCE_DIR}/protolog/include/Handlers"
                          )

target_include_directories(ProtologBenchmark PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/third-party/doctest/"
                          "${PROJECT_SOURCE_DIR}/protolog/include"
                          "${PROJECT_SOURCE_DIR}/protolog/include/Formatters"
                          "${PROJECT_SOURCE_DIR}/protolog/include/Handlers"
                          )
target_include_directories(ProtologSample PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/third-party/doctest/"
                          "${PROJECT_SOURCE_DIR}/protolog/include"
                          "${PROJECT_SOURCE_DIR}/protolog/include/Formatters"
                          "${PROJECT_SOURCE_DIR}/protolog/include/Handlers"
                          )
                          
