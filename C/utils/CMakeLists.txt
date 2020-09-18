#
#
# * Command Memo:
#   - cmake Configure
#   - cmake Build
#   - cmake Build Variables {Debug | Release | MinRelSize | RelWithDbgInfo}
#   - cmake Debug
#
# * Reference:
#   https://code.visualstudio.com/docs/cpp/cmake-linux

cmake_minimum_required(VERSION 3.8.0)
project(utils VERSION 0.1.0)

message("Searching Doxygen...")
find_package(Doxygen REQUIRED dot)
#find_package(Doxygen)
option(BUILD_DOCUMENTATION "Build doxygen documentation" ${DOXYGEN_FOUND})

if (DOXYGEN_FOUND)
    message("Doxygen found.")
    message("DOXYGEN_VERSION: " ${DOXYGEN_VERSION})
    message("DOXYGEN_EXECUTABLE: " ${DOXYGEN_EXECUTABLE})
    message("DOXYGEN_DOT_FOUND: " ${DOXYGEN_DOT_FOUND})
    message("DOXYGEN_DOT_EXECUTABLE: " ${DOXYGEN_DOT_EXECUTABLE})
    message("DOXYGEN_DOT_PATH: " ${DOXYGEN_DOT_PATH})

    #set(DOXYGEN_PROJECT_NAME "Project Name")
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/doxygen)
    set(DOXYGEN_OUTPUT_LANGUAGE English)
    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
    #set(DOXYGEN_INPUT ${CMAKE_SOURCE_DIR})
    set(DOXYGEN_INPUT_ENCODING UTF-8)
    #set(DOXYGEN_FILE_PATTERNS "*.c *.cpp *.h *.hxx *.hpp *.S *.asm")
    set(DOXYGEN_RECURSIVE YES)
    set(DOXYGEN_SOURCE_BROWSER YES)
    set(DOXYGEN_INLINE_SOURCES YES)
    set(DOXYGEN_REFERENCED_BY_RELATION YES)
    set(DOXYGEN_REFERENCES_RELATION YES)
    doxygen_add_docs(doxygen ${CMAKE_SOURCE_DIR})
else (DOXYGEN_FOUND)
    message("Please install doxygen package.")
endif (DOXYGEN_FOUND)

#include(CTest) 
#enable_testing()

#
# Setup Cmake Variables
#
set(CMAKE_VERBOSE_MAKEFILE "ON")
#set(BUILD_SHARED_LIBS "ON")

#
# Utility Library
#
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/..")

add_library(utils utils_hexdump.c utils_string.c)
set_target_properties(utils PROPERTIES PUBLIC_HEADER utils.h)

include(GNUInstallDirs)
install(TARGETS utils
        EXPORT utils-config
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include)

#
# Test Application
#
add_executable(utils_test utils_main.c hexdump_main.c blkcmp_main.c)
target_link_libraries(utils_test utils)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

message("********** CMAKE Variables **************************************************************")
message("PROJECT_NAME:    " ${PROJECT_NAME})
message("PROJECT_VERSION: " ${PROJECT_VERSION})
message("PROJECT_DESCRIPTION: " ${PROJECT_DESCRIPTION})
message("PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
message("PROJECT_BINARY_DIR: " ${PROJECT_BINARY_DIR})
message("*** CMAKE_SOURCE_DIR:                            " ${CMAKE_SOURCE_DIR})
message("*** CMAKE_BINARY_DIR:                            " ${CMAKE_BINARY_DIR})
message("*** CMAKE_INSTALL_LIBDIR:                        " ${CMAKE_INSTALL_LIBDIR})
message("*** CMAKE_INSTALL_INCLUDEDIR:                    " ${CMAKE_INSTALL_INCLUDEDIR})
message("*** CMAKE_CURRENT_SOURCE_DIR:                    " ${CMAKE_CURRENT_SOURCE_DIR})
message("*** CMAKE_CURRENT_BUILD_DIR:                     " ${CMAKE_CURRENT_BUILD_DIR})
message("*** BUILD_SHARED_LIBS:                           " ${BUILD_SHARED_LIBS})
message("*** CMAKE_BUILD_TYPE:                            " ${CMAKE_BUILD_TYPE})
message("*** CMAKE_INSTALL_PREFIX:                        " ${CMAKE_INSTALL_PREFIX})
message("*** CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT: " ${CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT})
message("*** CMAKE_PREFIX_PATH:                           " ${CMAKE_PREFIX_PATH})
message("*****************************************************************************************")
