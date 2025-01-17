cmake_minimum_required(VERSION 3.10)

# set the project name
project(non_default_constructible VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Library
add_library(non_default_constructible INTERFACE)
target_include_directories(non_default_constructible INTERFACE "./include")

# Test
add_executable(tests tests/test.cpp)

# find_package(Catch2 3 REQUIRED PATHS "D:/SourceCode/Catch2/build64_install")
Include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.4.0 # or a later release
    FIND_PACKAGE_ARGS PATHS "D:/SourceCode/Catch2/build64_install" # To try find_package before fetching from git
)
FetchContent_MakeAvailable(Catch2)

# find_package(Catch2 REQUIRED)
target_link_libraries(tests PRIVATE non_default_constructible Catch2::Catch2WithMain)

enable_testing() # enable testing functionality
add_test(NAME tests COMMAND $<TARGET_FILE:tests>)

function(add_compile_fail_test test_name source_file)
    add_custom_target(${test_name}
        COMMAND ${CMAKE_COMMAND} -E echo "Testing compilation failure for ${source_file}"
        COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS} -c ${CMAKE_CURRENT_SOURCE_DIR}/${source_file} -o /dev/null
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Checking that ${source_file} fails to compile"
        VERBATIM
    )
    
    # Register the custom target as a test
    add_test(NAME ${test_name} COMMAND ${CMAKE_COMMAND} --build . --target ${test_name})
    
    # Mark the test as expected to fail
    set_tests_properties(${test_name} PROPERTIES WILL_FAIL TRUE)
endfunction()
add_compile_fail_test(compile_fail_test_1 tests/test_fail_1.cpp)

# Install
install(FILES "./include/non_default_constructible.h" DESTINATION include)

