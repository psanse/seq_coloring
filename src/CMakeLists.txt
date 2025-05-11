# add the target and link to BitGraph libs
file(GLOB HEADER_FILES *.h)
file(GLOB SRC_FILES *.cpp)
add_executable(sequential_coloring ${SRC_FILES})

target_link_libraries(sequential_coloring PRIVATE graph bitscan utils)

set_target_properties(sequential_coloring 
	PROPERTIES
	#ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

add_subdirectory(tests)
