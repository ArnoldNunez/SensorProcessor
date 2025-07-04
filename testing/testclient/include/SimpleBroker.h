/*
 * Author: Arnold Nunez
 * Project: SensorProcessor::TestClient
 * Module: TestClient
 */

#ifndef TEST_CLIENT_SIMPLE_BROKER_H
#define TEST_CLIENT_SIMPLE_BROKER_H

#include <atomic>
#include <functional>
#include <queue>
#include <thread>
#include <zmq.hpp>
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

  /*
   * Sneds the given message, note this transfers ownership of the
   * zmq::message.
   */
  void sendMsg(zmq::message_t&& msg);

  void registerLoginResponseCallback(const LoginResponseCallback& callback);

  void startComms();
  void stopComms();

 private:
  zmq::context_t mContext;
  std::thread mRecvThread;
  std::thread mSendThread;
  std::atomic<bool> mExitSignal;
  std::queue<zmq::message_t> mWorkItems;
  std::mutex mWorkLock;

  LoginResponseCallback mLoginResponseCallback;

  void recvWork();
  void sendWork();
};
}  // namespace TestClient
#endif