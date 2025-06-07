/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: Communication
 */

#include "ClientBroker.h"

#include <ctime>
#include <iostream>
#include <thread>
#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "EventBus.h"
#include "SineWaveSensor.h"
#include "addressbook.pb.h"

//
// Message buildup example
//
// tutorial::AddressBook addressBook;
// tutorial::Person person;
// person.set_name("John Doe");
// person.set_email("jdoe@example.com");
// tutorial::Person::PhoneNumber* phoneNumber = person.add_phones();
// phoneNumber->set_number("555-4321");
// phoneNumber->set_type(tutorial::Person::PHONE_TYPE_HOME);
// std::cout << person.DebugString() << std::endl;

// // Register message types with the event bus
// std::cout << "Main: typeid:" << typeid(tutorial::Person).name() <<
// std::endl; eventBus.registerTypeWithEvent(typeid(tutorial::Person),
//                                CoreMessaging::EventID::LOGIN_REQUEST);

// doZeroMqStuff();
// testRealClient();
// testRealClientFrames();
// testClientPolling();

// void doZeroMqStuff() {
//   zmq::context_t ctx;
//   zmq::socket_t sock1(ctx, zmq::socket_type::push);
//   zmq::socket_t sock2(ctx, zmq::socket_type::pull);
//   sock1.bind("tcp://127.0.0.1:*");

//   const std::string last_endpoint = sock1.get(zmq::sockopt::last_endpoint);

//   std::cout << "Connecting to " << last_endpoint << std::endl;
//   sock2.connect(last_endpoint);

//   tutorial::AddressBook addressBook;
//   tutorial::Person person;
//   person.set_name("John Doe");
//   person.set_email("jdoe@example.com");
//   tutorial::Person::PhoneNumber* phoneNumber = person.add_phones();
//   phoneNumber->set_number("555-4321");
//   phoneNumber->set_type(tutorial::Person::PHONE_TYPE_HOME);
//   std::string personBinary;
//   person.SerializeToString(&personBinary);

//   std::array<zmq::const_buffer, 2> send_msgs = {
//       zmq::str_buffer("foo"), zmq::buffer(personBinary,
//       personBinary.length())};
//   if (!zmq::send_multipart(sock1, send_msgs)) {
//     return;
//   }

//   std::vector<zmq::message_t> recv_msgs;
//   const auto ret = zmq::recv_multipart(sock2, std::back_inserter(recv_msgs));
//   if (!ret) {
//     return;
//   }

//   std::cout << "Got " << *ret << " messages" << std::endl;
//   for (size_t i = 0; i < *ret; ++i) {
//     tutorial::Person p = tutorial::Person();
//     p.ParseFromArray(recv_msgs[i].data(), recv_msgs[i].size());

//     std::cout << p.name().c_str() << std::endl;
//     std::cout << p.DebugString() << std::endl;
//   }

//   // sock.bind("inproc://test");
//   // sock.send(zmq::str_buffer("Hello World"), zmq::send_flags::dontwait);
// }

// void testRealClient() {
//   zmq::context_t ctx;
//   zmq::socket_t sock(ctx, zmq::socket_type::router);
//   sock.bind("tcp://127.0.0.1:5556");

//   std::vector<zmq::message_t> recv_msgs;
//   const auto ret = zmq::recv_multipart(sock, std::back_inserter(recv_msgs));
//   if (!ret) {
//     return;
//   }

//   std::cout << "Got " << *ret << " messages" << std::endl;
//   for (size_t i = 0; i < *ret; ++i) {
//     tutorial::Person p = tutorial::Person();
//     p.ParseFromArray(recv_msgs[i].data(), recv_msgs[i].size());

//     // std::cout << p.name().c_str() << std::endl;
//     std::cout << p.DebugString() << std::endl;
//   }
// }

// void testRealClientFrames() {
//   std::cout << "---------------------------" << std::endl;
//   std::cout << "Test real client frames" << std::endl;
//   std::cout << "---------------------------" << std::endl;

//   zmq::context_t ctx;
//   zmq::socket_t sock(ctx, zmq::socket_type::router);
//   sock.bind("tcp://127.0.0.1:5556");

//   while (true) {
//     std::vector<zmq::message_t> frames;
//     zmq::message_t message;
//     zmq::recv_result_t result;

//     result = sock.recv(message, zmq::recv_flags::none);
//     if (result) {
//       tutorial::Person p = tutorial::Person();
//       p.ParseFromArray(message.data(), message.size());

//       std::cout << p.DebugString() << std::endl;
//     }

//     // do {
//     //     result = sock.recv(message, zmq::recv_flags::none);
//     //     if (result)
//     //     {
//     //         frames.push_back(message);
//     //     }
//     // } while (sock.getsockopt<int>(ZMQ_RCVMORE));

