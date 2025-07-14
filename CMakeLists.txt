cmake_minimum_required(VERSION 2.8...3.14)

if(${CMAKE_VERSION} VERSION_GREATER "3.24.0") 
   cmake_policy(SET CMP0135 NEW) 
endif()

#linkBG : linking to BitGraph package
project(SEQ_VERTEX_COLOR LANGUAGES CXX)


# Include the module FetchContent_Declare (since 3.11)
include(FetchContent)

############
# Fetching Bitgraph

# Download and add the libraries from the BitScanPlus Github repo
FetchContent_Declare(
  bitgraph
  GIT_REPOSITORY https://github.com/psanse/BitGraph
  GIT_TAG v1.1.0 				# latest release
)
FetchContent_MakeAvailable(bitgraph)

# Add /src folder of BitGraph
include_directories (PUBLIC ${FETCHCONTENT_BASE_DIR}/bitgraph-src/src)

############
# Fetching googletest

FetchContent_Declare(
   googletest
   URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)

  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)	
  set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
  
FetchContent_MakeAvailable(googletest)
	
#C++14 is required for googletest 
set(CMAKE_CXX_STANDARD 14)					
set(CMAKE_CXX_STANDARD_REQUIRED ON)		
	
#for tests to be run with make test or ctest
enable_testing()	
	
message(STATUS "GoogleTest framework available for tests")


#add targets that link with BitScanPlus libs
add_subdirectory(src)





