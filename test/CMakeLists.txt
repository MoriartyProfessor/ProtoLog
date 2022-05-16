add_executable(ProtologTest test.cpp)

target_include_directories(ProtologTest PUBLIC "${INCLUDES}")
target_link_libraries(ProtologTest fmt::fmt)
