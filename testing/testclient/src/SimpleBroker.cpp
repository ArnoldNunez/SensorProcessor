/*
 * Author: Arnold Nunez
 * Project: SensorProcessor::TestClient
 * Module: TestClient
 */

#include "SimpleBroker.h"

namespace TestClient {
//-----
SimpleBroker::SimpleBroker()
    : mContext(),
      mRecvThread(),
      mSendThread(),
      mExitSignal(false),
      mLoginResponseCallback() {}

//-----
void SimpleBroker::sendMsg(const zmq::message_t& msg) {
  std::cout << "SimpleBroker::sendMsg" << std::endl;
}

//-----
void SimpleBroker::registerLoginResponseCallback(
    const LoginResponseCallback& callback) {
  mLoginResponseCallback = callback;
}

//-----
void SimpleBroker::startComms() {
  mRecvThread = std::thread(&SimpleBroker::recvWork, this);
  mSendThread = std::thread(&SimpleBroker::sendWork, this);
}

void SimpleBroker::stopComms() {
  mExitSignal.store(false, std::memory_order_relaxed);

  if (mRecvThread.joinable()) {
    mRecvThread.join();
  }
  if (mSendThread.joinable()) {
    mSendThread.join();
  }

  mContext.close();
}

//-----
void SimpleBroker::recvWork() {}

//-----
void SimpleBroker::sendWork() {}

}  // namespace TestClient
