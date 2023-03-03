#include <Arduino.h>
//#include <ESP_EEPROM.h>
#include<EEPROM.h>
#define EEPROM_SIZE 164
#include <EmbAJAX.h>
#include <EmbAJAXValidatingTextInput.h>
#include <EmbAJAXScriptedSpan.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

// added to move browser slider if mixer changed
bool sliderChange = false;
int mch;
int mbus;
int mixerLevel;

// *** Setup values***//
//String MixerIP = "192.168.3.2";
const IPAddress outIp(192, 168, 1, 212);     // IP of Mixer
const unsigned int outPort = 10023;       //10024 for XR18   // remote port to receive OSC
const unsigned int localPort = 10023;    //10024 or XR18    // local port to listen for OSC packets 

#define STASSID "WifiNode"
#define STAPSK "lola1124"

// Set Static IP address
IPAddress local_IP(192, 168, 1, 222);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
//add foe UDP rec

//enter Fader Channel for easch control slider AND label
char s1Chan[] = "1"; 
char s1Label[] = "Drums";
char s2Chan[] = "7"; 
char s2Label[] = "Bass";
char s3Chan[] = "8"; 
char s3Label[] = "Ld Gtr";
char s4Chan[] = "9"; 
char s4Label[] = "Rm Gtr";
char s5Chan[] = "11"; 
char s5Label[] = "Keys";
char s6Chan[] = "13"; 
char s6Label[] = " Kevin";
char s7Chan[] = "14"; 
char s7Label[] = " Chris";
char s8Chan[] = "15"; 
char s8Label[] = " Leah";

// if you want to change the Bus 1-5 page header "find" the line commnet *?* and change "Bus 1"(to 5)  to whatever.....

char packet[255];
char reply[] = "Packet received!";

EmbAJAXOutputDriverWebServerClass server(80); 
EmbAJAXOutputDriverESP8266 driver(&server);

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
     // sendMainMute();
   strcpy(buttonText, "MUTE  ON");
    button_MuteOn = true;
    //return;
   }
   else{
  //  sendMainMute();
  button_MuteOn = false;
  strcpy(buttonText, "Mute Off");
 // return;
   }
   sendMainMute();
}
EmbAJAXPushButton button("button", " Main Mute ", buttonPressed);
EmbAJAXMutableSpan button_d("button_d");
char button_d_buf[BUFLEN];


const char* radio_opts[] = {"XR18", "X32"};
EmbAJAXRadioGroup<2> radio("radio", radio_opts);
EmbAJAXMutableSpan radio_d("radio_d");


EmbAJAXTextInput<BUFLEN> slider1ch("ch1");  // Text input, width BUFLEN
EmbAJAXMutableSpan slider1ch_d("ch1_d");
char slider1ch_d_buf[BUFLEN];
EmbAJAXTextInput<BUFLEN> slider1name("name1");  // Text input, width BUFLEN
EmbAJAXMutableSpan slider1name_d("name1_d");
char slider1name_d_buf[BUFLEN];

EmbAJAXTextInput<BUFLEN> slider2ch("ch2");  // Text input, width BUFLEN
EmbAJAXMutableSpan slider2ch_d("ch2_d");
char slider2ch_d_buf[BUFLEN];
EmbAJAXTextInput<BUFLEN> slider2name("2name");  // Text input, width BUFLEN
EmbAJAXMutableSpan slider2name_d("2name_d");
char slider2name_d_buf[BUFLEN];

EmbAJAXTextInput<BUFLEN> slider3ch("ch3");  // Text input, width BUFLEN
EmbAJAXMutableSpan slider3ch_d("ch3_d");
char slider3ch_d_buf[BUFLEN];
EmbAJAXTextInput<BUFLEN> slider3name("3name");  // Text input, width BUFLEN
EmbAJAXMutableSpan slider3name_d("3name_d");
char slider3name_d_buf[BUFLEN];

