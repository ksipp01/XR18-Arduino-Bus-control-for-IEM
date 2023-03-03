# Arduiono WiFi control of XR18 Buses for IEM
OSC based browser control for XR 18 Bus 1-5 for Individual remote control if in ear monitor mix

Uses ESP-8266 wifi board to remote control Berhinger XR-18 (or X32) with OSC commands via UDP over wireless.  
Each bus has it's own browser page with 8 channels each.  Bidirectional control so changes on the mixer are displayed on the web page with slider movement. 
Browser slider changes and labels are assinged on the Arduino IDE
Has a Main Mute Button as well, used in case sudden feedback or allow discrete band member communication through IEM.  
All Mixer settings are stored in EEPROM so restored with each power up.  

Future:
Saved Presets for each Bus
Web browser setup page (may consume too much EEPROM so might not pursue)

Setup
Set Wifi SSID, IP and Pswd
Set UPD address (Mixer IP) and port 10023 for X32, 10024 for XR18
Define SLider channels and labels.  
Ensure you're onthe same WiFi as the mixer (Use stadn alnoe router for best results. (this goes for XR 18 in general!)
You can change the "Bus #" page headings if you want (search for *?* and Change the heading text
It'w possible to add another Page for Bus 6 (With some copy paste in the code)
Adding additional Channel sliders would be possible but require significnat coding changes. 

XR 18 OSC commands are foudn here: 
https://behringer.world/wiki/doku.php?id=x-air_osc

X32 
https://tostibroeders.nl/wp-content/uploads/2020/02/X32-OSC.pdf 
This document also very helpful if wanting to customize this code...
