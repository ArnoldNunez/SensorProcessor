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

#include "BaseMessages.pb.h"
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
  CoreServices::Command command;
  command.set_commandid(CoreServices::COMMAND_ID_LOGIN_REQUEST);

  command.mutable_loginrequest()->set_username(username);
  command.mutable_loginrequest()->set_password(password);
  command.mutable_loginrequest()->mutable_timestamp()->set_seconds(time(NULL));
  command.mutable_loginrequest()->mutable_timestamp()->set_nanos(0);

  zmq::message_t msg;
  toZmqMsg(command, msg);

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