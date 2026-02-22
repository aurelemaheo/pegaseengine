cmake_minimum_required(VERSION 3.22)

# Project name
project(pegaseengine
	    VERSION 0.1
	    DESCRIPTION "Physic engine supporting simulation of rigid bodies in C++"
	    LANGUAGES CXX)

#set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Static library

# Generate list of source files
file(
	GLOB_RECURSE
	PEGASE_SOURCES
	CONFIGURE_DEPENDS
	"src/*.cpp"
)

# Build static library
#add_executable(pegaseengine ${source_files})
add_library(pegaseengine STATIC ${PEGASE_SOURCES})
target_include_directories(pegaseengine PUBLIC src/)
#add_subdirectory(examples)
set_target_properties(pegaseengine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# EXEMPLES

file(GLOB EXAMPLE_SOURCES 
	 CONFIGURE_DEPENDS 
	 "${CMAKE_SOURCE_DIR}/*.cpp"
	 )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

foreach(example_src ${EXAMPLE_SOURCES})
	get_filename_component(example_name ${example_src} NAME_WE)
	add_executable(${example_name} ${example_src})
	target_link_libraries(${example_name} PRIVATE pegaseengine)	
	add_dependencies(${example_name} pegaseengine)
	#target_compile_features(${example_name} PRIVATE cxx_std_20)
endforeach()

