/*
 * Author: Arnold Nunez
 * Project: SensorProcessor::TestClient
 * Module: TestClient
 */

#ifndef TEST_CLIENT_CLI_H
#define TEST_CLIENT_CLI_H

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "Login.pb.h"

namespace TestClient {
class SimpleBroker;

/**
 * Class representing the command line interface for
 * using the test client.
 */
class CLI {
 public:
  /**
   * Constructor.
   */
  CLI(SimpleBroker* broker);

  /**
   * send out login request message
   */
  void requestLogin(const std::string& username, const std::string& password);

 private:
  /// @brief Service broker
  SimpleBroker* mBroker;

  /**
   * Sets up the callbacks for messages from the broker.
   */
  void registerBrokerListeners();

  /**
   * Helper that takes a protobuf message and converts it to
   * a zmq message
   */
  template <typename ProtoMsgT>
  void toZmqMsg(const ProtoMsgT& msg, zmq::message_t& zmsg) {
    std::string str;
    msg.SerializeToString(&str);
    zmq::message_t msgTemp(str);
    zmsg.copy(msgTemp);
  }

  /**
   * Helper to print out the contents of a protobuf message.
   */
  template <typename ProtoMsgT>
  void printMsg(const ProtoMsgT& msg) {
    std::cout << "=== " << msg.GetDescriptor()->name() << " ===" << std::endl;
    std::cout << msg.DebugString() << std::endl;
  }
};
}  // namespace TestClient

#endif