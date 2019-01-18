#include <NeoPixelBus.h>

#include <WiFiManager.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define NUMPIXELS 3

WiFiManager wifiManager;
ESP8266WebServer server(5000);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> pixels(NUMPIXELS);

void setPixels(int r, int g, int b)
{
    for (int i = 0; i < NUMPIXELS; ++i)
    {
        pixels.SetPixelColor(i, RgbColor(r, g, b));
    }
    pixels.Show();
}

void handleRoot()
{
    Serial.println("handleRoot started");
    Serial.println(server.argName);
    Serial.println(server.hasArg("r"));
    Serial.println(server.hasArg("data"));
    Serial.println(server.arg("r"));

    setPixels(server.arg("r").toInt(),server.arg("g").toInt(),server.arg("b").toInt());
    
    server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void setup(void)
{
    Serial.begin(115200);
    Serial.println("");

    pixels.Begin();
    setPixels(255,0,0);
    pixels.Show();

    wifiManager.autoConnect("AutoConnectAP");

    if (MDNS.begin("esp8266"))
    {
        Serial.println("MDNS responder started");
    }

    Serial.println("registering handleRoot");
    server.on("/", handleRoot);

    Serial.println("registering handleNotFound");
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
}

void loop(void)
{
    server.handleClient();
    MDNS.update();
}