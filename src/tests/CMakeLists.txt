###################
# unit tests with googletest framwork

add_executable (tests_sequential_coloring 
     
    test_seq_coloring.cpp
    
)

set_target_properties(tests_sequential_coloring 
    PROPERTIES
    #ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    #LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/tests"
)

target_include_directories(tests_sequential_coloring PUBLIC
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)


target_link_libraries (tests_sequential_coloring LINK_PUBLIC  graph bitscan gtest_main utils)





