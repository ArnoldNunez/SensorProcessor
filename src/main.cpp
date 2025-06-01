#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "EventBus.h"
#include "Eventing.h"
#include "addressbook.pb.h"

CoreMessaging::EventBus eventBus;

void doZeroMqStuff() {
  zmq::context_t ctx;
  zmq::socket_t sock1(ctx, zmq::socket_type::push);
  zmq::socket_t sock2(ctx, zmq::socket_type::pull);
  sock1.bind("tcp://127.0.0.1:*");

  const std::string last_endpoint = sock1.get(zmq::sockopt::last_endpoint);

  std::cout << "Connecting to " << last_endpoint << std::endl;
  sock2.connect(last_endpoint);

  tutorial::AddressBook addressBook;
  tutorial::Person person;
  person.set_name("John Doe");
  person.set_email("jdoe@example.com");
  tutorial::Person::PhoneNumber* phoneNumber = person.add_phones();
  phoneNumber->set_number("555-4321");
  phoneNumber->set_type(tutorial::Person::PHONE_TYPE_HOME);
  std::string personBinary;
  person.SerializeToString(&personBinary);

  std::array<zmq::const_buffer, 2> send_msgs = {
      zmq::str_buffer("foo"), zmq::buffer(personBinary, personBinary.length())};
  if (!zmq::send_multipart(sock1, send_msgs)) {
    return;
  }

  std::vector<zmq::message_t> recv_msgs;
  const auto ret = zmq::recv_multipart(sock2, std::back_inserter(recv_msgs));
  if (!ret) {
    return;
  }

  std::cout << "Got " << *ret << " messages" << std::endl;
  for (size_t i = 0; i < *ret; ++i) {
    tutorial::Person p = tutorial::Person();
    p.ParseFromArray(recv_msgs[i].data(), recv_msgs[i].size());

    std::cout << p.name().c_str() << std::endl;
    std::cout << p.DebugString() << std::endl;
  }

  // sock.bind("inproc://test");
  // sock.send(zmq::str_buffer("Hello World"), zmq::send_flags::dontwait);
}

void testRealClient() {
  zmq::context_t ctx;
  zmq::socket_t sock(ctx, zmq::socket_type::router);
  sock.bind("tcp://127.0.0.1:5556");

  std::vector<zmq::message_t> recv_msgs;
  const auto ret = zmq::recv_multipart(sock, std::back_inserter(recv_msgs));
  if (!ret) {
    return;
  }

  std::cout << "Got " << *ret << " messages" << std::endl;
  for (size_t i = 0; i < *ret; ++i) {
    tutorial::Person p = tutorial::Person();
    p.ParseFromArray(recv_msgs[i].data(), recv_msgs[i].size());

    // std::cout << p.name().c_str() << std::endl;
    std::cout << p.DebugString() << std::endl;
  }
}

void testRealClientFrames() {
  std::cout << "---------------------------" << std::endl;
  std::cout << "Test real client frames" << std::endl;
  std::cout << "---------------------------" << std::endl;

  zmq::context_t ctx;
  zmq::socket_t sock(ctx, zmq::socket_type::router);
  sock.bind("tcp://127.0.0.1:5556");

  while (true) {
    std::vector<zmq::message_t> frames;
    zmq::message_t message;
    zmq::recv_result_t result;

    result = sock.recv(message, zmq::recv_flags::none);
    if (result) {
      tutorial::Person p = tutorial::Person();
      p.ParseFromArray(message.data(), message.size());

      std::cout << p.DebugString() << std::endl;
    }

    // do {
    //     result = sock.recv(message, zmq::recv_flags::none);
    //     if (result)
    //     {
    //         frames.push_back(message);
    //     }
    // } while (sock.getsockopt<int>(ZMQ_RCVMORE));

    // // Process the received frames
    // for (const auto& frame: frames)
    // {
    //     tutorial::Person p = tutorial::Person();
    //     p.ParseFromArray(frame.data(), frame.size());

    //     std::cout << p.DebugString() << std::endl;
    // }
  }
}

void testClientPolling() {
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

        eventBus.processMessage(p);
      }
    }

    // TODO: Throttle this thread better.
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }

  std::cout << "Comms Exiting..." << std::endl;
  zmq_close(sock);
  zmq_ctx_term((void*)context);
}

int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

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

  // Register message types with the event bus
  std::cout << "Main: typeid:" << typeid(tutorial::Person).name() << std::endl;
  eventBus.registerTypeWithEvent(typeid(tutorial::Person),
                                 CoreMessaging::EventID::LOGIN_REQUEST);

  // doZeroMqStuff();
  // testRealClient();
  // testRealClientFrames();
  testClientPolling();
  return 0;
}