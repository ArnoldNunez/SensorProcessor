
cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 14)

project(SensorProcessor VERSION 1.0)

find_package(Protobuf REQUIRED)

set(SRCS src/main.cpp
         src/SineWaveSensor.cpp

         # Generated
         generated/addressbook.pb.cc
)

set(HDRS include/SineWaveSensor.h

         # Generated
         generated/addressbook.pb.h)

add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME} 
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/generated>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>

           # Headers to include when installing
           # implicitly prefixes with ${CMAKE_INSTALL_PREFIX}
           #$<INSTALL_INTERFACE:include> 
)

target_link_libraries(${PROJECT_NAME} ${PROTOBUF_LIBRARY} zmq)

install(TARGETS ${PROJECT_NAME} DESTINATION .)