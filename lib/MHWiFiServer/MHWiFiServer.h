#ifndef MHWIFISERVER_H
#define MHWIFISERVER_H

#include "../../include/net_access.h"
#include "WString.h"
#include <WebServer.h>
#include <WiFi.h>

class MHWiFiServer {
 private:
  uint32_t memRAM_ant = ESP.getFreeHeap();  // system_get_free_heap_size();
  uint32_t memRAM, memRAM_min;
  String defaultHTML();

  // forms HTML
  void desenhoSet(String *pQuery);
  void frequenciaSet(String *pQuery);
  void ledIntensidadeSet(String *pQuery);
  void ledRGBSet(String *pQuery);

 public:
  MHWiFiServer();

  void conectaWifi();
  void handle();
  void printMemoria(const char *pMsg = NULL);
};

#endif