# Sequential coloring executable

file(GLOB HEADER_FILES CONFIGURE_DEPENDS "*.h")
file(GLOB SRC_FILES CONFIGURE_DEPENDS "*.cpp")

add_executable(sequential_coloring 
	${HEADER_FILES}
	${SRC_FILES}
)

# Link BitGraph libraries
target_link_libraries(sequential_coloring
	PRIVATE
		graph
		bitscan
		utils
)

# BitGraph headers
target_include_directories(sequential_coloring
    PRIVATE
        ${bitgraph_SOURCE_DIR}/src
)


# Require C++14
target_compile_features(sequential_coloring
    PRIVATE cxx_std_14
)

# Executable output directory
set_target_properties(sequential_coloring
	PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)


# Unit tests
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
