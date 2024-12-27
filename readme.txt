ESP32_WEBSERVER

Autor: Fabricio Luis

Data: 2024-12-26

Projeto implementa camada de rede de WiFi para o ESP32-S3-DevKitC-1 para padronizar 
o uso e o acesso via Wifi com WebServer (HTTP:80) e UDPServer (5555).

DataSheet: https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-devkitc-1/user_guide.html

platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino

Classe: MHWiFiServer

Nesse projeto serao criados dois branchs:
- main - serah utilizado a classe WifiServer.
    Classe mais generica para conexao com a rede Wifi.
    
- WebServer - serah utilizado a classe WebServer (HTTP)
    mais especializada para HTTP.

