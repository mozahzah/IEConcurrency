# SPDX-License-Identifier: GPL-2.0-only
# Copyright © Interactive Echoes. All rights reserved.
# Author: mozahzah

cmake_minimum_required(VERSION 3.29)
project(IEConcurrencyExample VERSION 1.0.0 LANGUAGES CXX)

message("\n------------------------------------------------------------")
message("${PROJECT_NAME}\n")

add_executable(${PROJECT_NAME} "./main.cpp")
target_link_libraries(${PROJECT_NAME} PUBLIC IEConcurrency)

set_target_properties(${PROJECT_NAME} PROPERTIES 
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

message("------------------------------------------------------------\n")