//     // // Process the received frames
//     // for (const auto& frame: frames)
//     // {
//     //     tutorial::Person p = tutorial::Person();
//     //     p.ParseFromArray(frame.data(), frame.size());

//     //     std::cout << p.DebugString() << std::endl;
//     // }
//   }
// }

// void testClientPolling() {
//   std::cout << "---------------------------" << std::endl;
//   std::cout << "Test Client Polling" << std::endl;
//   std::cout << "---------------------------" << std::endl;

//   zmq::context_t context;
//   zmq::socket_t sock(context, zmq::socket_type::router);
//   sock.bind("tcp://127.0.0.1:5556");

//   // Initialzie Poll Set
//   int numSockets = 1;
//   zmq::pollitem_t items[numSockets] = {{
//       sock,        // Socket to poll on
//       0,           // OR, natife file handle to poll on
//       ZMQ_POLLIN,  // Eevents to poll on
//       0            // Events returned after poll
//   }};
//   while (true) {
//     zmq::message_t message;
//     zmq::poll(&items[0], numSockets, std::chrono::milliseconds{-1});

//     if (items[0].revents & ZMQ_POLLIN) {
//       // Grab data from the socket
//       zmq::recv_result_t result = sock.recv(message, zmq::recv_flags::none);
//       if (result) {
//         // std::cout << "Processing message" << std::endl;
//         // process message
//         tutorial::Person p = tutorial::Person();
//         p.ParseFromArray(message.data(), message.size());
//         std::cout << p.DebugString() << std::endl;

//         eventBus.processMessage(p);
//       }
//     }

//     // TODO: Throttle this thread better.
//     std::this_thread::sleep_for(std::chrono::milliseconds(5));
//   }

//   std::cout << "Comms Exiting..." << std::endl;
//   zmq_close(sock);
//   zmq_ctx_term((void*)context);
// }

namespace CoreMessaging {

//-----
ClientBroker::ClientBroker() : mClientConfig(), mServerConfig() {}

//-----
ClientBroker::~ClientBroker() {}

//-----
void ClientBroker::testClientPolling(EventBus* eventBus) {
  std::cout << "---------------------------" << std::endl;
  std::cout << "Test Client Polling" << std::endl;
  std::cout << "---------------------------" << std::endl;

  zmq::context_t context;
  zmq::socket_t sock(context, zmq::socket_type::router);
  sock.bind("tcp://127.0.0.1:5556");

  // Initialzie Poll Set
  int numSockets = 1;
  zmq::pollitem_t items[numSockets] = {{
      sock,        // Socket to poll on
      0,           // OR, natife file handle to poll on
      ZMQ_POLLIN,  // Eevents to poll on
      0            // Events returned after poll
  }};
  long long elapsedTime = 0;
  while (true) {
    zmq::message_t message;
    zmq::poll(&items[0], numSockets, std::chrono::milliseconds{-1});

    if (items[0].revents & ZMQ_POLLIN) {
      // Grab data from the socket
      zmq::recv_result_t result = sock.recv(message, zmq::recv_flags::none);
      if (result) {
        // std::cout << "Processing message" << std::endl;
        // process message
        tutorial::Person p = tutorial::Person();
        p.ParseFromArray(message.data(), message.size());
        std::cout << p.DebugString() << std::endl;

        eventBus->processMessage(p);
      }
    }

    // Test push out sensor value
    SimSensors::SineWaveSensor sensor(1.0f, 2);
    float sample = sensor.getSample(elapsedTime);

    Communication::Sensors::SensorValueScalar valueMsg;
    valueMsg.set_sensorid(1);
    valueMsg.add_values(sample);
    google::protobuf::Timestamp* timestamp = valueMsg.mutable_timestamp();
    valueMsg.mutable_timestamp()->set_seconds(std::time(nullptr));
    valueMsg.mutable_timestamp()->set_nanos(0);

    std::string valueBinary;
    valueMsg.SerializeToString(&valueBinary);

    int result = zmq_send(sock, &valueBinary, valueBinary.size(), 0);
    if (result <= 0) {
      std::cout << "Failed to send sensor value" << std::endl;
    }
    std::cout << "Send Result: " << std::to_string(result) << std::endl;

    // TODO: Throttle this thread better.
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    elapsedTime += 5;
  }

  std::cout << "Comms Exiting..." << std::endl;
  zmq_close(sock);
  zmq_ctx_term((void*)context);
}

void ClientBroker::testClientPush() {
  zmq::context_t context;
  zmq::socket_t sockSender(context, zmq::socket_type::push);
  sockSender.connect("tcp://172.19.112.1:5558");
  // sockSender.connect("tcp://0.0.0.0:5558");

  long long elapsedTime = 0;
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Test push out sensor value
    SimSensors::SineWaveSensor sensor(1.0f, 2);
    float sample = sensor.getSample(elapsedTime);

    Communication::Sensors::SensorValueScalar valueMsg;
    valueMsg.set_sensorid(1);
    valueMsg.add_values(sample);
    google::protobuf::Timestamp* timestamp = valueMsg.mutable_timestamp();
    valueMsg.mutable_timestamp()->set_seconds(std::time(nullptr));
    valueMsg.mutable_timestamp()->set_nanos(0);

    std::string valueBinary;
    valueMsg.SerializeToString(&valueBinary);

    // std::cout << "Sending data" << std::endl;
    // // int result = zmq_send(sockSender, &valueBinary, valueBinary.size(),
    // 0); int result = zmq_send(sockSender, "Test", 4, 0); if (result <= 0) {
    //   std::cout << "Failed to send sensor value" << std::endl;
    // }
    // std::cout << "Send Result: " << std::to_string(result) << std::endl;

    // char bytes[1000] = {0};
    // valueMsg.SerializeToArray(&bytes, 1000);
    // zmq::message_t msg(bytes, 1000);
    zmq::message_t msg(valueBinary);

    zmq::send_result_t res = sockSender.send(msg, zmq::send_flags::none);
    if (res.has_value()) {
      std::cout << "Sent " << std::to_string(res.value()) << " bytes"
                << std::endl;
    }

    elapsedTime += 1000;
  }
}

