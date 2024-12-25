#include "net_access.h"
#include <WebServer.h>
#include <WiFi.h>

int ct = 0;

uint32_t memRAM_anterior = ESP.getFreeHeap();  // system_get_free_heap_size();
uint32_t memRAM;

WiFiServer server(80);

String defaultHTML();

void handle();

void printMemoria(const char *pMsg = NULL);

void setup()
{
  Serial.begin(115200);
  delay(500);

  // WiFi.mode(WIFI_STA);
  WiFi.begin(NET_SSID, NET_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.printf("\nWiFi conectado\n");
  Serial.printf("IP: %s\n", WiFi.localIP().toString());

  server.begin();

  Serial.printf("Server HTTP Ok.\n");
  delay(500);
}

void loop()
{
  handle();
}

void handle()
{
  WiFiClient client = server.available();

  if (client) {
    delay(500);

    String queryString = "";
    // String currentLine = "";
    char c;

    int posHTTP = 0;

    while (client.connected()) {
      c = client.read();
      queryString += c;

      // informa a posicao da string " HTTP/1.1".
      posHTTP = queryString.indexOf(" HTTP/1.1");

      if (posHTTP >= 0) {
        int pos_barra = queryString.indexOf(" /") + 1;
        queryString = queryString.substring(pos_barra, posHTTP);

        Serial.printf("\nqueryString:%s\n", queryString.c_str());

        // Imprime pagina defaultHTML.
        client.println(defaultHTML().c_str());
        break;
      }
    }

    client.stop();

    printMemoria();
  }  // if (client) {
}

String defaultHTML()
{
  String response = "<html><center><h3>ESP32 - Wifi - WebServer</h3></center><br>\n";
  // Frequencia (/frequenciaSet?hertz=1000)
  response += " <form action='/frequenciaSet'>";
  response += "Frequencia:<input type='text' name='hertz' size='4' value='1000'>";
  response += "<input type='submit' value='ok'></form>\n";

  // Desenho (/desenhoSet?nr=1)
  response += " <form action='/desenhoSet'>";
  response += "Desenho:<input type='text' name='nr' size='2' value='1'>";
  response += "<input type='submit' value='ok'></form>\n";

  // LED Intensidade (/ledSetIntensidade?nr=100)
  response += " <form action='/ledSetIntensidade'>";
  response += "LED Intensidade:<input type='text' name='nr' size='3' value='50'>";
  response += "<input type='submit' value='ok'></form>\n";

  // LED Cor (/ledSetRGB?nr=100)
  response += " <form action='/ledSetRGB'>";
  response += "R:<input type='text' name='R' size='3' value='127'>";
  response += "G:<input type='text' name='G' size='3' value='128'>";
  response += "B:<input type='text' name='B' size='3' value='129'>";
  response += "<input type='submit' value='ok'></form>\n";

  response += "<a href='/'>Inicio</a><br>\n";
  response += "</html>";
  return response;
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
