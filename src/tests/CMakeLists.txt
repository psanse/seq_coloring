
#######################
# Sequential coloring unit tests
#######################

add_executable (tests_sequential_coloring      
    test_seq_coloring.cpp    
)

# Link dependencies
target_link_libraries(tests_sequential_coloring
    PRIVATE
        graph
        bitscan
        utils
        GTest::gtest_main
)


# Include directories
target_include_directories(tests_sequential_coloring
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${bitgraph_SOURCE_DIR}/src
)

# Require C++14
target_compile_features(tests_sequential_coloring
    PRIVATE cxx_std_14
)

# Output directory
set_target_properties(tests_sequential_coloring 
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)


# Register GoogleTest tests with CTest
include(GoogleTest)
gtest_discover_tests(tests_sequential_coloring)