void ClientBroker::testClientPollingPull(EventBus* eventBus) {
  std::cout << "---------------------------" << std::endl;
  std::cout << "Test Client Polling (PULL)" << std::endl;
  std::cout << "---------------------------" << std::endl;

  zmq::context_t context;
  zmq::socket_t sock(context, zmq::socket_type::pull);
  zmq::socket_t sockSender(context, zmq::socket_type::push);
  sock.bind("tcp://127.0.0.1:5557");
  sockSender.connect("tcp://127.0.0.1:5558");

  long long elapsedTime = 0;

  // Initialzie Poll Set
  int numSockets = 1;
  zmq::pollitem_t items[numSockets] = {{
      sock,        // Socket to poll on
      0,           // OR, natife file handle to poll on
      ZMQ_POLLIN,  // Eevents to poll on
      0            // Events returned after poll
  }};
  while (true) {
    zmq::message_t message;
    zmq::poll(&items[0], numSockets, std::chrono::milliseconds{0});

    if (items[0].revents & ZMQ_POLLIN) {
      // Grab data from the socket
      zmq::recv_result_t result = sock.recv(message, zmq::recv_flags::none);
      if (result) {
        // std::cout << "Processing message" << std::endl;
        // process message
        tutorial::Person p = tutorial::Person();
        p.ParseFromArray(message.data(), message.size());
        std::cout << p.DebugString() << std::endl;

        eventBus->processMessage(p);

        std::string personBinary;
        p.SerializeToString(&personBinary);
      }
    }

    // Test push out sensor value
    SimSensors::SineWaveSensor sensor(1.0f, 2);
    float sample = sensor.getSample(elapsedTime);

    Communication::Sensors::SensorValueScalar valueMsg;
    valueMsg.set_sensorid(1);
    valueMsg.add_values(sample);
    google::protobuf::Timestamp* timestamp = valueMsg.mutable_timestamp();
    valueMsg.mutable_timestamp()->set_seconds(std::time(nullptr));
    valueMsg.mutable_timestamp()->set_nanos(0);

    std::string valueBinary;
    valueMsg.SerializeToString(&valueBinary);

    int result = zmq_send(sockSender, &valueBinary, valueBinary.size(), 0);
    if (result <= 0) {
      std::cout << "Failed to send sensor value" << std::endl;
    }
    std::cout << "Send Result: " << std::to_string(result) << std::endl;

    // TODO: Throttle this thread better.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    elapsedTime += 5;
  }

  std::cout << "Comms Exiting..." << std::endl;
  zmq_close(sock);
  zmq_ctx_term((void*)context);
}

//-----
void ClientBroker::testPublishSensor(
    const Communication::Sensors::Sensor& sensor) {}

//-----
void ClientBroker::testPublishSensorValue(
    const Communication::Sensors::SensorValueScalar& value) {}

//-----
void ClientBroker::setConfiguration(const AppConfig& config) {
  mClientConfig = config.getClientConfig();
  mServerConfig = config.getServerConfig();
}

}  // namespace CoreMessaging
