#include <Arduino.h>
#line 1 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino"
#line 1 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino"
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. 
// To get started please visit https://microsoft.github.io/azure-iot-developer-kit/docs/projects/connect-iot-hub?utm_source=ArduinoExtension&utm_medium=ReleaseNote&utm_campaign=VSCode
#include "AZ3166WiFi.h"
#include "DevKitMQTTClient.h"

#include "config.h"
#include "utility.h"
#include "SystemTickCounter.h"

static bool hasWifi = false;

static uint64_t send_interval_ms;

static int soc;
static float voltage;
static float temperature;
static float humidity;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Utilities
#line 22 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino"
static void InitWifi();
#line 40 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino"
static void SendConfirmationCallback();
#line 57 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino"
void setup();
#line 81 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino"
void loop();
#line 22 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino"
static void InitWifi()
{
  Screen.print(2, "Connecting...");
  
  if (WiFi.begin() == WL_CONNECTED)
  {
    IPAddress ip = WiFi.localIP();
    Screen.print(1, ip.get_address());
    hasWifi = true;
    Screen.print(2, "Running... \r\n");
  }
  else
  {
    hasWifi = false;
    Screen.print(1, "No Wi-Fi\r\n ");
  }
}

static void SendConfirmationCallback()
{
  blinkSendConfirmation();

  IPAddress ip = WiFi.localIP();
  Screen.print(1, ip.get_address());
  char line1[20];
  sprintf(line1, "V:%.2f S:%d", voltage, soc); 
  Screen.print(2, line1);

  char line2[20];
  sprintf(line2, "T:%.2f H:%.2f", temperature, humidity);
  Screen.print(3, line2);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Arduino sketch
void setup()
{
  Screen.init();
  Screen.print(0, "PrydwenMX");
  Screen.print(2, "Initializing...");
  
  Screen.print(3, " > Serial");
  Serial.begin(115200);

  // Initialize the WiFi module
  Screen.print(3, " > WiFi");
  hasWifi = false;
  InitWifi();
  if (!hasWifi)
  {
    return;
  }

  Screen.print(3, " > Sensors");
  SensorInit();

  send_interval_ms = SystemTickCounterRead();
}

void loop()
{
  if (hasWifi)
  {
    if ((int)(SystemTickCounterRead() - send_interval_ms) >= getInterval())
    {
      // Send teperature data
      char messagePayload[MESSAGE_MAX_LEN];

      bool success = readMessage(messagePayload, &temperature, &humidity, &voltage, &soc);

      if (success)
      {
          char line[45];
          sprintf(line, "V:%.2f S:%d T:%.2f H:%.2f", voltage, soc, temperature, humidity);

          LogInfo(line);
      }

       
      send_interval_ms = SystemTickCounterRead();
    }
  }
  delay(60000);
}

