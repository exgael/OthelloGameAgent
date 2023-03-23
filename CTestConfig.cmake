set(CTEST_PROJECT_NAME "MyProject")
set(CTEST_NIGHTLY_START_TIME "00:00:00 UTC")

set(CTEST_TEST_TIMEOUT 1800)

# Add any additional configuration here...

# Include the test directory
add_subdirectory(tests)
