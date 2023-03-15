#include <Arduino.h>

#include <EmbAJAX.h>
//#define EmbAJAXOutputDriverWebServerClass EthernetWebServer 
//#include <EmbAJAXOutputDriverGeneric.h> 
#include <EmbAJAXValidatingTextInput.h>
#include <EmbAJAXScriptedSpan.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

EmbAJAXOutputDriverWebServerClass server(80); 
EmbAJAXOutputDriverESP8266 driver(&server);
//EmbAJAXOutputDriverGeneric driver(&server);

//convert slider number to approiate mixer channel
int sliderChan[8] = {16, 7, 8, 9, 11, 13, 14, 15};
bool synced = false;
class Mixer{
   public:
bool sliderChange = false;
int mch;
int mbus;
int mixerLevel;
};
Mixer mixer;
class Browser{
   public:
bool sliderChange = false;
int mch;
int mbus;
int mixerLevel;
};
Browser browser;

// *** Setup values***//
//String MixerIP = "192.168.3.2";
const IPAddress outIp(192, 168, 1, 212); // X32 emulator IP or XR18 IP 
const unsigned int outPort = 10023;       //10024 for XR18  //10023 for x32 // remote port to receive OSC
//const unsigned int localPort = 10023;  // NOT USED//
//const IPAddress outIp(192, 168, 2,3);   // IP of Mixer
//const unsigned int outPort = 10024;       //10024 for XR18  //10023 for x32 // remote port to receive OSC
//const unsigned int localPort = 10024;   //  NOT USED //10024 or XR18    // 10023 for X32local port to listen for OSC packets 

#define STASSID "WifiNode"
#define STAPSK "lola1124"

//#define STASSID "NETGEAR07"
//#define STAPSK "SelmerEffect"


// Set Static IP address
IPAddress local_IP(192, 168, 1, 222);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

#ifndef STASSID

#endif
const char* ssid = STASSID;
const char* password = STAPSK;

//add foe UDP rec
WiFiUDP Udp; 




char packet[255];
char reply[] = "Packet received!";

//pg1
EmbAJAXSlider ch1B1("ch1B1", 0,  1000, 0);   
EmbAJAXSlider ch2B1("ch2B1", 0,  1000, 0);   
EmbAJAXSlider ch3B1("ch3B1", 0,  1000, 0);   
EmbAJAXSlider ch4B1("ch4B1", 0,  1000, 0);   
EmbAJAXSlider ch5B1("ch5B1", 0,  1000, 0);   
EmbAJAXSlider ch6B1("ch6B1", 0,  1000, 0);   
EmbAJAXSlider ch7B1("ch7B1", 0,  1000, 0);   
EmbAJAXSlider ch8B1("ch8B1", 0,  1000, 0); 

//pg2
EmbAJAXSlider ch1B2("2chB1", 0,  1000, 0);   
EmbAJAXSlider ch2B2("2chB2", 0,  1000, 0);   
EmbAJAXSlider ch3B2("2chB3", 0,  1000, 0);   
EmbAJAXSlider ch4B2("2chB4", 0,  1000, 0);   
EmbAJAXSlider ch5B2("2chB5", 0,  1000, 0);   
EmbAJAXSlider ch6B2("2chB6", 0,  1000, 0);   
EmbAJAXSlider ch7B2("2chB7", 0,  1000, 0);   
EmbAJAXSlider ch8B2("2chB8", 0,  1000, 0);   

//pg3
EmbAJAXSlider ch1B3("3chB1", 0,  1000, 0);   
EmbAJAXSlider ch2B3("3chB2", 0,  1000, 0);   
EmbAJAXSlider ch3B3("3chB3", 0,  1000, 0);   
EmbAJAXSlider ch4B3("3chB4", 0,  1000, 0);   
EmbAJAXSlider ch5B3("3chB5", 0,  1000, 0);   
EmbAJAXSlider ch6B3("3chB6", 0,  1000, 0);   
EmbAJAXSlider ch7B3("3chB7", 0,  1000, 0);   
EmbAJAXSlider ch8B3("3chB8", 0,  1000, 0);   

