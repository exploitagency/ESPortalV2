#ESPortalV2  
  
-----  
HELP  
-----  
  
ESPortalV2  
  
Created by Corey Harding  
www.LegacySecurityGroup.com / www.Exploit.Agency  
https://github.com/exploitagency/ESPortalV2  
  
ESPortal is a WiFi Captive Portal Credential Harvester for ESP8266 Devices.  
  
ESPortal is distributed under the MIT License. The license and copyright notice can not be removed and must be distributed alongside all future copies of the software.  
  
-----  
Accessing ESPortal Main Menu  
-----  
  
SSID: "Free WiFi"  
URL:  http://192.168.1.1/esportal  
  
-----  
Configure ESPortal  
-----  
  
Default credentials to access the configuration page:  
username "admin"  
password "hacktheplanet"  
  
WiFi Configuration  
  
Network Type  
Access Point Mode: Create a standalone access point(No Internet Connectivity-Requires Close Proximity)  
Join Existing Network: Join an existing network(Possible Internet Connectivity-Could use Device Remotely)  
  
Hidden: Choose whether or not to use a hidden SSID when creating an access point  
  
SSID: SSID of the access point to create or of the network you are choosing to join  
Password: Password of the access point which you wish to create or of the network you are choosing to join  
Channel: Channel of the access point you are creating  
  
IP: IP to set for ESPortal  
Gateway: Gateway to use, make it the same as ESPortal's IP if an access point or the same as the router if joining a network  
Subnet: Typically set to 255.255.255.0  
  
ESPortal Administration Settings:  
  
Username: Username to configure/upgrade ESPortal  
Password: Password to configure/upgrade ESPortal  
  
-----  
ESPortal Credential Harvester  
-----  
  
A social engineering attack vector.  
Redirects HTTP requests to a fake login page. 
Does not support HTTPS requests nor does it override cached HTTPS redirects.  
You can define a custom template for up to 5 specific domains, a welcome portal, and a catch-all.  
  
Example scenario: Setup ESPortalV2 to act as a free WiFi hotspot.  
  
Captured credentials are stored on the exfiltration page in the file corresponding to the URL.  
  
Custom html templates can be uploaded for the ESPortal login credential harvester via FTP.  
If a custom html template is found it will override the default settings.  
Upon deletion the default settings are automatically restored.  
  
The filenames must match the below exactly in order to apply a template override.  
captiveportal.html     -     The catch all that handles the redirects.(Would be rare to override this part)  
welcome.html           -     The welcome page for the "free wifi" hotspot.  
spoof_other.html       -     The generic login credential harvester for a site not in the list.  
spoof_site1.html       -     The 1st login credential harvester site in the list with a custom layout.  
spoof_site2.html       -     The 2nd login credential harvester site in the list with a custom layout.  
spoof_site3.html       -     The 3rd login credential harvester site in the list with a custom layout.  
error.html             -     Display some sort of custom error when the user enters login credentials.  
  
NOTE: Modifying any ESPortal related setting requires a reboot of the ESPortal device.  
Do not leave any line blank or as a duplicate of another.  
  
-----  
List Exfiltrated Data  
-----  
  
Displays any data that has been collected from the victim using ESPortal's exfiltration methods.  
  
-----  
Format File System  
-----  
  
This will erase the contents of the SPIFFS file system including ALL Payloads that have been uploaded.  
Formatting may take up to 90 seconds.  
All current settings will be retained unless you reboot your device during this process.  
  
-----  
Upgrade ESPortal Firmware  
-----  
  
Authenticate using your username and password set in the configuration page.  
  
Default credentials to access the firmware upgrade page:  
username "admin"  
password "hacktheplanet"  
  
Select "Browse" choose the new firmware to be uploaded and then click "Upgrade".  
  
You will need to manually reset the device upon the browser alerting you that the upgrade was successful.  
  
-----  
Licensing Information  
-----  
  
ESPortalV2 by Corey Harding: https://www.LegacySecurityGroup.com  
Code available at: https://github.com/exploitagency/ESPortalV2  
ESPortalV2 software is licensed under the MIT License  
/*  
 MIT License  
  
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
