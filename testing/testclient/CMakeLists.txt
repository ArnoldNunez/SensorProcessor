
cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 14)

message("Project TestClient")
project(TestClient VERSION 1.0)

find_package(Protobuf REQUIRED)

set(SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp

         # Generated
         ${CMAKE_SOURCE_DIR}/generated/addressbook.pb.cc
         ${CMAKE_SOURCE_DIR}/generated/Sensor.pb.cc
         ${CMAKE_SOURCE_DIR}/generated/SensorType.pb.cc

         ${CMAKE_SOURCE_DIR}/generated/DiagnosticsService/ServerInfo.pb.cc

         ${CMAKE_SOURCE_DIR}/generated/SessionService/Login.pb.cc

)

set(HDRS # Generated
         ${CMAKE_SOURCE_DIR}/generated/addressbook.pb.h
         ${CMAKE_SOURCE_DIR}/generated/Sensor.pb.h
         ${CMAKE_SOURCE_DIR}/generated/SensorType.pb.h

         ${CMAKE_SOURCE_DIR}/generated/DiagnosticsService/ServerInfo.pb.h

         ${CMAKE_SOURCE_DIR}/generated/SessionService/Login.pb.h
         
         # 3rd Party
         ${CMAKE_SOURCE_DIR}/external/nlohmann/json.hpp)

add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME} 
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/generated>
           $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/generated/DiagnosticsService>
           $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/generated/SessionService>
           $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/external/nlohmann>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
           
           # Headers to include when installing
           # implicitly prefixes with ${CMAKE_INSTALL_PREFIX}
           #$<INSTALL_INTERFACE:include> 
)

target_link_libraries(${PROJECT_NAME} ${PROTOBUF_LIBRARY} zmq)

install(TARGETS ${PROJECT_NAME} DESTINATION .)