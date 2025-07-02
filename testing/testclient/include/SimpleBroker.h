/*
 * Author: Arnold Nunez
 * Project: SensorProcessor::TestClient
 * Module: TestClient
 */

#ifndef TEST_CLIENT_SIMPLE_BROKER_H
#define TEST_CLIENT_SIMPLE_BROKER_H

#include <zmq.h>

#include <atomic>
#include <functional>
#include <thread>
#include <zmq_addon.hpp>

#include "Login.pb.h"

namespace TestClient {
/**
 * Simple broker to communicate with the applicatin in test.
 */
class SimpleBroker {
 public:
  typedef std::function<void(const CoreServices::LoginResponse&)>
      LoginResponseCallback;

  SimpleBroker();

  void sendMsg(const zmq::message_t& msg);

  void registerLoginResponseCallback(const LoginResponseCallback& callback);

  void startComms();
  void stopComms();

 private:
  zmq::context_t mContext;
  std::thread mRecvThread;
  std::thread mSendThread;
  std::atomic<bool> mExitSignal;

  LoginResponseCallback mLoginResponseCallback;

  void recvWork();
  void sendWork();
};
}  // namespace TestClient
#endif