EmbAJAXTextInput<BUFLEN> slider4ch("ch4");  // Text input, width BUFLEN
EmbAJAXMutableSpan slider4ch_d("ch4_d");
char slider4ch_d_buf[BUFLEN];
EmbAJAXTextInput<BUFLEN> slider4name("4name");  // Text input, width BUFLEN
EmbAJAXMutableSpan slider4name_d("4name_d");
char slider4name_d_buf[BUFLEN];

EmbAJAXTextInput<BUFLEN> slider5ch("ch5");  // Text input, width BUFLEN
EmbAJAXMutableSpan slider5ch_d("ch5_d");
char slider5ch_d_buf[BUFLEN];
EmbAJAXTextInput<BUFLEN> slider5name("5name");  // Text input, width BUFLEN
EmbAJAXMutableSpan slider5name_d("5name_d");
char slider5name_d_buf[BUFLEN];

EmbAJAXTextInput<BUFLEN> slider6ch("ch6");  // Text input, width BUFLEN
EmbAJAXMutableSpan slider6ch_d("ch6_d");
char slider6ch_d_buf[BUFLEN];
EmbAJAXTextInput<BUFLEN> slider6name("6name");  // Text input, width BUFLEN
EmbAJAXMutableSpan slider6name_d("6name_d");
char slider6name_d_buf[BUFLEN];

EmbAJAXTextInput<BUFLEN> slider7ch("ch7");  // Text input, width BUFLEN
EmbAJAXMutableSpan slider7ch_d("ch7_d");
char slider7ch_d_buf[BUFLEN];
EmbAJAXTextInput<BUFLEN> slider7name("7name");  // Text input, width BUFLEN
EmbAJAXMutableSpan slider7name_d("7name_d");
char slider7name_d_buf[BUFLEN];

EmbAJAXTextInput<BUFLEN> slider8ch("ch8");  // Text input, width BUFLEN
EmbAJAXMutableSpan slider8ch_d("ch8_d");
char slider8ch_d_buf[BUFLEN];
EmbAJAXTextInput<BUFLEN> slider8name("8name");  // Text input, width BUFLEN
EmbAJAXMutableSpan slider8name_d("8name_d");
char slider8name_d_buf[BUFLEN];

EmbAJAXValidatingTextInput<16> valtext("valtext");
EmbAJAXMutableSpan valtext_d("valtext_d");
char valtext_d_buf[BUFLEN];

EmbAJAXStatic nextCell("</td><td>&nbsp;</td><td><b>");
EmbAJAXStatic nextCell3("</td><td>&nbsp;&nbsp;&nbsp;</td><td><b>");
EmbAJAXStatic nextCell6("</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td><b>");
EmbAJAXStatic nextCell18("</td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td><td><b>");
EmbAJAXStatic nextRow("</b></td></tr><tr><td>");

