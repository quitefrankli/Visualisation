cmake_minimum_required(VERSION 3.20)

# set the project name
project(Visualisation)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ version")
set(CMAkE_CXX_STANDARD_REQUIRED True CACHE STRING "is standard required?")

add_definitions(/MP12)
add_definitions(-D__WIN32)

add_definitions(-DPROJECT_TOP_LEVEL_SRC_DIR="${CMAKE_SOURCE_DIR}")
add_definitions(-DPROJECT_BUILD_DIR="${CMAKE_BINARY_DIR}")
add_definitions(-DPROJECT_BIN_DIR="${CMAKE_BINARY_DIR}/bin")

if(NOT ENABLE_COMPILER_WARNINGS)
	# MSVC warnings https://docs.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-170
	if(MSVC)
		add_definitions(/W1)
	else()
		add_definitions(-w)
	endif()
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ PROPERTY VS_STARTUP_PROJECT Visualisation)

file(GLOB_RECURSE VisualisationLibSources ${CMAKE_SOURCE_DIR}/VisualisationLib/*.cpp)
add_library(VisualisationLibs ${VisualisationLibSources})

file(GLOB_RECURSE VisualisationDisplaySources ${CMAKE_SOURCE_DIR}/Display/*.cpp)

if(${ENABLE_CONSOLE})
	message("Enabling Console!")
else()
	message("Disabling Console!")
	# hides the console window that usually pops up with windows applications
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

add_executable(VisualisationDisplay ${VisualisationDisplaySources})
target_include_directories(VisualisationDisplay PRIVATE ${CMAKE_SOURCE_DIR}/VisualisationLib)
target_link_libraries(VisualisationDisplay VisualisationLibs)