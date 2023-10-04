# 1 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino"
# 1 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino"
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. 
// To get started please visit https://microsoft.github.io/azure-iot-developer-kit/docs/projects/connect-iot-hub?utm_source=ArduinoExtension&utm_medium=ReleaseNote&utm_campaign=VSCode
# 5 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino" 2
# 6 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino" 2
# 7 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino" 2

# 9 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino" 2
# 10 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino" 2
# 11 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino" 2

static bool hasWifi = false;

static int soc;
static float voltage;
static float temperature;
static float humidity;

WiFiServer server(8080);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Utilities
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

static String JsonOutput()
{
    String jsonString = "{";

    jsonString += "\"battv\":" + String(voltage) + ",";
    jsonString += "\"humidity\":" + String(humidity) + ",";
    jsonString += "\"temp\":" + String(temperature);

    jsonString += "}";

    return jsonString;
}

static void HandleRequest(WiFiClient client)
{
    char messagePayload[256];
    bool success = readMessage(messagePayload, &temperature, &humidity, &voltage, &soc);

    if (success)
    {
        while (client.connected())
        {
            if (client.available())
            {
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: application/json;charset=utf-8");
                client.println("Connection: close");
                client.println();
                client.println(JsonOutput());
            }

            break;
        }

        delay(1000);
        client.stop();
        blinkSendConfirmation();
    }
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

  server.begin();
  do{{ (void)(0 && printf("Started")); { LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 120 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino" 3 4
 __null
# 120 "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino"
 ) l(AZ_LOG_INFO, "c:\\Users\\Shawn\\Documents\\IoTWorkbenchProjects\\examples\\devkit_getstarted\\Device\\GetStarted.ino", __func__, 120, 0x01, "Started"); } }; }while((void)0,0);
}

void loop()
{
    if (hasWifi)
    {
        WiFiClient client = server.available();
        if (client)
        {
            blinkLED();
            HandleRequest(client);
        }
    }
}