MAKE_EmbAJAXPage(page6, "EmbAJAX example - Inputs", "",
    new EmbAJAXStatic("<table cellpadding=\"10\"><tr><td>"),
    
new EmbAJAXStatic("Select Model"),
 &nextCell,
    &radio,
    &nextCell,
    &radio_d,
    &nextRow,
    
new EmbAJAXStatic("Enter Mixer IP Address:"),
     &nextCell,
    &valtext,
    &nextCell,
    &valtext_d,
    &nextRow,
    
    new EmbAJAXStatic("Server status:"),
    &nextCell,
    new EmbAJAXConnectionIndicator(),

    &nextRow,
    &nextRow,
    &nextCell,
    new EmbAJAXStatic("Channel"),
    &nextCell,
    &nextCell,
    new EmbAJAXStatic("Label"),
    &nextRow,
    
    new EmbAJAXStatic("Slider 1"),
     &nextCell,
    &slider1ch,
    &nextCell,
    &slider1ch_d,
    &nextCell,
    &slider1name,  
    &nextCell,
    &slider1name_d,
    &nextRow,

    new EmbAJAXStatic("Slider 2"),
     &nextCell,
    &slider2ch,
    &nextCell,
    &slider2ch_d,
    &nextCell,
    &slider2name,  
    &nextCell,
    &slider2name_d,
    &nextRow,

    new EmbAJAXStatic("Slider 3"),
     &nextCell,
    &slider3ch,
    &nextCell,
    &slider3ch_d,
    &nextCell,
    &slider3name,  
    &nextCell,
    &slider3name_d,
    &nextRow,

        new EmbAJAXStatic("Slider 4"),
     &nextCell,
    &slider4ch,
    &nextCell,
    &slider4ch_d,
    &nextCell,
    &slider4name,  
    &nextCell,
    &slider4name_d,
    &nextRow,

        new EmbAJAXStatic("Slider 5"),
     &nextCell,
    &slider5ch,
    &nextCell,
    &slider5ch_d,
    &nextCell,
    &slider5name,  
    &nextCell,
    &slider5name_d,
    &nextRow,

        new EmbAJAXStatic("Slider 6"),
     &nextCell,
    &slider6ch,
    &nextCell,
    &slider6ch_d,
    &nextCell,
    &slider6name,  
    &nextCell,
    &slider6name_d,
    &nextRow,
    

        new EmbAJAXStatic("Slider 7"),
     &nextCell,
    &slider7ch,
    &nextCell,
    &slider7ch_d,
    &nextCell,
    &slider7name,  
    &nextCell,
    &slider7name_d,
    &nextRow,

        new EmbAJAXStatic("Slider 8"),
     &nextCell,
    &slider8ch,
    &nextCell,
    &slider8ch_d,
    &nextCell,
    &slider8name,  
    &nextCell,
    &slider8name_d,
    &nextRow,

    new EmbAJAXStatic("</b></td></tr></table>"),

       new EmbAJAXStatic("</p><p>Link to <a href=\"/page1\">page 1</a> </p>"),
   new EmbAJAXStatic("</p><p>Link to <a href=\"/page2\">page 2</a> </p>"),
   new EmbAJAXStatic("</p><p>Link to <a href=\"/page3\">page 3</a> </p>"),
   new EmbAJAXStatic("</p><p>Link to <a href=\"/page4\">page 4</a> </p>"),
   new EmbAJAXStatic("</p><p>Link to <a href=\"/page5\">page 5</a> </p>"),
   new EmbAJAXStatic("</b></td></tr></table>")
                
)

// try new page1
MAKE_EmbAJAXPage(page1, "XR18 Control",// "",
                 // "<!-- For simplicitly reasons, we're inlining the CSS into the main page, here.\n"
                 //  "Note that you could also link to an external stylesheet (served from SD card, or from the internet) like this:\n"
                 //  "<link rel=\"stylesheet\" href=\"styles.css\">  -->\n"
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
                 new EmbAJAXStatic("<h1>Bus 1</h1>"),
                 // &mode,
                 //  new EmbAJAXStatic("</p><p><i>&nbsp+10</i></p>"),
                 // new EmbAJAXStatic("<i>&nbsp-50</i>"),
                 // &blinkfreq,   //orig
                 &ch1B1,
                 &ch2B1,
                 &ch3B1,
                 &ch4B1,
                 &ch5B1,
                 &ch6B1,
                 &ch7B1,
                 &ch8B1,
                 new EmbAJAXStatic("<i>  </i></p>"),
                 &nextCell3,
                 &slider1name_d,
                 &nextCell6,
                 &slider2name_d,
                 &nextCell6,
                 &slider3name_d,
                 &nextCell6,
                 &slider4name_d,
                 &nextCell6,
                 &slider5name_d,
                 &nextCell6,
                 &slider6name_d,
                 &nextCell6,
                 &slider7name_d,
                 &nextCell6,
                 &slider8name_d,
                // &nextCell6,
                 new EmbAJAXStatic("<i>  </i></p>"),
                 new EmbAJAXStatic("<i>  </i></p>"),
                 &nextRow,
                 &nextRow,
                 &nextCell18,
                 &nextCell18,
                 &nextCell18,
                 &button,
                 &nextCell,
                 &button_d,
                 &nextRow,
                //end
                 new EmbAJAXStatic("<i>  </i></p>"),
                 new EmbAJAXStatic("</p><p>Link to <a href=\"/\">Index</a> </p>")

                                 

                )
