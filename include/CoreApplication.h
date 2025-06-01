/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: SensorProcessorService
 */

#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H

#include <thread>

/**
 * Class representing the Core application. This is
 * the entry point for the application.
 */
class CoreApplication {
 public:
  /**
   * Constructor
   */
  CoreApplication();

  /**
   * Destructor
   */
  ~CoreApplication();

  /**
   * Entry point of the application
   */
  void start();

  /**
   * Exit the application. Cleans up gracefully.
   */
  void exit();

 private:
  /**
   * The main application thread
   */
  std::thread mMainThread;
};

#endif  // CORE_APPLICATION_H