/*
 * Author: Arnold Nunez
 * Project: SensorProcessor::TestClient
 * Module: TestClient
 */

#include <google/protobuf/stubs/common.h>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "SimpleBroker.h"
#include "cli.h"

/**
 * The Main entry point.
 * \param argc No command line parameters supported.
 * \param argv No command line parametesr supported.
 */
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  int major = 0;
  int minor = 0;
  int patch = 0;
  zmq_version(&major, &minor, &patch);

  std::cout << "Using ZMQ: " << std::to_string(major) << "."
            << std::to_string(minor) << "." << std::to_string(patch)
            << std::endl;

  TestClient::SimpleBroker broker;
  TestClient::CLI cli(&broker);

  broker.startComms();

  std::string commands = R"(=== Commands ===
login: Send login request
q: quit the application)";
  std::cout << commands << std::endl;
  std::string input;
  while (std::cin >> input) {
    if (input == "login") {
      cli.requestLogin("username", "password");
    } else if (input == "q") {
      break;
    } else {
      std::cout << "Not a valid command" << std::endl;

      std::cout << commands << std::endl;
    }
  }

  // stop comms
  broker.stopComms();

  return 0;
}