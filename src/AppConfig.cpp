/*
 * Author: Arnold Nunez
 * Project: SensorProcessor
 * Module: SensorProcessorService
 */

#include "AppConfig.h"

//-----
AppConfig::AppConfig() : mClientConfig(), mServerConfig() {}

//-----
AppConfig::AppConfig(const AppConfig& appConfig) {
  mClientConfig = appConfig.getClientConfig();
  mServerConfig = appConfig.getServerConfig();
}

//-----
AppConfig::~AppConfig() {}

//-----
std::string AppConfig::toStdString() const {
  std::ostringstream outStream;

  outStream << " ===== AppConfig =====" << std::endl;
  outStream << "- Client Config -" << std::endl;
  outStream << "     name: " << mClientConfig.mName << std::endl;
  outStream << "     ip: " << mClientConfig.mIp << std::endl;
  outStream << "     recv port: " << mClientConfig.mRecvPort << std::endl;
  outStream << "- Server Config -" << std::endl;
  outStream << "     name: " << mServerConfig.mName << std::endl;
  outStream << "     ip: " << mServerConfig.mIp << std::endl;
  outStream << "     recv port: " << mServerConfig.mRecvPort << std::endl;
  outStream << "=======================" << std::endl;

  return outStream.str();
}

//-----
AppConfig AppConfig::defaultConfig() {
  AppConfig defaultConfig;
  defaultConfig.mClientConfig.mIp = "127.0.0.1";
  defaultConfig.mClientConfig.mName = "Client";
  defaultConfig.mClientConfig.mRecvPort = 5558;

  defaultConfig.mClientConfig.mIp = "127.0.0.1";
  defaultConfig.mClientConfig.mName = "server";
  defaultConfig.mClientConfig.mRecvPort = 5558;

  return defaultConfig;
}

//-----
bool AppConfig::fromJson(const nlohmann::json& json, AppConfig& outConfig) {
  AppConfig result;

  // TODO: Error checking.

  NodeConfig clientConfig;
  clientConfig.mName =
      json.at("settings").at("communication").at("client").at("name");
  clientConfig.mIp =
      json.at("settings").at("communication").at("client").at("ip");
  clientConfig.mRecvPort =
      json.at("settings").at("communication").at("client").at("receive_port");

  NodeConfig serverConfig;
  serverConfig.mName =
      json.at("settings").at("communication").at("server").at("name");
  serverConfig.mIp =
      json.at("settings").at("communication").at("server").at("ip");
  serverConfig.mRecvPort =
      json.at("settings").at("communication").at("server").at("receive_port");

  result.mClientConfig = clientConfig;
  result.mServerConfig = serverConfig;

  outConfig = result;

  return true;
}

//-----
AppConfig::NodeConfig AppConfig::getClientConfig() const {
  return mClientConfig;
}

//-----
AppConfig::NodeConfig AppConfig::getServerConfig() const {
  return mServerConfig;
}