//pg2  *?*
                  MAKE_EmbAJAXPage(page2, "XR18 Control",// "",
                 // "<!-- For simplicitly reasons, we're inlining the CSS into the main page, here.\n"
                 //  "Note that you could also link to an external stylesheet (served from SD card, or from the internet) like this:\n"
                 //  "<link rel=\"stylesheet\" href=\"styles.css\">  -->\n"
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

                 new EmbAJAXStatic("<h1>Bus 2</h1>"),
                 // &mode,
                 //  new EmbAJAXStatic("</p><p><i>&nbsp+10</i></p>"),
                 // new EmbAJAXStatic("<i>&nbsp-50</i>"),
                 // &blinkfreq,   //orig
                 &ch1B2,
                 &ch2B2,
                 &ch3B2,
                 &ch4B2,
                 &ch5B2,
                 &ch6B2,
                 &ch7B2,
                 &ch8B2,
                 new EmbAJAXStatic("<i>  </i></p>"),
                  &nextCell3,
                 &slider1name_d,
                 &nextCell6,
                 &slider2name_d,
                 &nextCell6,
                 &slider3name_d,
                 &nextCell6,
                 &slider4name_d,
                 &nextCell6,
                 &slider5name_d,
                 &nextCell6,
                 &slider6name_d,
                 &nextCell6,
                 &slider7name_d,
                 &nextCell6,
                 &slider8name_d,
                // &nextCell6,
                 new EmbAJAXStatic("<i>  </i></p>"),
                 new EmbAJAXStatic("<i>  </i></p>"),
                 &nextRow,
                 &nextRow,
                 &nextCell18,
                 &nextCell18,
                 &nextCell18,
                 &button,
                 &nextCell,
                 &button_d,
                 &nextRow,
                //end
                 new EmbAJAXStatic("<i>  </i></p>"),
                 new EmbAJAXStatic("</p><p>Link to <a href=\"/\">Index</a> </p>")
                )
                  //*?*
                  MAKE_EmbAJAXPage(page3, "XR18 Control",// "",
                 // "<!-- For simplicitly reasons, we're inlining the CSS into the main page, here.\n"
                 //  "Note that you could also link to an external stylesheet (served from SD card, or from the internet) like this:\n"
                 //  "<link rel=\"stylesheet\" href=\"styles.css\">  -->\n"
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


                 new EmbAJAXStatic("<h1>Bus 3</h1>"),
                 // &mode,
                 //  new EmbAJAXStatic("</p><p><i>&nbsp+10</i></p>"),
                 // new EmbAJAXStatic("<i>&nbsp-50</i>"),
                 // &blinkfreq,   //orig
                 &ch1B3,
                 &ch2B3,
                 &ch3B3,
                 &ch4B3,
                 &ch5B3,
                 &ch6B3,
                 &ch7B3,
                 &ch8B3,
                 new EmbAJAXStatic("<i>  </i></p>"),
                  &nextCell3,
                 &slider1name_d,
                 &nextCell6,
                 &slider2name_d,
                 &nextCell6,
                 &slider3name_d,
                 &nextCell6,
                 &slider4name_d,
                 &nextCell6,
                 &slider5name_d,
                 &nextCell6,
                 &slider6name_d,
                 &nextCell6,
                 &slider7name_d,
                 &nextCell6,
                 &slider8name_d,
                // &nextCell6,
                 new EmbAJAXStatic("<i>  </i></p>"),
                 new EmbAJAXStatic("<i>  </i></p>"),
                 &nextRow,
                 &nextRow,
                 &nextCell18,
                 &nextCell18,
                 &nextCell18,
                 &button,
                 &nextCell,
                 &button_d,
                 &nextRow,
                //end
                 new EmbAJAXStatic("<i>  </i></p>"),
                 new EmbAJAXStatic("</p><p>Link to <a href=\"/\">Index</a> </p>")
                )
