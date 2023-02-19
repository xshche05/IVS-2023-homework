cmake_minimum_required(VERSION 3.16)
project(ivs_proj_1)

set(CMAKE_CXX_STANDARD 17)


if(CMAKE_COMPILER_IS_GNUCXX)
    include(CodeCoverage.cmake)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(POSITION_INDEPENDENT_CODE ON)
endif()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Test targets
enable_testing()

include(GoogleTest)

find_library(BLACK_BOX_LIBS black_box_lib REQUIRED PATHS libs NO_DEFAULT_PATH)
include_directories("libs")

add_executable(black_box_test black_box_tests.cpp)
target_link_libraries(black_box_test ${BLACK_BOX_LIBS} gtest_main gmock_main)
gtest_discover_tests(black_box_test)

add_executable(white_box_test white_box_tests.cpp white_box_code.cpp)
target_link_libraries(white_box_test gtest_main gmock_main)
gtest_discover_tests(white_box_test)
if(CMAKE_COMPILER_IS_GNUCXX)
    SETUP_TARGET_FOR_COVERAGE(white_box_test_coverage white_box_test white_box_test_coverage)
endif()

add_executable(tdd_test tdd_code.cpp tdd_tests.cpp)
target_link_libraries(tdd_test gtest_main gmock_main)
gtest_discover_tests(tdd_test)
if(CMAKE_COMPILER_IS_GNUCXX)
    SETUP_TARGET_FOR_COVERAGE(tdd_test_coverage tdd_test tdd_test_coverage)
endif()

add_custom_target(pack
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMAND ${CMAKE_COMMAND} -E tar "cfv" "xlogin00.zip" --format=zip
        "black_box_tests.cpp"
        "white_box_tests.cpp"
        "tdd_code.h"
        "tdd_code.cpp")

find_package(Doxygen 1.8.0)
if(DOXYGEN_FOUND)
    set(doxyfile ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    add_custom_target(doc
            COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
            )
endif()
