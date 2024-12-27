#ifndef MHWIFISERVER_H
#define MHWIFISERVER_H

#include "../../include/net_access.h"
#include "WString.h"
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// Variaveis #defines abaixo deverao estar no arquivo global.h
#define MH_ID "MH321"
#define UDP_PORT 5555
#define WEB_PORT 80
#define MOSTRA_MEMORIA 1

class MHWiFiServer {
 private:
#ifdef MOSTRA_MEMORIA
  uint32_t memRAM_ant = ESP.getFreeHeap();  // system_get_free_heap_size();
  uint32_t memRAM, memRAM_min;
#endif

  char _localIP[20] = "";

  // UDP
  WiFiUDP _wiFiUDP;

  String defaultHTML();

  // forms HTML
  void desenhoSet(String *pQuery);
  void frequenciaSet(String *pQuery);
  void ledIntensidadeSet(String *pQuery);
  void ledRGBSet(String *pQuery);
  void motor_off();
  void motor_on();

 public:
  MHWiFiServer();

  void conectaWifi();
  void handle();
  bool listenUDP();

#ifdef MOSTRA_MEMORIA
  void printMemoria(const char *pMsg = NULL);
#endif
};

#endif