//pg4
EmbAJAXSlider ch1B4("4chB1", 0,  1000, 0);   
EmbAJAXSlider ch2B4("4chB2", 0,  1000, 0);   
EmbAJAXSlider ch3B4("4chB3", 0,  1000, 0);   
EmbAJAXSlider ch4B4("4chB4", 0,  1000, 0);   
EmbAJAXSlider ch5B4("4chB5", 0,  1000, 0);   
EmbAJAXSlider ch6B4("4chB6", 0,  1000, 0);   
EmbAJAXSlider ch7B4("4chB7", 0,  1000, 0);   
EmbAJAXSlider ch8B4("4chB8", 0,  1000, 0);   

//pg5
EmbAJAXSlider ch1B5("5chB1", 0,  1000, 0);   
EmbAJAXSlider ch2B5("5chB2", 0,  1000, 0);   
EmbAJAXSlider ch3B5("5chB3", 0,  1000, 0);   
EmbAJAXSlider ch4B5("5chB4", 0,  1000, 0);   
EmbAJAXSlider ch5B5("5chB5", 0,  1000, 0);   
EmbAJAXSlider ch6B5("5chB6", 0,  1000, 0);   
EmbAJAXSlider ch7B5("5chB7", 0,  1000, 0);   
EmbAJAXSlider ch8B5("5chB8", 0,  1000, 0);   


EmbAJAXSlider* sliders[8][5] = {
  {&ch1B1, &ch1B2, &ch1B3, &ch1B4, &ch1B5},
  {&ch2B1, &ch2B2, &ch2B3, &ch2B4, &ch2B5},
  {&ch3B1, &ch3B2, &ch3B3, &ch3B4, &ch3B5},
  {&ch4B1, &ch4B2, &ch4B3, &ch4B4, &ch4B5},
  {&ch5B1, &ch5B2, &ch5B3, &ch5B4, &ch5B5},
  {&ch6B1, &ch6B2, &ch6B3, &ch6B4, &ch6B5},
  {&ch7B1, &ch7B2, &ch7B3, &ch7B4, &ch7B5},
  {&ch8B1, &ch8B2, &ch8B3, &ch8B4, &ch8B5},
};

#define BUFLEN 10

char buttonText[] = "Mute off";
bool button_MuteOn = false;
void buttonPressed(EmbAJAXPushButton*) {
     if (!button_MuteOn){
   strcpy(buttonText, "MUTE  ON");
    button_MuteOn = true;
   }
   else{
  button_MuteOn = false;
  strcpy(buttonText, "Mute Off");
   }
   sendMainMute();
}

EmbAJAXPushButton button("button", " Main Mute ", buttonPressed);
EmbAJAXMutableSpan button_d("button_d");
char button_d_buf[BUFLEN];

EmbAJAXStatic nextCell("</td><td>&nbsp;</td><td><b>");
EmbAJAXStatic nextCell3("</td><td>&nbsp;&nbsp;&nbsp;</td><td><b>");
EmbAJAXStatic nextCell6("</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td><b>");
EmbAJAXStatic nextCell18("</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td><b>");
EmbAJAXStatic nextRow("</td></tr><tr><td><b>");
EmbAJAXStatic s1("</td></tr><a href=\"/page5\">Drums</a><tr><td>");
EmbAJAXStatic s2("</td></tr><a href=\"/page4\">Bass</a><tr><td>");
EmbAJAXStatic s3("</td></tr>Ld&nbspGtr<tr><td>");
EmbAJAXStatic s4("</td></tr>Rm&nbspGtr<tr><td>");
EmbAJAXStatic s5("</td></tr>Keys<tr><td>");
EmbAJAXStatic s6("</td></tr><a href=\"/page1\">Voc1</a><tr><td>");
EmbAJAXStatic s7("</td></tr><a href=\"/page2\">Voc2</a><tr><td>");
EmbAJAXStatic s8b("</td></tr><a href=\"/page3\">Voc3</a><tr><td>"); // for some reasng s8 doesn't work.  says redeclared.  ??  aadding the b makes it ok 
//EmbAJAXStatic s8("</td></tr><a href=\"/page3\">Voc3</a><tr><td>");


