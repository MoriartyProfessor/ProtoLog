cmake_minimum_required(VERSION 3.10)

# set the project name
project(ProtologTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(test)
add_subdirectory(samples)
add_subdirectory(benchmarks)
add_subdirectory(doctest)
add_subdirectory(protolog)

target_include_directories(ProtologTest PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/doctest/"
                          )
target_include_directories(ProtologTest PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/protolog/include"
                          )
target_include_directories(ProtologTest PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/protolog/include/Formatters"
                          )
target_include_directories(ProtologTest PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/protolog/include/Handlers"
                          )

target_include_directories(ProtologSample PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/doctest/"
                          )
target_include_directories(ProtologSample PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/protolog/include"
                          )
target_include_directories(ProtologSample PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/protolog/include/Formatters"
                          )
target_include_directories(ProtologSample PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/protolog/include/Handlers"
                          )
                          
target_include_directories(ProtologBenchmark PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/doctest/"
                          )
target_include_directories(ProtologBenchmark PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/protolog/include"
                          )
target_include_directories(ProtologBenchmark PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/protolog/include/Formatters"
                          )
target_include_directories(ProtologBenchmark PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/protolog/include/Handlers"
                          )
