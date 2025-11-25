cmake_minimum_required(VERSION 3.16)
project(SoulOfNG LANGUAGES CXX)

if(MSVC)
    
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    
    add_compile_options(
        "$<$<CONFIG:Debug>:/MTd>"
        "$<$<CONFIG:Release>:/MT>"
    )
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "$ORIGIN")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_RPATH "$ORIGIN")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building in DEBUG mode")
    add_compile_definitions(DEBUG_MODE=1)
else()
    add_compile_definitions(DEBUG_MODE=0)
    message("Building in RELEASE mode")
endif()

if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif()

set(SHOULD_USE_EDITOR ON CACHE BOOL "Use editor or no")
set(NET_BUILD_SERVER ON CACHE BOOL "Build server or no")

message("Use editor: " ${SHOULD_USE_EDITOR})

set(SFML_INSTALL_PKGCONFIG_FILES FALSE CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES FALSE CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC FALSE CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE FALSE CACHE BOOL "" FORCE)
set(SFML_USE_STATIC_STD_LIBS TRUE CACHE BOOL "" FORCE)

add_subdirectory(external)
add_subdirectory(network/Packet)

if(NET_BUILD_SERVER)
    message("BUILDING SERVER")
    add_subdirectory(network/Server)
endif()

file(GLOB_RECURSE PROJECT_HEADERS  ${INCLUDE_DIR}/*.h ${INCLUDE_DIR}/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES ${SRC_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
        src/Physics/CollisionSystem.cpp
        include/Physics/CollisionSystem.hpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/external/SFML/include/)

target_link_libraries(${PROJECT_NAME} PUBLIC
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        sfml-network
        nlohmann_json
        NetPackets
        steam_sdk
)

#To copy resources folder inside build directory
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
set(OUTPUT_RESOURCE_DIR "${CMAKE_BINARY_DIR}/resources")

if(EXISTS ${RESOURCE_DIR})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${OUTPUT_RESOURCE_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
                "${RESOURCE_DIR}" 
                "${OUTPUT_RESOURCE_DIR}"
        COMMENT "Copying resources to build directory"
    )
    message(STATUS "Resources will be copied to: ${OUTPUT_RESOURCE_DIR}")
else()
    message(WARNING "Resource directory not found: ${RESOURCE_DIR}")
endif()
 
#USE_EDITOR=${SHOULD_USE_EDITOR} doesn't work for some reason
if(SHOULD_USE_EDITOR)
    target_link_libraries(${PROJECT_NAME} PUBLIC ImGui-SFML::ImGui-SFML)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_EDITOR=1)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_EDITOR=0)
endif()

get_target_property(STEAM_DLL_PATH steam_sdk STEAM_RUNTIME_DLL)

if(EXISTS "${STEAM_DLL_PATH}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${STEAM_DLL_PATH}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMENT "Copying Steam API library"
    )

    install(FILES "${STEAM_DLL_PATH}"
        DESTINATION .
        COMPONENT Runtime
    )

else()
    message(WARNING "STEAM_DLL_PATH failed to find: ${STEAM_DLL_PATH}")
endif()

if(EXISTS ${RESOURCE_DIR})
    install(DIRECTORY ${RESOURCE_DIR}/
        DESTINATION resources
        USE_SOURCE_PERMISSIONS
    )
    message(STATUS "Resources will be installed to: resources/")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE SFML_STATIC)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE Dbghelp)
elseif(UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE -g)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-rdynamic")
endif()

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION .
    BUNDLE DESTINATION .
    LIBRARY DESTINATION lib
    COMPONENT Runtime
)   