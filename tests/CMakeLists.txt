# === tests/CMakeLists.txt ===
include(FetchContent)

# Download GoogleTest automatically (v1.14)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Create the test executable
add_executable(test_core test_core.cpp)

# Link it with the GoogleTest framework
target_link_libraries(test_core gtest_main)

# Integrate GoogleTest with CMake's ctest
include(GoogleTest)
gtest_discover_tests(test_core)