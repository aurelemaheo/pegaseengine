cmake_minimum_required(VERSION 3.22)

# Project name
project(pegaseengine)
#set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_STANDARD 11)

#Generate list of source files
file(
	GLOB_RECURSE
	source_files
	src/*
)

#include_directories(${source_files})

# Build static library
add_library(pegaseengine STATIC ${source_files})
set_target_properties(pegaseengine PROPERTIES LINKER_LANGUAGE CXX)

