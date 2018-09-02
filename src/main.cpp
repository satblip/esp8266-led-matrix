#include <writeText.h>
#include <wifiConnection.h>
#include <otaUpdate.h>
#include "config.h"

void setup()
{
  textSetup();
  wifiSetupAndDisplay();
  otaInit();
}

void loop()
{
  otaHandle();
  confirmWifiConnection();
}
