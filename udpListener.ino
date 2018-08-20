#include <SPI.h>
#include <Ethernet.h>

// MAC address
byte mac[] =  { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  

// IP address
byte ip[] = {192,168,88,248};

EthernetServer server(80); 
EthernetClient client;

// Name pins
int SW1 = 7;
int SW2 = 6;
int SW3 = 5;



void setup()
{

  // Initiate pins
  pinMode(SW1, OUTPUT);
  pinMode(SW2, OUTPUT);
  pinMode(SW3, OUTPUT);

  // Setting up pins
  digitalWrite(SW1, LOW);
  digitalWrite(SW2, LOW);
  digitalWrite(SW3, LOW);

  // Begining all
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(19200);

  Serial.print("Connection details:\n");
  Serial.println(Ethernet.localIP());

  
}

void loop () {

  client = server.available();

  if( client ){
    Serial.println("BEGIN-------------->\n");

    boolean currentLineIsBlank = true;
    String line = "";
    bool firstLine = true;
    
    while (client.connected()){
      
      if (client.available()) {
        char c = client.read();
        Serial.print(c);

        if (c == '\n' && currentLineIsBlank) {
          String html = "{";
          for(int i=0; i<2; i++){
            if( SW_state(i) ){
              html += "\"sw_"+String(i)+"\":1 ";
            }else{
              html += "\"sw_"+String(i)+"\":0 ";
            }

            if( i<1 ) html += ",";
          }
          html += "}";
          
          client.println(html);
          break;
        }

        if (c == '\n') {
          
          currentLineIsBlank = true;
          if( firstLine ) {
            firstLine = false;
            Serial.println("-------------------------------" + line);
            if( line.indexOf( "reboot=0" ) >= 0 ){
              digitalWrite(SW1, HIGH);
              delay(3000);
              digitalWrite(SW1, LOW);
            }else if( line.indexOf( "reboot=1" ) >= 0 ){
              digitalWrite(SW2, HIGH);
              delay(3000);
              digitalWrite(SW2, LOW);
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
    Serial.println("END-------------->\n");
  }
}

bool SW_state(byte num){
  bool val;
  switch(num){
    case 0: val = digitalRead(SW1); break;
    case 1: val = digitalRead(SW2); break;
  }
  return !val;
}

void SW(byte num, bool val){
  val = !val;
  switch(num){
    case 0: digitalWrite(SW1, val); break;
    case 1: digitalWrite(SW2, val); break;
  }
}

