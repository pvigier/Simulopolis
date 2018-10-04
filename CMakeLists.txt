# Configuration

cmake_minimum_required(VERSION 3.1)

project(Simulopolis)

# Compiler 

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s")

# Special flags

if(${CMAKE_CXX_PLATFORM_ID} MATCHES "MinGW")
	message("MinGW")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	message("GCC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-pie -no-pie -Wl,-rpath,.")
endif()

# Files

file(GLOB_RECURSE SRCS src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)

# Search directories

include_directories(src/)

# Executable

set(EXECUTABLE_NAME "Simulopolis")
add_executable(${EXECUTABLE_NAME} ${SRCS} ${HEADERS})

# Libraries

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2.4 REQUIRED network audio graphics window system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
else()
    message(FATAL_ERROR "SFML not found")
endif()

find_package (TinyXML2)
if(TINYXML2_FOUND)
    include_directories(${TINYXML2_INCLUDE_DIR})
    target_link_libraries (${EXECUTABLE_NAME}  ${TINYXML2_LIBRARIES})
else()
    message(FATAL_ERROR "TinyXML2 not found")
endif()

find_package(Boost 1.65 REQUIRED serialization)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(${EXECUTABLE_NAME} ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "Boost serialization not found")
endif()
