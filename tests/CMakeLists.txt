include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

# --- test_core ---
add_executable(test_core test_core.cpp)
target_link_libraries(test_core gtest_main)
gtest_discover_tests(test_core)

# --- test_decoder ---
add_executable(test_decoder test_decoder.cpp)
target_link_libraries(test_decoder gtest_main)
target_sources(test_decoder PRIVATE
    ${CMAKE_SOURCE_DIR}/src/isa/decoder.cpp
)
gtest_discover_tests(test_decoder)
