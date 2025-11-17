cmake_minimum_required(VERSION 3.10)
project(NetPackets)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(${PROJECT_NAME} STATIC
    ${SRC_DIR}/Packet.cpp
)

target_link_libraries(${PROJECT_NAME} sfml-network)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})