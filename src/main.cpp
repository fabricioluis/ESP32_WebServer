#include "net_access.h"
#include <WebServer.h>
#include <WiFi.h>

int ct = 0;

uint32_t memRAM_anterior = ESP.getFreeHeap();  // system_get_free_heap_size();
uint32_t memRAM;

WiFiServer server(80);

String defaultHTML();

void handle();

/*
void contador();
void teste();
*/

void printMemoria(const char *pMsg = NULL);

void setup()
{
  Serial.begin(115200);
  delay(1000);

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
  delay(1000);
}

void loop()
{
  handle();
}

void handle()
{
  WiFiClient client = server.available();

  if (client) {
    Serial.printf("III: %s\n\n", client.remoteIP().toString());
    delay(500);

    String queryString = "";
    // String currentLine = "";
    char c;

    int posHTTP = 0;

    while (client.connected()) {
      c = client.read();
      queryString += c;
      // Serial.write(c);

      // informa a posicao da string " HTTP/1.1".
      posHTTP = queryString.indexOf(" HTTP/1.1");

      if (posHTTP >= 0) {
        int pos_barra = queryString.indexOf(" /") + 1;
        queryString = queryString.substring(pos_barra, posHTTP);

        Serial.printf("\nqueryString:%s\n", queryString.c_str());

        client.println(defaultHTML().c_str());
        break;
      }

      /*
            if (c == '\n') {
              // se for final de transmissao HTML vem com \n.
              if (currentLine.length() == 0) {
                client.println(defaultHTML().c_str());
                break;
              }
              else {
                currentLine = "";
              }  // if (currentLine.length() == 0) {
            }
            else if (c != '\r') {
              currentLine += c;
            }  // if (c == '\n') {
      */
    }

    client.stop();
    Serial.println("FFF - Cliente desconectado.");

    printMemoria();
  }
}  // if (client) {

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

/*
void contador()
{
  String response = "Contador: ";
  response += ct++;
  server.send(200, "text/plain", response);

  // printMemoria();
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

  String response = "queryString: /teste - acessada com a query string: " + queryString;
  server.send(200, "text/html", response);
  //  server.send(200, "text/plain", response);

  // printMemoria();
}

*/