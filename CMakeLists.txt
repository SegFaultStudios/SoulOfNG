cmake_minimum_required(VERSION 3.16)
project(SoulOfNG LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SHOULD_USE_EDITOR ON CACHE BOOL "Use editor or no")

message("Use editor: " ${SHOULD_USE_EDITOR})

add_subdirectory(external)

file(GLOB_RECURSE PROJECT_HEADERS  ${INCLUDE_DIR}/*.h ${INCLUDE_DIR}/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES ${SRC_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/external/SFML/include/)

target_link_libraries(${PROJECT_NAME} PUBLIC
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        sfml-network
        nlohmann_json
)

if(SHOULD_USE_EDITOR)
    target_link_libraries(${PROJECT_NAME} PUBLIC ImGui-SFML::ImGui-SFML)
endif()

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

target_compile_definitions(${PROJECT_NAME} PRIVATE 
        USE_EDITOR=${SHOULD_USE_EDITOR}
)

if(EXISTS ${RESOURCE_DIR})
    install(DIRECTORY ${RESOURCE_DIR}/
        DESTINATION resources
        USE_SOURCE_PERMISSIONS
    )
    message(STATUS "Resources will be installed to: resources/")
endif()

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION .
    BUNDLE DESTINATION .
    LIBRARY DESTINATION lib
)