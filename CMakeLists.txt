cmake_minimum_required(VERSION 2.6)

# Project name
project(pegaseengine)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_STANDARD 11)

#target_include_directories(test PRIVATE ${/Users/aurele/git/pegaseengine/thirdparty/boost_1_72_0})

# Include
include_directories(include)
include_directories(/Users/aurele/git/pegaseengine/thirdparty/boost_1_72_0)

#Generate list of source files
file(
	GLOB_RECURSE
	source_files
	src/*
)

#Binary name
add_executable(
  pegaseengine
  ${source_files}
)
