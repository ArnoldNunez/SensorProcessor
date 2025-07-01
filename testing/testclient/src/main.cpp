/*
 * Author: Arnold Nunez
 * Project: SensorProcessor::TestClient
 * Module: TestClient
 */

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <zmq.hpp>
#include <zmq_addon.hpp>

/**
 * The Main entry point.
 * \param argc No command line parameters supported.
 * \param argv No command line parametesr supported.
 */
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  //   GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::cout << "TestClient::main" << std::endl;

  return 0;
}