#include "Arduino.h"
#include "MHWiFiServer.h"

MHWiFiServer *mhwfs;

void setup()
{
  Serial.begin(115200);
  delay(500);

  mhwfs = new MHWiFiServer();
  mhwfs->conectaWiFi();
}

void loop()
{
  mhwfs->handle();
}
