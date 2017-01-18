cmake_minimum_required(VERSION 2.6)

# Project name
project(pegaseengine)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

# Include
include_directories(include)

#Generate list of source files
file(
	GLOB_RECURSE
	source_files
	src/*
        include/*
)

#Binary name
add_executable(
  pg
  ${source_files}
)
