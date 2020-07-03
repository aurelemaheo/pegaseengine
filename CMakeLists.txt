cmake_minimum_required(VERSION 2.6)

# Project name
project(pegaseengine)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

#target_include_directories(test PRIVATE ${/Users/aurele/git/pegaseengine/thirdparty/boost_1_72_0})

# Include
include_directories(include)
include_directories(/Users/aurele/git/pegaseengine/thirdparty/boost_1_72_0)
include_directories(/Users/aurele/git/pegaseengine/thirdparty/glfw/include)

#Library
#add_library(libglfw3 STATIC /Users/aurele/git/pegaseengine/thirdparty/glfw/src)

#link_directories(/Users/aurele/git/pegaseengine/thirdparty/glfw/src)
link_directories(/usr/local/Cellar/glfw/3.3.2/lib)

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

#target_link_libraries(pegaseengine ${GLFW3_LIBRARY})
target_link_libraries(pegaseengine libglfw.3.3.dylib)

if (APPLE)
#  target_link_libraries(pegaseengine "-framework OpenGL")
endif()
