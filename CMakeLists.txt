cmake_minimum_required(VERSION 3.15)

project(HuffmanCode VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDART 14)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(project_headers 
	include/HuffmanCode.h
)

set(project_sources 
	src/main.cpp
	src/HuffmanCode.cpp
)

add_executable(${PROJECT_NAME}
	${project_headers}
	${project_sources}
)