//*?*
                 MAKE_EmbAJAXPage(page4, "XR18 Control",// "",
                 // "<!-- For simplicitly reasons, we're inlining the CSS into the main page, here.\n"
                 //  "Note that you could also link to an external stylesheet (served from SD card, or from the internet) like this:\n"
                 //  "<link rel=\"stylesheet\" href=\"styles.css\">  -->\n"
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

                 new EmbAJAXStatic("<h1>Bus 4</h1>"),
                 // &mode,
                 //  new EmbAJAXStatic("</p><p><i>&nbsp+10</i></p>"),
                 // new EmbAJAXStatic("<i>&nbsp-50</i>"),
                 // &blinkfreq,   //orig
                 &ch1B4,
                 &ch2B4,
                 &ch3B4,
                 &ch4B4,
                 &ch5B4,
                 &ch6B4,
                 &ch7B4,
                 &ch8B4,
                 new EmbAJAXStatic("<i>  </i></p>"),
                  &nextCell3,
                 &slider1name_d,
                 &nextCell6,
                 &slider2name_d,
                 &nextCell6,
                 &slider3name_d,
                 &nextCell6,
                 &slider4name_d,
                 &nextCell6,
                 &slider5name_d,
                 &nextCell6,
                 &slider6name_d,
                 &nextCell6,
                 &slider7name_d,
                 &nextCell6,
                 &slider8name_d,
                // &nextCell6,
                 new EmbAJAXStatic("<i>  </i></p>"),
                 new EmbAJAXStatic("<i>  </i></p>"),
                 &nextRow,
                 &nextRow,
                 &nextCell18,
                 &nextCell18,
                 &nextCell18,
                 &button,
                 &nextCell,
                 &button_d,
                 &nextRow,
                //end
                 new EmbAJAXStatic("<i>  </i></p>"),
                 new EmbAJAXStatic("</p><p>Link to <a href=\"/\">Index</a> </p>")
                )
                //*?*
                MAKE_EmbAJAXPage(page5, "XR18 Control",// "",
                 // "<!-- For simplicitly reasons, we're inlining the CSS into the main page, here.\n"
                 //  "Note that you could also link to an external stylesheet (served from SD card, or from the internet) like this:\n"
                 //  "<link rel=\"stylesheet\" href=\"styles.css\">  -->\n"
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
                 
                 new EmbAJAXStatic("<h1>Bus 5</h1>"),
                 // &mode,
                 //  new EmbAJAXStatic("</p><p><i>&nbsp+10</i></p>"),
                 // new EmbAJAXStatic("<i>&nbsp-50</i>"),
                 // &blinkfreq,   //orig
                 &ch1B5,
                 &ch2B5,
                 &ch3B5,
                 &ch4B5,
                 &ch5B5,
                 &ch6B5,
                 &ch7B5,
                 &ch8B5,
                 new EmbAJAXStatic("<i>  </i></p>"),
                  &nextCell3,
                 &slider1name_d,
                 &nextCell6,
                 &slider2name_d,
                 &nextCell6,
                 &slider3name_d,
                 &nextCell6,
                 &slider4name_d,
                 &nextCell6,
                 &slider5name_d,
                 &nextCell6,
                 &slider6name_d,
                 &nextCell6,
                 &slider7name_d,
                 &nextCell6,
                 &slider8name_d,
                // &nextCell6,
                 new EmbAJAXStatic("<i>  </i></p>"),
                 new EmbAJAXStatic("<i>  </i></p>"),
                 &nextRow,
                 &nextRow,
                 &nextCell18,
                 &nextCell18,
                 &nextCell18,
                 &button,
                 &nextCell,
                 &button_d,
                 &nextRow,
                //end
                 new EmbAJAXStatic("<i>  </i></p>"),
                 new EmbAJAXStatic("</p><p>Link to <a href=\"/\">Index</a> </p>")
                )
