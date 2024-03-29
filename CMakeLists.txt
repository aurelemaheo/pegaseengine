cmake_minimum_required(VERSION 3.22)

# Project name
project(pegaseengine)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "-w -g")

#target_include_directories(test PRIVATE ${/home/aurele/git/pegaseengine/thirdparty/boost_1_72_0})

# Include
include_directories(include)
include_directories(/home/aurele/git/pegaseengine/thirdparty)
include_directories(/home/aurele/git/pegaseengine/thirdparty/glad/include)
include_directories(/home/aurele/git/pegaseengine/thirdparty/glfw/include)
#include_directories(/home/aurele/git/pegaseengine/thirdparty/jsoncpp/include)
#include_directories(/System/Library/Frameworks/Python.framework//Versions/2.7/include/python2.7)
include_directories(/Library/Frameworks/Python.framework/Versions/3.8/include/python3.8)
include_directories(${GLEW_INCLUDE_DIRS})

#Library
#add_library(libglfw3 STATIC /home/aurele/git/pegaseengine/thirdparty/glfw/src)

link_directories(/home/aurele/git/pegaseengine/thirdparty/glfw/src)
link_directories(/Library/Frameworks/Python.framework/Versions/3.8/lib)
#link_directories(/home/aurele/git/pegaseengine/thirdparty/jsoncpp/lib)
link_directories(${GLEW_LIBRARIES})
#link_directories(/usr/local/Cellar/glfw/3.3.2/lib)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
#find_library(GLUT_LIBRARY GLUT)
find_library(GLUT_LIBRARIES GLUT)

if (APPLE)
	find_library(COCOA_LIBRARY Cocoa)
	find_library(CARBON_LIBRARY Carbon)
	find_library(IOKIT_LIBRARY IOKit)
	find_library(COREFOUNDATION_LIBRARY CoreFoundation)
	find_library(CORESERVICES_LIBRARY CoreServices)
endif ()

find_library(OPENGL_LIBRARY OpenGL)
#find_library(JSONCPP_LIBRARY libjsoncpp.dylib)
#find_library(PYTHON_LIBRARIES libpython3.8.dylib)

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
#target_link_libraries(pegaseengine glfw3)
target_link_libraries(pegaseengine ${GLUT_LIBRARY})

if (APPLE)
	target_link_libraries(pegaseengine ${COCOA_LIBRARY})
	target_link_libraries(pegaseengine ${CARBON_LIBRARY})
	target_link_libraries(pegaseengine ${IOKIT_LIBRARY})
	target_link_libraries(pegaseengine ${CORESERVICES_LIBRARY})
	target_link_libraries(pegaseengine ${COREFOUNDATION_LIBRARY})
endif()

target_link_libraries(pegaseengine ${OPENGL_LIBRARY} ${GLUT_LIBRARY} ${GLUT_LIBRARIES})
target_link_libraries(pegaseengine ${JSONCPP_LIBRARY})
#target_link_libraries(pegaseengine libpython3.8.dylib)
#target_link_libraries(pegaseengine PYTHON_LIBRARIES)
#target_link_libraries(pegaseengine libglfw.3.3.dylib)


if (APPLE)
#  target_link_libraries(pegaseengine "-framework OpenGL")
endif()
