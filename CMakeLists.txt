cmake_minimum_required(VERSION 3.00)

project(consoletest)

set(CMAKE_C_STANDARD 90)

if(WIN32)
    set(PLATFORM_SRC win32shell.c)
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_SRC linuxshell.c)
    find_package(Threads)
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -pthread")
elseif(UNIX AND APPLE)
    set(PLATFORM_SRC linuxshell.c)
else()
    set(PLATFORM_SRC)
endif()

# Test file
file(COPY input.txt DESTINATION ${CMAKE_BINARY_DIR})

# Library
ADD_LIBRARY( shellspawn STATIC shellspawn.h ${PLATFORM_SRC} )

# Test client app
add_executable(testclient testclient.c)

# Test Script 1
add_executable(shelltest shelltest.c shellspawn.h ${PLATFORM_SRC})
TARGET_LINK_LIBRARIES(shelltest shellspawn)

# Test Script 2
add_executable(noconsoletest noconsoletest.c shellspawn.h ${PLATFORM_SRC})
TARGET_LINK_LIBRARIES(noconsoletest shellspawn)