String html;
//html = "";
EmbAJAXSlider shared_slider("shared_slider", 0,  1000, 0);
EmbAJAXSlider other_slider("other_slider", 0,  1000, 0);
EmbAJAXMutableSpan other_slider_display("other_slider_display");
char other_slider_display_buf[8];

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
                 new EmbAJAXStatic("</p><p>Link to <a href=\"/page1\">page 1</a> </p>"),
                 new EmbAJAXStatic("</p><p>Link to <a href=\"/page2\">page 2</a> </p>"),
                 new EmbAJAXStatic("</p><p>Link to <a href=\"/page3\">page 3</a> </p>"),
                 new EmbAJAXStatic("</p><p>Link to <a href=\"/page4\">page 4</a> </p>"),
                 new EmbAJAXStatic("</p><p>Link to <a href=\"/page5\">page 5</a> </p>"),
                  new EmbAJAXStatic("</p><p>Link to <a href=\"/page6\">Page 6</a> </p>")

                )
             
void handlePage() {
  Serial.println("Handle Page");
  if (server.method() == HTTP_POST) { // AJAX request
    page.handleRequest(updateUI);
    page1.handleRequest(updateUI);
    page2.handleRequest(updateUI);
    page3.handleRequest(updateUI);
    page4.handleRequest(updateUI);
    page5.handleRequest(updateUI); 
    page6.handleRequest(updateUI); 

  } else {  // Page load
    page.print();
    page1.print();
    page2.print();
    page3.print();
    page4.print();
    page5.print();
    page6.print();
  }
}

#ifndef STASSID

#endif
const char* ssid = STASSID;
const char* password = STAPSK;


//orig//
WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP


//WiFiUDP Udp;   // A UDP instance to let us send and receive packets over UDP 
//////const IPAddress ip;
//const String ipstring = valtext_d.value();
//
////const IPAddress outIp(ip.fromString(ipstring));    // remote IP of your computer
////const IPAddress outIp(192, 168, 1, 212);  
//const IPAddress outIp(ipstring);  
//const unsigned int outPort = 10023;          // remote port to receive OSC
//const unsigned int localPort = 10023;        // local port to listen for OSC packets (actually not used for sending)

unsigned long XremoteMillis;

void updateUI() {
    radio_d.setValue(radio_opts[radio.selectedOption()]);
  
    slider1ch_d.setValue(strncpy(slider1ch_d_buf, s1Chan, BUFLEN));
    slider2ch_d.setValue(strncpy(slider2ch_d_buf, s2Chan, BUFLEN));
    slider3ch_d.setValue(strncpy(slider3ch_d_buf, s3Chan, BUFLEN));
    slider4ch_d.setValue(strncpy(slider4ch_d_buf, s4Chan, BUFLEN));
    slider5ch_d.setValue(strncpy(slider5ch_d_buf, s5Chan, BUFLEN));
    slider6ch_d.setValue(strncpy(slider6ch_d_buf, s6Chan, BUFLEN));
    slider7ch_d.setValue(strncpy(slider7ch_d_buf, s7Chan, BUFLEN));
    slider8ch_d.setValue(strncpy(slider8ch_d_buf, s8Chan, BUFLEN));

    slider1name_d.setValue(strncpy(slider1name_d_buf, s1Label, BUFLEN));
    slider2name_d.setValue(strncpy(slider2name_d_buf, s2Label, BUFLEN));
    slider3name_d.setValue(strncpy(slider3name_d_buf, s3Label, BUFLEN));
    slider4name_d.setValue(strncpy(slider4name_d_buf, s4Label, BUFLEN));
    slider5name_d.setValue(strncpy(slider5name_d_buf, s5Label, BUFLEN));
    slider6name_d.setValue(strncpy(slider6name_d_buf, s6Label, BUFLEN));
    slider7name_d.setValue(strncpy(slider7name_d_buf, s7Label, BUFLEN));
    slider8name_d.setValue(strncpy(slider8name_d_buf, s8Label, BUFLEN));

    valtext_d.setValue(strncpy(valtext_d_buf, valtext.value(), BUFLEN));
    button_d.setValue(strncpy(button_d_buf, buttonText, BUFLEN));

    if (sliderChange){
       sliders [mch-1][mbus-1] -> setValue(mixerLevel); 
      /// Serial.print("try set fader:  ");
     //  Serial.println(mixerLevel);
       sliderChange = false;
    }
    delay(50);
}
void handleOscResponse(OSCMessage &msg) {
  Serial.println("Received OSC response:");
  printOSCMessage(msg);
}

