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
      mLoginResponseCallback(),
      mWorkItems(),
      mWorkLock() {}

//-----
void SimpleBroker::sendMsg(zmq::message_t&& msg) {
  std::cout << "SimpleBroker::sendMsg" << std::endl;

  mWorkLock.lock();
  mWorkItems.emplace(std::move(msg));
  mWorkLock.unlock();
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
  mExitSignal.store(true, std::memory_order_relaxed);

  if (mRecvThread.joinable()) {
    mRecvThread.join();
  }
  if (mSendThread.joinable()) {
    mSendThread.join();
  }

  mContext.close();
}

//-----
void SimpleBroker::recvWork() {
  const std::string address = "tcp://127.0.0.1:5558";
  zmq::socket_t recvSock(mContext, zmq::socket_type::pull);
  recvSock.bind(address);

  std::cout << "SimpleBroker:: listening on: " << address << std::endl;

  // Initialzie Poll Set
  int numSockets = 1;
  zmq::pollitem_t items[numSockets] = {{
      recvSock,    // Socket to poll on
      0,           // OR, natife file handle to poll on
      ZMQ_POLLIN,  // Eevents to poll on
      0            // Events returned after poll
  }};
  long long elapsedTime = 0;
  while (!mExitSignal.load(std::memory_order_relaxed)) {
    zmq::message_t message;
    // zmq::poll(&items[0], numSockets, std::chrono::milliseconds{-1});
    zmq::poll(&items[0], numSockets, std::chrono::milliseconds{0});

    if (items[0].revents & ZMQ_POLLIN) {
      // Grab data from the socket
      zmq::recv_result_t result = recvSock.recv(message, zmq::recv_flags::none);
      if (result) {
        // Process message

        std::cout << "Received message" << std::endl;
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  recvSock.close();
}

//-----
void SimpleBroker::sendWork() {
  const std::string address = "tcp://127.0.0.1:5557";
  zmq::socket_t sendSock(mContext, zmq::socket_type::push);

  std::cout << "send connect string: " << address << std::endl;

  sendSock.connect(address);
  std::cout << "SimpleBroker:: connected to " << address << std::endl;

  while (!mExitSignal.load(std::memory_order_relaxed)) {
    zmq::message_t msg;

    // Read from work queue
    mWorkLock.lock();

    if (mWorkItems.size() == 0) {
      mWorkLock.unlock();

      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      continue;
    }

    msg.copy(mWorkItems.front());
    mWorkItems.pop();
    mWorkLock.unlock();

    // send zeromq message
    std::cout << "SimpleBroker::sending message" << std::endl;
    sendSock.send(msg, zmq::send_flags::none);
  }

  sendSock.close();
}

}  // namespace TestClient
