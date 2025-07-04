/*
 * Author: Arnold Nunez
 * Project: SensorProcessor::TestClient
 * Module: TestClient
 */

#include "cli.h"

#include <google/protobuf/util/time_util.h>

#include <string>
#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "SimpleBroker.h"

namespace TestClient {
//-----
CLI::CLI(SimpleBroker* broker) : mBroker(broker) {
  if (broker) {
    registerBrokerListeners();
  }
}

//-----
void CLI::requestLogin(const std::string& username,
                       const std::string& password) {
  CoreServices::LoginRequest request;
  request.set_username(username);
  request.set_password(password);
  request.mutable_timestamp()->set_seconds(time(NULL));
  request.mutable_timestamp()->set_nanos(0);

  zmq::message_t msg;
  toZmqMsg(request, msg);

  if (mBroker) {
    mBroker->sendMsg(std::move(msg));
  }
}

//-----
void CLI::registerBrokerListeners() {
  mBroker->registerLoginResponseCallback(
      [this](const CoreServices::LoginResponse& response) {
        printMsg(response);
      });
}
}  // namespace TestClient