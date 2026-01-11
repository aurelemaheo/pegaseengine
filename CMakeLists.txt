cmake_minimum_required(VERSION 3.22)

# Project name
project(pegaseengine)
#set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_STANDARD 14)

#Generate list of source files
file(
	GLOB_RECURSE
	source_files
	src/*
)

include_directories("/Users/aurele/.pyenv/versions/3.10.10/lib/python3.10/site-packages/pybind11/include")
include_directories("/Users/aurele/.pyenv/versions/3.10.10/include/python3.10")

# Build static library
add_executable(pegaseengine ${source_files})
#add_library(pegaseengine STATIC ${source_files})
set_target_properties(pegaseengine PROPERTIES LINKER_LANGUAGE CXX)

