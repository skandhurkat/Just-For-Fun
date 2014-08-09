cmake_minimum_required (VERSION 2.6)
project (Library)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  message(FATAL_ERROR
      "This code compiles only on Windows")
endif()

include_directories(${PROJECT_SRC_DIR}/include)

subdirs(src)
