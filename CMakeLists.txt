cmake_minimum_required(VERSION 3.21)

project(
	HDLocalMap
	VERSION 1.0.3
	LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(XSEPlugin)