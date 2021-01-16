cmake_minimum_required (VERSION 3.4)
project (cpp11tests)

set (CHRONO_TEST_SRCS chrono_test.cpp)
set (FUTURE_TEST_SRCS future_test.cpp)

add_executable (chrono_test ${CHRONO_TEST_SRCS})
add_executable (future_test ${FUTURE_TEST_SRCS})