MAKE_EmbAJAXPage(page1, "XR18 Control",// "",

                 "<style>\n"
                 "input[type=range]{\n"
                 "writing-mode: bt-lr;\n"
                 "-webkit-appearance: slider-vertical;\n"
                 "width: 20px;\n"
                 "height: 200px;\n"
                 "padding: 0 24px;\n"
                 "outline: none;\n"
                 "background:transparent;\n"
                 "}\n"
                 "</style>\n",
                 // END:   CSS styling

// *?*//
                 new EmbAJAXStatic("<h1>Aux 1</h1>"),

                &ch1B1,
                &ch2B1,
                &ch3B1,
                &ch4B1,
                &ch5B1,
                &ch6B1,
                &ch7B1,
                &ch8B1,
                new EmbAJAXStatic("<i>  </i></p>"),
                &nextRow,
                &nextCell3,
                &s1,
                &nextCell6,
                &nextCell,
                &s2,
                &nextCell6,
                &nextCell,
                &s3,
                &nextCell6,
                &s4,
                &nextCell6,
                &nextCell,
                &nextCell,
                &s5,
                &nextCell6,
                &nextCell3,
                &s6,
                &nextCell3,
                &nextCell6,
                &s7,
                &nextCell6,
                &nextCell3,
                &s8b,
                &nextCell6,
                new EmbAJAXStatic("<i>  </i></p>"),
                new EmbAJAXStatic("<i>  </i></p>"),
                &nextRow,
                &nextRow,
                &nextCell6,
                &nextCell18,
                &nextCell18,
                &button,
                &nextCell,
                &button_d
                )


// //pg2  *?*
                  MAKE_EmbAJAXPage(page2, "XR18 Control",// "",
                 "<style>\n"
                 "input[type=range]{\n"
                 "writing-mode: bt-lr;\n"
                 "-webkit-appearance: slider-vertical;\n"
                 "width: 20px;\n"
                 "height: 200px;\n"
                 "padding: 0 24px;\n"
                 "outline: none;\n"
                 "background:transparent;\n"
                 "}\n"
                 "</style>\n",
                 // END:   CSS styling

                 new EmbAJAXStatic("<h1>Aux 2</h1>"),

                 &ch1B2,
                 &ch2B2,
                 &ch3B2,
                 &ch4B2,
                 &ch5B2,
                 &ch6B2,
                 &ch7B2,
                 &ch8B2,
                new EmbAJAXStatic("<i>  </i></p>"),
                &nextRow,
                &nextCell3,
                &s1,
                &nextCell6,
                &nextCell,
                &s2,
                &nextCell6,
                &nextCell,
                &s3,
                &nextCell6,
                &s4,
                &nextCell6,
                &nextCell,
                &nextCell,
                &s5,
                &nextCell6,
                &nextCell3,
                &s6,
                &nextCell3,
                &nextCell6,
                &s7,
                &nextCell6,
                &nextCell3,
                &s8b,
                &nextCell6,
                new EmbAJAXStatic("<i>  </i></p>"),
                new EmbAJAXStatic("<i>  </i></p>"),
                &nextRow,
                &nextRow,
                &nextCell6,
                &nextCell18,
                &nextCell18,
                &button,
                &nextCell,
                &button_d
                )
              //     //*?*
                  MAKE_EmbAJAXPage(page3, "XR18 Control",// "",
                 "<style>\n"
                 "input[type=range]{\n"
                 "writing-mode: bt-lr;\n"
                 "-webkit-appearance: slider-vertical;\n"
                 "width: 20px;\n"
                 "height: 200px;\n"
                 "padding: 0 24px;\n"
                 "outline: none;\n"
                 "background:transparent;\n"
                 "}\n"
                 "</style>\n",
                 // END:   CSS styling


                 new EmbAJAXStatic("<h1>Aux 3</h1>"),
                 &ch1B3,
                 &ch2B3,
                 &ch3B3,
                 &ch4B3,
                 &ch5B3,
                 &ch6B3,
                 &ch7B3,
                 &ch8B3,
                new EmbAJAXStatic("<i>  </i></p>"),
                &nextRow,
                &nextCell3,
                &s1,
                &nextCell6,
                &nextCell,
                &s2,
                &nextCell6,
                &nextCell,
                &s3,
                &nextCell6,
                &s4,
                &nextCell6,
                &nextCell,
                &nextCell,
                &s5,
                &nextCell6,
                &nextCell3,
                &s6,
                &nextCell3,
                &nextCell6,
                &s7,
                &nextCell6,
                &nextCell3,
                &s8b,
                &nextCell6,
                new EmbAJAXStatic("<i>  </i></p>"),
                new EmbAJAXStatic("<i>  </i></p>"),
                &nextRow,
                &nextRow,
                &nextCell6,
                &nextCell18,
                &nextCell18,
                &button,
                &nextCell,
                &button_d
                )