void printOSCMessage(OSCMessage &msg) {
  Serial.println("OSC Message:");
  Serial.println(msg.size());
  for (int i = 0; i < msg.size(); i++) {
    Serial.print(i);
    Serial.print(": ");
    if (msg.isInt(i)) {
      Serial.println(msg.getInt(i));
    } else if (msg.isFloat(i)) {
      Serial.println(msg.getFloat(i));
    } else if (msg.isString(i)) {
      char buffer[128];
      msg.getString(i, buffer, 128);
      Serial.println(buffer);
    } else {
      Serial.println("Unknown type");
    }
  }
}

//int sliderValues[9][6]; // An 8x5 array to hold the values for each slider
int levelArray [8][5];
int ch;
int bus;
int oldChLvl[8][5];
//char oscMsg [8][5][20];
//strcpy(chData[channel-1][button-1], "Hello"); // Copy the string "Hello" into the chData array for channel 1 and button 2
char * MessageBuilder(int ch, int bus) {
  static char str[20];
  sprintf(str, "/ch/%02d/mix/%02d/level", ch+1, bus+1); // Format the string using the channel and button variables, and copy it into the str array  (X32 and XR 18 same)
  return str;
}

////  main mute command  for XR18 is  /lr/mix/on  add 0 for off 1 for on
// for X32 is /main/st/mix/on
void sendMainMute(){
//OSCMessage msg("/lr/mix/on");
OSCMessage msg("/main/st/mix/on");
//Serial.println("Send Mute command");
if (button_MuteOn){
msg.add(0);  //mute btn on means mix off so 0
Serial.println("MUTE ON");
}
else {
  msg.add(1);
  Serial.println("Mute off");
}
Udp.beginPacket(outIp, outPort);
msg.send(Udp);
Udp.endPacket();  
}

