cmake_minimum_required(VERSION 3.16)

project(steam_sdk LANGUAGES CXX)

set(STEAM_SDK_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")

if(WIN32)
    set(STEAM_SDK_PLATFORM "windows")
elseif(APPLE)
    set(STEAM_SDK_PLATFORM "macos")
else()
    set(STEAM_SDK_PLATFORM "linux")
endif()

if(WIN32)
    set(STEAM_SDK_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/${STEAM_SDK_PLATFORM}" CACHE PATH "Path to STEAM_SDK shared library")
    set(STEAM_SDK_DLL "${STEAM_SDK_LIB_DIR}/steam_api64.dll" CACHE PATH "Path to STEAM_SDK shared library")
elseif(APPLE)
    set(STEAM_SDK_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/${STEAM_SDK_PLATFORM}" CACHE PATH "Path to STEAM_SDK shared library")
    set(STEAM_SDK_DLL "${STEAM_SDK_LIB_DIR}/libsteam_api.dylib" CACHE PATH "Path to STEAM_SDK shared library")
else()
    set(STEAM_SDK_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/${STEAM_SDK_PLATFORM}" CACHE PATH "Path to STEAM_SDK shared library")
    set(STEAM_SDK_DLL "${STEAM_SDK_LIB_DIR}/libsteam_api.so" CACHE PATH "Path to STEAM_SDK shared library")
endif()

if(NOT EXISTS "${STEAM_SDK_DLL}")
    message(FATAL_ERROR "STEAM SDK library not found: ${STEAM_SDK_DLL}")
endif()

add_library(${PROJECT_NAME} SHARED IMPORTED GLOBAL)

set_target_properties(${PROJECT_NAME} PROPERTIES
    IMPORTED_LOCATION "${STEAM_SDK_DLL}"
    INTERFACE_INCLUDE_DIRECTORIES "${STEAM_SDK_INCLUDE_DIR}"
    STEAM_RUNTIME_DLL "${STEAM_SDK_DLL}"
)

message(STATUS "STEAM SDK: ${STEAM_SDK_DLL}, includes: ${STEAM_SDK_INCLUDE_DIR}")