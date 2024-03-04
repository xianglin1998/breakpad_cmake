# Ref from https://github.com/Mendeley/breakpad/blob/master/mendeley/CMakeLists.txt

cmake_minimum_required(VERSION 3.5)
project(breakpad)

set(BREAKPAD_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/breakpad/src)

if(WIN32)
	ADD_DEFINITIONS(-DUNICODE)
	ADD_DEFINITIONS(-D_UNICODE)
	add_definitions(-D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")

    include_directories(
		${BREAKPAD_SRC_DIR} 
		${BREAKPAD_SRC_DIR}/third_party/windows/include 
	)

    set(CLIENT_SRC_DIR ${BREAKPAD_SRC_DIR}/client/windows)
    set(BREAKPAD_LIB_SRCS
		${BREAKPAD_SRC_DIR}/common/windows/guid_string.cc
		${BREAKPAD_SRC_DIR}/common/windows/string_utils.cc

		${CLIENT_SRC_DIR}/handler/exception_handler.cc
		${CLIENT_SRC_DIR}/crash_generation/client_info.cc
		${CLIENT_SRC_DIR}/crash_generation/crash_generation_client.cc
		${CLIENT_SRC_DIR}/crash_generation/crash_generation_server.cc
		${CLIENT_SRC_DIR}/crash_generation/minidump_generator.cc
	)
elseif(APPLE)
	message(ERROR "Unsupported")
elseif(UNIX)
	message(ERROR "Unsupported")
endif()

# breakpad - client library for capturing minidumps when a crash occurs
SET(LIBRARY_OUTPUT_PATH ${BREAKPAD_SRC_DIR})  # Output library to src dir.
add_library(libbreakpad STATIC ${BREAKPAD_LIB_SRCS})
target_link_libraries(libbreakpad)
find_library(LIBBREAKPAD libbreakpad ${LIBRARY_OUTPUT_PATH})
