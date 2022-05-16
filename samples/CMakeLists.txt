add_executable(ProtologSample sample.cpp)
target_include_directories(ProtologSample PUBLIC "${INCLUDES}")
target_link_libraries(ProtologSample fmt::fmt)
