/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: SensorProcessorService
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "json.hpp"

class AppConfig {
 public:
  /**
   * Configuration information for a Node.
   */
  struct NodeConfig {
    /**
     * Constructor.
     */
    NodeConfig() : mName(""), mIp(""), mRecvPort(0) {}

    // Name to associate with the Node
    std::string mName;

    // IP address associated with the Node
    std::string mIp;

    // Receive port associated with the Node
    unsigned short mRecvPort;
  };

  /**
   * Creates a config object from the given json
   * \param json      The json object to parse.
   * \param outConfig The output app config resulting from the parsing.
   * \return True on successfull parsing, false otherwise.
   */
  static bool fromJson(const nlohmann::json& json, AppConfig& outConfig);

  /**
   * Constructor
   */
  AppConfig();

  /**
   * Copy Constructor.
   * \param appConfig The app config to copy from.
   */
  AppConfig(const AppConfig& appConfig);

  /**
   * Destructor
   */
  ~AppConfig();

  /**
   * Helper that converst the config to a pretty printed
   * string.
   */
  std::string toStdString() const;

  /**
   * Getter for the client configuration
   * \return  Node configuration for the client
   */
  NodeConfig getClientConfig() const;

  /**
   * Getter for the server configuration
   * \return  Node configuration for the server
   */
  NodeConfig getServerConfig() const;

 private:
  /// @brief Configuration information for the client node.
  NodeConfig mClientConfig;

  /// @brief Configuration information for the server node.
  NodeConfig mServerConfig;
};

#endif  // APP_CONFIG_H