// // //*?*
                 MAKE_EmbAJAXPage(page4, "XR18 Control",// "",
                 "<style>\n"
                 "input[type=range]{\n"
                 "writing-mode: bt-lr;\n"
                 "-webkit-appearance: slider-vertical;\n"
                 "width: 20px;\n"
                 "height: 200px;\n"
                 "padding: 0 24px;\n"
                 "outline: none;\n"
                 "background:transparent;\n"
                 "}\n"
                 "</style>\n",
                // END:   CSS styling

                 new EmbAJAXStatic("<h1>Aux 4</h1>"),
                 &ch1B4,
                 &ch2B4,
                 &ch3B4,
                 &ch4B4,
                 &ch5B4,
                 &ch6B4,
                 &ch7B4,
                 &ch8B4,
                new EmbAJAXStatic("<i>  </i></p>"),
                &nextRow,
                &nextCell3,
                &s1,
                &nextCell6,
                &nextCell,
                &s2,
                &nextCell6,
                &nextCell,
                &s3,
                &nextCell6,
                &s4,
                &nextCell6,
                &nextCell,
                &nextCell,
                &s5,
                &nextCell6,
                &nextCell3,
                &s6,
                &nextCell3,
                &nextCell6,
                &s7,
                &nextCell6,
                &nextCell3,
                &s8b,
                &nextCell6,
                new EmbAJAXStatic("<i>  </i></p>"),
                new EmbAJAXStatic("<i>  </i></p>"),
                &nextRow,
                &nextRow,
                &nextCell6,
                &nextCell6,
                &nextCell18,
                &nextCell18,
                &button,
                &nextCell,
                &button_d
                )
// //                 //*?*
                MAKE_EmbAJAXPage(page5, "XR18 Control",// "",
                 "<style>\n"               
                 "input[type=range]{\n"
                 "writing-mode: bt-lr;\n"
                 "-webkit-appearance: slider-vertical;\n"
                 "width: 20px;\n"
                 "height: 200px;\n"
                 "padding: 0 24px;\n"
                 "outline: none;\n"
                 "background:transparent;\n"
                 "}\n"
                 "</style>\n",
                 // END:   CSS styling
                 
                 new EmbAJAXStatic("<h1>Aux 5</h1>"),
                 &ch1B5,
                 &ch2B5,
                 &ch3B5,
                 &ch4B5,
                 &ch5B5,
                 &ch6B5,
                 &ch7B5,
                 &ch8B5,
                new EmbAJAXStatic("<i>  </i></p>"),
                &nextRow,
                &nextCell3,
                &s1,
                &nextCell6,
                &nextCell,
                &s2,
                &nextCell6,
                &nextCell,
                &s3,
                &nextCell6,
                &s4,
                &nextCell6,
                &nextCell,
                &nextCell,
                &s5,
                &nextCell6,
                &nextCell3,
                &s6,
                &nextCell3,
                &nextCell6,
                &s7,
                &nextCell6,
                &nextCell3,
                &s8b,
                &nextCell6,
                new EmbAJAXStatic("<i>  </i></p>"),
                new EmbAJAXStatic("<i>  </i></p>"),
                &nextRow,
                &nextRow,
                &nextCell6,
                &nextCell18,
                &nextCell18,
                &button,
                &nextCell,
                &button_d
                )


