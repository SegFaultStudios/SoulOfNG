cmake_minimum_required(VERSION 3.10)
project(NetServer)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME}
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/Server.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/external/SFML/include/)
target_link_libraries(${PROJECT_NAME} PUBLIC NetPackets sfml-network)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)