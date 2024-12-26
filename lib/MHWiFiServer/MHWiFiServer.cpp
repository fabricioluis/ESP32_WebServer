#include "MHWiFiServer.h"

WiFiServer server(80);

MHWiFiServer::MHWiFiServer()
{
  memRAM = memRAM_min = ESP.getFreeHeap();
}

void MHWiFiServer::conectaWifi()
{
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

void MHWiFiServer::handle()
{
  WiFiClient client = server.available();

  if (client) {
    delay(500);

    String queryString = "";
    // String currentLine = "";
    char c;

    int posHTTP = 0;

    int i = 0;
    while (client.connected()) {
      c = client.read();
      queryString += c;

      // informa a posicao da string " HTTP/1.1".
      posHTTP = queryString.indexOf(" HTTP/1.1");

      if (posHTTP >= 0) {
        Serial.printf("queryString:%s\n", queryString.c_str());
        int pos_barra = queryString.indexOf(" /") + 1;
        queryString = queryString.substring(pos_barra, posHTTP);

        // Faz o tratamento de acordo com o formulario escolhido.
        if (queryString.indexOf("frequenciaSet") > 0) {
          frequenciaSet(&queryString);
        }
        else if (queryString.indexOf("desenhoSet") > 0) {
          desenhoSet(&queryString);
        }
        else if (queryString.indexOf("ledIntensidadeSet") > 0) {
          ledIntensidadeSet(&queryString);
        }
        else if (queryString.indexOf("ledRGBSet") > 0) {
          ledRGBSet(&queryString);
        }

        break;
      }
    }

    // Imprime pagina defaultHTML.
    client.println(defaultHTML().c_str());
    client.stop();

    printMemoria();
  }  // if (client) {
}

String MHWiFiServer::defaultHTML()
{
  String response = "<!DOCTYPE html><html><head><meta charset='UTF-8'></head>";
  response += "<center><h3> ESP32 - Wifi - WebServer</ h3></center><br>\n ";

  // Frequencia (/frequenciaSet?hertz=1000)
  response += " <form action='/frequenciaSet' accept-charset='utf-8'>";
  response += "Frequencia:<input type='text' name='hertz' size='4' value='1000'>";
  response += "<input type='submit' value='ok'></form>\n";

  // Desenho (/desenhoSet?nr=1)
  response += " <form action='/desenhoSet'>";
  response += "Desenho:<input type='text' name='nr' size='2' value='1'>";
  response += "<input type='submit' value='ok'></form>\n";

  // LED Intensidade (/ledSetIntensidade?nr=100)
  response += " <form action='/ledIntensidadeSet'>";
  response += "LED Intensidade:<input type='text' name='nr' size='3' value='50'>";
  response += "<input type='submit' value='ok'></form>\n";

  // LED Cor (/ledSetRGB?nr=100)
  response += " <form action='/ledRGBSet'>";
  response += "Red:<input type='text' name='R' size='3' value='127'>";
  response += "Green:<input type='text' name='G' size='3' value='128'>";
  response += "Blue:<input type='text' name='B' size='3' value='129'>";
  response += "<input type='submit' value='ok'></form>\n";

  response += "<a href='/'>Inicio</a><br>\n";
  response += "</html>";
  return response;
}

// ---------------------------- //
// Faz o tratamento dos <forms> //
// ---------------------------- //

// trata o form /desenhoSet
void MHWiFiServer::desenhoSet(String *pQuery)
{
  // "/desenhoSet?nr=" 15
  String a = pQuery->substring(15, pQuery->length());
  Serial.printf("(%s)\n", a.c_str());
}

// trata o form /frequenciaSet
void MHWiFiServer::frequenciaSet(String *pQuery)
{
  // "/frequenciaSet?hertz=" 21
  String a = pQuery->substring(21, pQuery->length());
  Serial.printf("(%s)\n", a.c_str());
}

// trata o form /ledSetIntensidade
void MHWiFiServer::ledIntensidadeSet(String *pQuery)
{
  // "/ledIntensidadeSet?nr=" 22
  String a = pQuery->substring(22, pQuery->length());
  Serial.printf("(%s)\n", a.c_str());
}

// trata o form /ledRGBSet
void MHWiFiServer::ledRGBSet(String *pQuery)
{
  // "/ledRGBSet?R=127&G=128&B=129"
  String R = pQuery->substring(pQuery->indexOf("R="), pQuery->indexOf("&G"));
  String G = pQuery->substring(pQuery->indexOf("G="), pQuery->indexOf("&B"));
  String B = pQuery->substring(pQuery->indexOf("B="), pQuery->length());
  Serial.printf("(%s,%s,%s)\n", R.c_str(), G.c_str(), B.c_str());
}

// ---------------------------- //
// Faz o tratamento dos <forms> //
// ---------------------------- //

void MHWiFiServer::printMemoria(const char *pMsg)
{
  // https://github.com/esp8266/esp8266-wiki/wiki/Memory-Map

  // Verifica se a memoria RAM diminui.
  memRAM = ESP.getFreeHeap();  // system_get_free_heap_size();

  // armazena sempre a minima de RAM.
  if (memRAM_min > memRAM)
    memRAM_min = memRAM;

  if (pMsg)
    Serial.printf(
        "\nMin.:(%d) %d-%d=%d %s\n", memRAM_min, memRAM_ant, memRAM, (memRAM_ant - memRAM), pMsg);
  else
    Serial.printf("\nMin.:(%d) %d-%d=%d\n", memRAM_min, memRAM_ant, memRAM, (memRAM_ant - memRAM));

  // system_print_meminfo();
  // system_show_malloc();
  memRAM_ant = memRAM;
}
