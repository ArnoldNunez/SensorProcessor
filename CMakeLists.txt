
cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 14)

project(SensorProcessor VERSION 1.0)

find_package(Protobuf REQUIRED)

add_subdirectory(testing/testclient)

set(SRCS src/main.cpp
         src/CoreApplication.cpp
         src/AppConfig.cpp
         src/ClientBroker.cpp
         src/SineWaveSensor.cpp
         src/EventBus.cpp
         src/EventDispatcher.cpp

         # Core Services
         src/IService.cpp
         src/SessionService.cpp

         # Generated
         generated/addressbook.pb.cc
         generated/Sensor.pb.cc
         generated/SensorType.pb.cc

         generated/DiagnosticsService/ServerInfo.pb.cc

         generated/SessionService/Login.pb.cc

)

set(HDRS include/CoreApplication.h
         include/AppConfig.h
         include/ClientBroker.h
         include/SineWaveSensor.h
         include/EventBus.h
         include/EventDispatcher.h
         include/Eventing.h

         # Core Services
         include/IService.h
         include/SessionService.h

         # Generated
         generated/addressbook.pb.h
         generated/Sensor.pb.h
         generated/SensorType.pb.h

         generated/DiagnosticsService/ServerInfo.pb.h

         generated/SessionService/Login.pb.h
         
         # 3rd Party
         external/nlohmann/json.hpp)

add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME} 
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/generated>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/generated/DiagnosticsService>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/generated/SessionService>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/nlohmann>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
           

           # Headers to include when installing
           # implicitly prefixes with ${CMAKE_INSTALL_PREFIX}
           #$<INSTALL_INTERFACE:include> 
)

target_link_libraries(${PROJECT_NAME} ${PROTOBUF_LIBRARY} zmq)

install(TARGETS ${PROJECT_NAME} DESTINATION .)
install(FILES resources/config/AppConfig.json DESTINATION . COMPONENT config)