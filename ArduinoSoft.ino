#include <UIPEthernet.h> // Used for Ethernet

#define PIN_LED 2

#define PIN_RELAY_0 4




// **** ETHERNET SETTING ****
// Ethernet MAC address - must be unique on your network
byte mac[] = { 0x41, 0x44, 0x41, 0x43, 0x48, 0x41 };
//Ethernet interface IP address (unique in your network)
IPAddress ip(192, 168, 88, 248);                        
//ethernet interface IP port (80 = http)
EthernetServer server(80);
EthernetClient client;
String str = "";


void setup() {

  pinMode(PIN_RELAY_0, OUTPUT);


  SW(0, LOW);


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();

}

void loop() {
  client = server.available();

  if( client ){


    boolean currentLineIsBlank = true;
    String line = "";
    bool firstLine = true;
    
    while (client.connected()){
      
      if (client.available()) {
        char c = client.read();
      

   

        if (c == '\n') {
          
          currentLineIsBlank = true;
          if( firstLine ) {
            firstLine = false;

            if( line.indexOf( "reboot" ) >= 0 ){
              SW(0, HIGH);
              delay(4000);
              SW(0, LOW);
            }
          }
        }else if (c != '\r') {
          currentLineIsBlank = false;
          if( firstLine ) {
            line = line + c;
          }
        }
      }
    }

    delay(10);

    client.stop();

  }
}

bool SW_state(byte num){
  bool val;
  switch(num){
    case 0: val = digitalRead(PIN_RELAY_0); break;

  }
  return !val;
}

void SW(byte num, bool val){
  val = !val;
  switch(num){
    case 0: digitalWrite(PIN_RELAY_0, val); break;

  }
}



