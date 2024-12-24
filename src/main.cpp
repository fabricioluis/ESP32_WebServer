#include "net_access.h"
#include <WebServer.h>
#include <WiFi.h>

int ct = 0;

uint32_t memRAM_anterior = ESP.getFreeHeap();  // system_get_free_heap_size();
uint32_t memRAM;

WebServer server(80);

void contador();
void handle();
void teste();
void printMemoria(const char *pMsg = NULL);

void setup()
{
  Serial.begin(115200);
  WiFi.begin(NET_SSID, NET_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWiFi conectado\n");
  Serial.printf("IP: %s\n", WiFi.localIP().toString());

  server.on("/", handle);
  server.on("/ct", contador);
  server.on("/teste", teste);

  server.begin();

  Serial.printf("Server HTTP Ok.\n");
}

void loop()
{
  server.handleClient();
}

void handle()
{
  String response = "<html>\n<center><h3>ESP32 - Wifi - WebServer</h3></center><br>\n";
  response += "<a href='/ct'>Contador</a><br>\n";
  response += "<a href='/teste'>Teste</a><br>\n";
  response += "<a href='/'>Inicio</a><br>\n";
  response += "</html>";
  server.send(200, "text/html", response);

  printMemoria();
}

void contador()
{
  String response = "Contador: ";
  response += ct++;
  server.send(200, "text/plain", response);

  printMemoria();
}

// Função para lidar com a rota "/teste"
void teste()
{
  // Obter a query string
  String queryString = server.uri();
  int pos = queryString.indexOf('?');
  if (pos > 0) {
    queryString = queryString.substring(pos + 1);
  }

  // Extrair os parâmetros da query string
  // (Exemplo: usando a biblioteca ArduinoJson para parsear)
  // ...

  String response = "URL: /teste - acessada com a query string: " + queryString;
  server.send(200, "text/html", response);
  //  server.send(200, "text/plain", response);

  printMemoria();
}

void printMemoria(const char *pMsg)
{
  // https://github.com/esp8266/esp8266-wiki/wiki/Memory-Map

  // Verifica se a memoria RAM diminui.
  memRAM = ESP.getFreeHeap();  // system_get_free_heap_size();
  if (pMsg)
    Serial.printf(
        "\nMem.: %d-%d=%d %s\n", memRAM_anterior, memRAM, (memRAM_anterior - memRAM), pMsg);
  else
    Serial.printf("\nMem.: %d-%d=%d\n", memRAM_anterior, memRAM, (memRAM_anterior - memRAM));

  // system_print_meminfo();
  // system_show_malloc();
  memRAM_anterior = memRAM;
}