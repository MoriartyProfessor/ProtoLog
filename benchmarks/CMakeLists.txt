add_executable(ProtologBenchmark benchmarks.cpp)
target_include_directories(ProtologBenchmark PUBLIC "${INCLUDES}")
target_link_libraries(ProtologBenchmark fmt::fmt)
