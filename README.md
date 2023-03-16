# Arduino controlled XR18 Buses for IEM
OSC based browser control for XR 18 Aux/Bus 1-5 for Individual remote control of in-ear monitor mix

Uses ESP-8266 wifi (or Mega 2560 with Ethernet Sheild) board to remote control Behringer XR-18 (or X32) with OSC commands via UDP over wireless or Ethernet.  
Each bus has it's own browser page with 8 channels each.  Bidirectional control so changes on the mixer are displayed on the web page with slider movement. 
Browser slider changes and labels are assinged on the Arduino IDE.
Has a Main Mute Button as well in case sudden feedback or allow to discrete band member communication through IEM.  
All Mixer fader settings are automatically loaded from the Mixer at startup.  

Setup
Set Wifi SSID, IP and Pswd or Ether IP, gateway and subnet. 
Set UPD address (Mixer IP) and UDP port 10023 for X32, 10024 for XR18
Define Slider channels and labels.  
Ensure you're on the same WiFi/LAN as the mixer (Use a stand alone router for best results, this goes for XR 18 in general!)
You can change the "Bus #" page headings if you want 
It's possible to add another Page for Bus 6 (With some copy paste in the code)
Adding additional Channel sliders would be possible but require significant coding changes. 

Testing can be done with the X32 Emulator (here: https://sites.google.com/site/patrickmaillot/x32  )
The code posted here is setup for that, you'll have to change the IP and Port to use with XR18

XR 18 OSC commands are found here: 
https://behringer.world/wiki/doku.php?id=x-air_osc

X32 
https://tostibroeders.nl/wp-content/uploads/2020/02/X32-OSC.pdf 
This document also very helpful if wanting to customize this code...

Thank you Thomas Friedrichsmeier (the EmbAJAX author) and Patrick Maillot (Author of X32 Emulator and many others) for their assistance.  



![Capture](https://user-images.githubusercontent.com/13811600/225424743-89e8388e-198c-4afd-9edb-cdfaa8221acb.JPG)
