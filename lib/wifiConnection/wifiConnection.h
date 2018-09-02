#include "config.h"
#include <writeText.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char *ssid = "PoneyOnTheFly - 2ghz";
const char *password = "chachouetloulou";

// TCP server at port 80 will respond to HTTP requests
WiFiServer server(80);
IPAddress ip;

void wifiSetupAndDisplay()
{
  WiFi.mode(WIFI_STA);
  sprintf(text, "Connecting to %s", ssid);
  len = strlen(text);
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    displayText(text);
    delay(ANIM_DELAY);
  }
  // Start TCP (HTTP) server
  server.begin();
  ip = WiFi.localIP();
  String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
  strcpy(text, ipStr.c_str());
  len = strlen(text);
}

void confirmWifiConnection()
{
  displayText(text);
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  // Wait for data from client to become available
  while (client.connected() && !client.available())
  {
    displayText(text);
    delay(1);
  }

  // Read the first line of HTTP request
  String req = client.readStringUntil('\r');

  // First line of HTTP request looks like "GET /path HTTP/1.1"
  // Retrieve the "/path" part by finding the spaces
  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);
  if (addr_start == -1 || addr_end == -1)
  {
    return;
  }
  req = req.substring(addr_start + 1, addr_end);
  client.flush();

  String s;
  String answer;
  int pos;
  if (req.indexOf('?') > 0)
  {

    //Change url tags to text
    answer = req.substring(req.indexOf('=') + 1);
    answer.replace('+', ' ');
    //Conver HTML URL Encode to Text:
    //https://www.w3schools.com/tags/ref_urlencode.asp
    answer.replace("%21", "\!");
    answer.replace("%22", "\"");
    answer.replace("%23", "\#");
    answer.replace("%24", "\$");
    answer.replace("%25", "%");
    answer.replace("%26", "&");
    answer.replace("%27", "\'");
    answer.replace("%28", "\(");
    answer.replace("%29", "\)");
    answer.replace("%2B", "\+");
    answer.replace("%2C", "\,");
    answer.replace("%2D", "\-");
    answer.replace("%2E", "\.");
    answer.replace("%2F", "\/");
    answer.replace("%3A", "\:");
    answer.replace("%3B", "\;");
    answer.replace("%3C", "\<");
    answer.replace("%3D", "\=");
    answer.replace("%3E", "\>");
    answer.replace("%3F", "\?");
    answer.replace("%40", "\@");
    answer.replace("%5B", "\[");
    answer.replace("%5C", "\\");
    answer.replace("%5D", "\]");
    answer.replace("%5E", "\^");
    answer.replace("%7B", "\{");
    answer.replace("%7C", "\|");
    answer.replace("%7D", "\}");

    strcpy(text, answer.c_str());
    len = strlen(text);
    x = LEDMATRIX_WIDTH;
    lmd.clear();

    //Serial.println(req.substring(req.indexOf('=')+1));
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML><style> #header{ min-height: 20px; background-color: #666699; } #menu{ min-height: 20px; margin-top: 1%; background-color: #999999; } #body{ min-height: 200px; margin-top: 1%; } #footer{ min-height: 20px; margin-top: 1%; background-color: #666699; } #header, #menu, #body, #footer{ margin-left: 10%; margin-right: 10%; box-shadow: 3px 5px 7px #666666; border: 1px solid black; } @viewport{ zoom: 1.0; width: extend-to-zoom; } @-ms-viewport{ width: extend-to-zoom; zoom: 1.0; } </style> <html lang='en'> <head> <meta name='viewport' content='width=device-width, initial-scale=1'> <title>ESP8266 Web Server</title> </head> <body> <div id='header'><center><h1>Welcome to the ESP8266 Web Server</H1></center></div>";
    s += " <div id='menu'><center><H2>Printing to the 4 - 8x8 Matrix</h2></center></div> ";
    s += "<div id='body'><center><div><div><br/><H3>";
    s += answer;
    s += "<p><a href='./'>Back</a></p>";
    s += "</H3></div></div> </center> </div> <div id='footer'> </div> </body></html> ";
    //s+=req.substring(req.indexOf('=')+1);
    s + "</body></html>\r\n\r\n";
    client.print(s);
    return;
  }
  else
  {

    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    s += "<!doctype html> <style> \#header{ min-height: 20px; background-color: \#666699; } \#menu{ min-height: 20px; margin-top: 1%; background-color: \#999999; } \#body{ min-height: 200px; margin-top: 1%; } \#footer{ min-height: 20px; margin-top: 1%; background-color: \#666699; } \#header, \#menu, \#body, \#footer{ margin-left: 10%; margin-right: 10%; box-shadow: 3px 5px 7px \#666666; border: 1px solid black; } @viewport{ zoom: 1.0; width: extend-to-zoom; } @-ms-viewport{ width: extend-to-zoom; zoom: 1.0; } </style> <html lang='en'> <head> <meta name='viewport' content='width=device-width, initial-scale=1'> <title>ESP8266 Web Server</title> </head> <body> <div id='header'><center><h1>Welcome to the ESP8266 Web Server</H1></center></div> <div id='menu'><center><H2>Enter text which will be displayed on a 4 - 8x8 led matrix</h2></center></div> <div id='body'><center><div><div><br> <form action='esp8266'> <br>Enter Text to display on LED Matrix:<br><input type='text' maxlength='70' name='max' value=''><br><br> <input type='submit' value='Submit'></form> </div></div> </center> </div> <div id='footer'> </div> </body>";
    s += "</html>\r\n\r\n";
    client.print(s);
    return;
  }
}