MAKE_EmbAJAXPage(page, "Home Page",
                 "<!-- For simplicitly reasons, we're inlining the CSS into the main page, here.\n"
                 "Note that you could also link to an external stylesheet (served from SD card, or from the internet) like this:\n"
                 "<link rel=\"stylesheet\" href=\"styles.css\">  -->\n"
                 "<style>\n"
                 "input[type=range]{\n"
                 "writing-mode: bt-lr;\n"
                 "-webkit-appearance: slider-vertical;\n"
                 "width: 20px;\n"
                 "height: 200px;\n"
                 "padding: 0 24px;\n"
                 "outline: none;\n"
                 "background:transparent;\n"
                 "}\n"
                 "</style>\n",
                 
                 //&textInput,
                 new EmbAJAXStatic("</p><p>Link to <a href=\"/page1\">Aux 1</a> </p>"),
                 new EmbAJAXStatic("</p><p>Link to <a href=\"/page2\">Aux 2</a> </p>"),
                 new EmbAJAXStatic("</p><p>Link to <a href=\"/page3\">Aux 3</a> </p>"),
                 new EmbAJAXStatic("</p><p>Link to <a href=\"/page4\">Aux 4</a> </p>"),
                 new EmbAJAXStatic("</p><p>Link to <a href=\"/page5\">Aux 5</a> </p>"),
                 // new EmbAJAXStatic("</p><p>Link to <a href=\"/page6\">Page 6</a> </p>")

                )



unsigned long XremoteMillis;
bool mixerChange = false;


void updateUI() {

findURL();  

if (!mixer.sliderChange){
  browser.sliderChange = true;
//  Serial.println("broswer change");
sendOscMessageAndWaitForResponse();
}


if (mixer.sliderChange){
  int convCh = find(sliderChan, mixer.mch);    
//   Serial.print("Mixer moved slider: ");
//   Serial.print(mixer.mch);
//   Serial.print("   Bus:  ");
//   Serial.print(mixer.mbus);
//   Serial.print("    Leve: ");
// Serial.println(mixer.mixerLevel);  

   // sliders [convCh] -> setValue(mixer.mixerLevel); // change for single page
     sliders [convCh][mixer.mbus-1] -> setValue(mixer.mixerLevel); // change for 5 page
    mixer.sliderChange = false;
    delay(50); // was 50
}

//Serial.println("update UI");
     button_d.setValue(strncpy(button_d_buf, buttonText, BUFLEN));
}

int find(int arr[], int seek)
{
    for (int i = 0; i < 8; ++i)
    {
        if (arr[i] == seek) return i;
    }
    return -1;
}

int levelArray [8][5];
int ch;
int bus;
int oldChLvl[8][5];

void setupSync(){
// send blank message for ch 1-8 
for (int c = 0; c < 8; ++c){
  for (int b = 0; b < 5; ++b){
  OSCMessage msg(MessageBuilder(sliderConvert(c), b+1));  //browser.mbus was option
 // Serial.println("msg:  " + String(MessageBuilder(sliderConvert(c),b+1)));  
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
delay(50); 
readLevel();
 sliders [c][b] -> setValue(mixer.mixerLevel); // change for single page 
//Serial.print("setup sync:  ");
//Serial.println(mixer.mixerLevel);
delay(50);
if (c==7 && b==4){
synced = true;
//Udp.endPacket();
}
}  
}
}

char * MessageBuilder(int ch, int bus) {  // bus comes from array,  channel needs conversion 
  static char str[20];
  sprintf(str, "/ch/%02d/mix/%02d/level", ch, bus);   // was ch+1 prior to addling sliderConvert// Format the string using the channel and button variables, and copy it into the str array  (X32 and XR 18 same)
  return str;
}

