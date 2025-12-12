add_executable(udp-test udp-test.cpp)
target_link_libraries(udp-test PRIVATE Catch2::Catch2 example3)