void sendOscMessageAndWaitForResponse() {
  //if (ch1.intValue() != ch1Old){
for (int c = 0; c<8; c++){
  for (int b = 0; b<5; b++){
    int level = sliders[c][b]->intValue();
//    Serial.println("Slider Ch:" + String(c) + "bus"  + String(b) + "Val:  " + String(level)+ "     oldVal:  " +  String(oldChLvl[c][b]));
    if (level != oldChLvl[c][b]){
        OSCMessage msg(MessageBuilder(c, b));
 //   Serial.println("msg:  " + String(MessageBuilder(c, b)));    
    float levelFloat = ((float)level) / 1000;
 //   Serial.print("float  ");
 //   Serial.println(levelFloat);
    msg.add(levelFloat);
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
   // oldChLvl[c][b] = sliders[c][b].intValue();
   oldChLvl[c][b] = level;
   levelArray[c][b] = level;
  // Serial.print("Level: ");
//Serial.println(level);
   //write int to eeprom single position. 
   int addr = calcAdd(0, c, b);
  writeIntToEEPROM(addr, level); 
  delay(20);
 // Serial.print("Address: ");
 // Serial.println(addr);
//  writeIntArrayToEEPROM(1, eepromVar1, 8, 5);
   //add new values to eeprom
  // set_Save();
  // set_PrintAll();
int temp;
EEPROM.get(addr, temp);
//Serial.print("eeprom:  ");
//Serial.println(temp);
// Serial.print("Address: ");
 // Serial.println(addr);
  while (Udp.parsePacket() == 0) {
 //   Serial.println("UDPParse is zero");
    delay(10);
    break;
  }

//  OSCMessage response;
 // while (Udp.available()) {
   // response.fill(Udp.read());
//  }
//  handleOscResponse(response); 
    }
  }
}

}  
void setup() {
pinMode(BUILTIN_LED, OUTPUT);
digitalWrite(BUILTIN_LED, LOW); //power indicator
//setup eprom
//write 1 bulk data 
//EEPROM.begin(512);
EEPROM.begin(EEPROM_SIZE);
  // Example WIFI setup as an access point. Change this to whatever suits you, best.
  // WiFi.mode(WIFI_AP);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(local_IP, gateway, subnet);
  //  WiFi.softAPConfig (IPAddress (192,168,4,1), IPAddress (0,0,0,0), IPAddress (255,255,255,0));
  //WiFi.softAPConfig (IPAddress (192,168,1,222), IPAddress (0,0,0,0), IPAddress (255,255,255,0));
  // WiFi.softAP("EmbAJAXTest", "12345678");
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
//uncommnet to force restore default then re-comment next boot
//EEPROM.put(0,0);


//readIntArrayFromEEPROM(1, sliders, 8, 5);
//readIntArrayFromEEPROM(1, settings.levelArray);
EEPROM.get(0, levelArray);  // 1st address is for defauls load control, next address is ssize of int so 4 more (o for no default use) 
delay(50);
Serial.println("Restore Fader levels");
for(int i = 0; i<8; i++){
    for (int a = 0; a<5; a++){
      sliders [i][a] -> setValue(levelArray[i][a]); 
    //  Serial.println(levelArray[i][a]);
    }
  }
delay(50);
  //new for 2 pages
  driver.installPage(&page, "/", updateUI);
   driver.installPage(&page1, "/page1", updateUI);
   delay(50);
  driver.installPage(&page2, "/page2", updateUI);
  delay(50);
  driver.installPage(&page3, "/page3", updateUI);
  delay(50);
  driver.installPage(&page4, "/page4", updateUI);
  delay(50);
  driver.installPage(&page5, "/page5", updateUI);
  delay(50);
  driver.installPage(&page6, "/page6", updateUI);
  delay(50);
  server.begin();

delay(200);
// aded for test setup poage
    valtext.setPlaceholder("192,168,1,1");
    valtext.setPattern("\\d{1,3}\\,\\d{1,3}\\,\\d{1,3}\\,\\d{1,3}");

updateUI(); // to initialize display
//valtext_d.setValue("0.0.0.0");
delay(200);

//add for UDP receive. 
Udp.begin(10023);

sendMainMute();
}

//void testCh1(OSCMessage &msg, int addrOffset){  // this Works!
//  Serial.print("CHANNEL 1 ROUTE");
//}

//float level = 0;
void loop() {
  server.handleClient();
  sendOscMessageAndWaitForResponse();
  //}
  unsigned long currentMillis;
  // Check is a /xremote OSC command needs to be sent to X32
  currentMillis = millis();
  if ((currentMillis - XremoteMillis) > 9000) {
  //  Serial.printf("Sending Remote\n");
    OSCMessage msg("/xremote");
    // msg.add(1);
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
    //  send_udp(s_remt, 12);
    XremoteMillis = currentMillis;
    delay(100);
    Serial.println("****remote sent****");
  }
 
  //new for 2 page
  driver.loopHook();
delay(100);
////try adding UPD rec to syc settings
 int packetSize = Udp.parsePacket();
  if (packetSize) {   
  OSCMessage response; 
  while(packetSize--)
      response.fill(Udp.read());
      delay(50);
   response.getAddress(packet, 0);
//Serial.println(packet);
String address = String(packet);
mch = address.substring(4,6).toInt();
mbus = address.substring(11,13).toInt();
//response.route("/ch/01", testCh1);
//printOSCMessage(response);
float level = response.getFloat(0);
mixerLevel = level*1000;
sliderChange = true;
updateUI(); 

  }
  delay(50);  
}
