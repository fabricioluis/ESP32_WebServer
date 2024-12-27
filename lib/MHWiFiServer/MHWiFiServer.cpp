#include "MHWiFiServer.h"

WiFiServer server(80);

MHWiFiServer::MHWiFiServer()
{
#ifdef MOSTRA_MEMORIA
  memRAM = memRAM_min = ESP.getFreeHeap();
#endif
}

void MHWiFiServer::conectaWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(NET_SSID, NET_PASSWORD);
  delay(500);
  Serial.print("Conectando ");

  int wifi_ct = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);

    wifi_ct++;
    if (wifi_ct == 40) {
      wifi_ct = 0;
      Serial.println(" ");
    }
  }

  strcpy(_localIP, WiFi.localIP().toString().c_str());

  _wiFiUDP.begin(UDP_PORT);

  server.begin();

  Serial.printf("\nConectado - IP: %s:%d / (UDP:%d)\n", _localIP, WEB_PORT, UDP_PORT);

  delay(500);
}

void MHWiFiServer::handle()
{
  if (listenUDP()) {
#ifdef MOSTRA_MEMORIA
    printMemoria();
#endif
  }

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
        if (queryString.indexOf("motor_off") > 0) {
          motor_off();
        }
        else if (queryString.indexOf("motor_on") > 0) {
          motor_on();
        }
        else if (queryString.indexOf("frequenciaSet") > 0) {
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

#ifdef MOSTRA_MEMORIA
    printMemoria();
#endif
  }  // if (client) {
}

String MHWiFiServer::defaultHTML()
{
  String response = "<!DOCTYPE html><html><head><meta charset='UTF-8'></head>";
  response += "<center><h3> ESP32 - Wifi - WebServer</ h3></center><br>\n ";

  // Motor - Funcionalidades
  response += " <a href='/motor_off'>Desliga</a> ";
  response += " <a href='/motor_on'>Liga</a>";

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

void MHWiFiServer::motor_off()
{
  Serial.printf("Motor Desligado.\n");
}

void MHWiFiServer::motor_on()
{
  Serial.printf("Motor Ligado.\n");
}

// ---------------------------- //
// Faz o tratamento dos <forms> //
// ---------------------------- //

/*
 * UDP
 * listenUDP()
 * Faz o tratamento do protocolo UDP para o NodeMCU via porta (UDP_PORT).
 * Devolve para quem pediu o IP local(_localIP) do NodeMCU.
 * Retorna true se houve tratamento de pacotes UDP, caso contrario false.
 */
bool MHWiFiServer::listenUDP()
{
  char _incomingPacket[256] = "";
  uint8_t *_replyPacket = new uint8_t[41];
  bool retorno = false;

  int packetSize = _wiFiUDP.parsePacket();

  if (packetSize) {
    // receive incoming UDP packets
    Serial.printf("Recebe %d bytes do IP:%s na porta %d\n",
                  packetSize,
                  _wiFiUDP.remoteIP().toString().c_str(),
                  _wiFiUDP.remotePort());

    int len = _wiFiUDP.read(_incomingPacket, 255);

    if (len > 0)
      _incomingPacket[len] = 0;

    Serial.printf("Conteudo UDP: %s\n", _incomingPacket);

    // send back a reply, to the IP address and port we got the packet from
    _wiFiUDP.beginPacket(_wiFiUDP.remoteIP(), _wiFiUDP.remotePort());
    strcpy((char *)_replyPacket, _localIP);

    // Se a string de envio comecar com "MH", entao sera adicionado a serial da Mesa.
    if (strcmp(_incomingPacket, "MH") == 0) {
      strcat((char *)_replyPacket, " - ");
      strcat((char *)_replyPacket, MH_ID);
    }

    int tam_replayPackey = strlen((char *)_replyPacket);
    _replyPacket[tam_replayPackey] = '\0';
    _wiFiUDP.write(_replyPacket, tam_replayPackey);
    _wiFiUDP.endPacket();

    retorno = true;
  }

  free(_replyPacket);

  return retorno;
}

#ifdef MOSTRA_MEMORIA
/*
 * Mostra a memoria do MicroControlador e também informa o valor minimo.
 */
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
#endif