void readLevel(){
int readLvl;
//Serial.println("ReadLevel()");
  int packetSize = Udp.parsePacket();
  if (packetSize) {   
  OSCMessage response; 
  while(packetSize--){
      response.fill(Udp.read());
  }      
      delay(50);   
   response.getAddress(packet, 0);
String address = String(packet);
mixer.mch = address.substring(4,6).toInt();
 //Serial.print("Chan: ");
 //Serial.print(mixer.mch);
mixer.mbus = address.substring(11,13).toInt();
 //Serial.print("    Bus: ");
// Serial.print(mixer.mbus);
float level = response.getFloat(0);

readLvl = level*1000;
//Serial.print("     Read level: ");
//Serial.println(readLvl);
if (readLvl > 1000)
return;
else
mixer.mixerLevel = readLvl;
}
// if (synced)
//  Udp.endPacket();
}
int sliderConvert (int ch){
//  Serial.print("Convert:  ");
//  Serial.print(ch);
//  Serial.print("     to:  ");
//  Serial.println(sliderChan[ch]);
  return sliderChan[ch];
}

////  main mute command  for XR18 is  /lr/mix/on  add 0 for off 1 for on
// for X32 is /main/st/mix/on
void sendMainMute(){
//OSCMessage msg("/lr/mix/on"); //XR18
OSCMessage msg("/main/st/mix/on");//X32  //works

if (button_MuteOn){
msg.add(0);  //mute btn on means mix off so 0
//Serial.println(F("MUTE ON"));
}
else {
  msg.add(1);
 /// Serial.println(F("Mute off"));
}
Udp.beginPacket(outIp, outPort);
msg.send(Udp);
Udp.endPacket();  
}

void sendOscMessageAndWaitForResponse() {
int b = browser.mbus-1;
for (int c = 0; c<8; c++){
 int level = sliders[c][b]->intValue();  
   if (level != oldChLvl[c][b]){
      OSCMessage msg(MessageBuilder(sliderConvert(c), browser.mbus));  //was option 
     //Serial.println("msg:  " + String(MessageBuilder(sliderConvert(c), browser.mbus)));  
    float levelFloat = ((float)level) / 1000;
    msg.add(levelFloat);
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
   oldChLvl[c][b] = level;
   levelArray[c][b] = level;
  
  //Serial.print("OSC msg Level: ");
 // Serial.println(level);
    }
  }
  browser.sliderChange = false;
}
void setup() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(local_IP, gateway, subnet);

  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
delay(50);

driver.installPage(&page, "/", updateUI);
delay(20);
driver.installPage(&page1, "/page1", updateUI);
delay(20);
driver.installPage(&page2, "/page2", updateUI);
delay(20);
driver.installPage(&page3, "/page3", updateUI);
delay(20);
driver.installPage(&page4, "/page4", updateUI);
delay(20);
driver.installPage(&page5, "/page5", updateUI);
delay(20);

server.begin();

Udp.begin(10023);

sendMainMute();
//Serial.println("setup done");
}

void findURL() {
  String path = server.uri(); // get the URL path
if (path.length() > 2){
  browser.mbus = path.substring(5,6).toInt();
 
}  
}

void loop() {
  // Serial.println("loop begin");

findURL();


 if (!synced){
 setupSync();
 }
  unsigned long currentMillis;
  // Check is a /xremote OSC command needs to be sent to X32
 if (synced){ 
  currentMillis = millis();
  if ((currentMillis - XremoteMillis) > 9000) { // wsa 9000
  //  Serial.printf("Sending Remote\n");
    OSCMessage msg("/xremote");
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
      XremoteMillis = currentMillis;
    delay(50); // was 100
   // Serial.println(F("****xremote sent****")); 
   
  }

  int packetSize = Udp.parsePacket();
if(packetSize && !browser.sliderChange){
//mixer.mixerLevel = readLevel();
readLevel();
mixer.sliderChange = true;  
}
if (!packetSize && mixer.sliderChange)
updateUI();
 
driver.loopHook();
 }
 //Serial.println("loop end");
}
