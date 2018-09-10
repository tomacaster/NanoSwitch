#include <SPI.h>
#include <Ethernet.h>

// MAC address
byte mac[] =  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFA, 0xED };  

// IP address
byte ip[] = {192,168,88,249};

EthernetServer server(80); 
EthernetClient client;

// Name pins
int SW1 = 7;

void setup()
{

  // Initiate pins
  pinMode(SW1, OUTPUT);

  // Setting up pins
  digitalWrite(SW1, LOW);


  // Begining all
  Ethernet.begin(mac, ip);
  server.begin();
  
}

void loop () {

  client = server.available();

  if(client)
  {
    
    boolean currentLineIsBlank = true;
    String line = "";
    bool firstLine = true;
    
    while (client.connected()){
      
      if (client.available()) {
        char c = client.read();
        if (c == '\n' && currentLineIsBlank) {
          String html;
          for(int i=0; i<2; i++){
            if( SW_state(i) ){
              html = "OK";
            }
          }
          
          client.println(html);
          break;
        }

        if (c == '\n') {
          
          currentLineIsBlank = true;
          if( firstLine ) {
            firstLine = false;
            if( line.indexOf( "reboot=0" ) >= 0 ){
              digitalWrite(SW1, HIGH);
              delay(3000);
              digitalWrite(SW1, LOW);
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
    case 0: val = digitalRead(SW1); break;
  }
  return !val;
}

void SW(byte num, bool val){
  val = !val;
  switch(num){
    case 0: digitalWrite(SW1, val); break;
  }
}
