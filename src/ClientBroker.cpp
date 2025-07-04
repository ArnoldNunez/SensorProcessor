/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: Communication
 */

#include "ClientBroker.h"

#include <ctime>
#include <iostream>
#include <thread>
#include <zmq_addon.hpp>

#include "CoreApplication.h"
#include "EventBus.h"
#include "SineWaveSensor.h"
#include "addressbook.pb.h"

namespace CoreMessaging {

//-----
ClientBroker::ClientBroker()
    : mClientConfig(),
      mServerConfig(),
      mRecvThread(),
      mSendThread(),
      mContext(1),
      mExitSignal(false) {}

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
    SimSensors::SineWaveSensor sensor(10.0f, 2);
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
void ClientBroker::startCommunication() {
  mRecvThread = std::thread(&ClientBroker::receiveWork, this);
  mSendThread = std::thread(&ClientBroker::sendWork, this);

  mRecvThread.join();
  mSendThread.join();

  mContext.close();
}

//-----
void ClientBroker::setConfiguration(const AppConfig& config) {
  mClientConfig = config.getClientConfig();
  mServerConfig = config.getServerConfig();
}

//-----
void ClientBroker::receiveWork() {
  zmq::socket_t recvSock(mContext, zmq::socket_type::pull);
  recvSock.set(zmq::sockopt::linger, 0);

  std::string recvConnStr = "tcp://" + mServerConfig.mIp + ":" +
                            std::to_string(mServerConfig.mRecvPort);
  std::cout << "ClientBroker:: listening on: " << recvConnStr << std::endl;

  recvSock.bind(recvConnStr);

  long long elapsedTime = 0;

  // Initialzie Poll Set
  int numSockets = 1;
  zmq::pollitem_t items[numSockets] = {{
      recvSock,    // Socket to poll on
      0,           // OR, natife file handle to poll on
      ZMQ_POLLIN,  // Eevents to poll on
      0            // Events returned after poll
  }};
  while (true) {
    // Check exit signal to exit gracefully.
    if (CoreApplication::EXIT_SIGNAL.load()) {
      break;
    }

    // Poll on the socket
    zmq::message_t message;
    zmq::poll(&items[0], numSockets, std::chrono::milliseconds{0});

    if (items[0].revents & ZMQ_POLLIN) {
      // Grab data from the socket
      zmq::recv_result_t result = recvSock.recv(message, zmq::recv_flags::none);
      if (result) {
        std::cout << "ClientBroker:: received message" << std::endl;
        // eventBus->processMessage(p);
      }
    }
  }

  recvSock.close();
}

//-----
void ClientBroker::sendWork() {
  zmq::socket_t sockSender(mContext, zmq::socket_type::push);
  sockSender.set(zmq::sockopt::linger, 0);

  std::string sendConnStr = "tcp://" + mClientConfig.mIp + ":" +
                            std::to_string(mClientConfig.mRecvPort);

  std::cout << "ClientBroker:: connected to: " << sendConnStr << std::endl;

  sockSender.connect(sendConnStr);

  while (true) {
    // Check exit signal to exit gracefully.
    if (CoreApplication::EXIT_SIGNAL.load()) {
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // FOR TESTING PURPOSES
    // std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    // std::string msgStr = "TestMessage";
    // zmq::message_t msg(msgStr);
    // sockSender.send(msg, zmq::send_flags::none);
  }

  sockSender.close();
}

}  // namespace CoreMessaging
